#include"raylib.h"
#include<fstream>
#include<iostream>
#include "functions.h"

void save(bool greensturn, int grid[5][5]) {
	std::ofstream out;
	out.open("gamedata.txt");
	out << greensturn << "\n";
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			out << grid[i][j] << "\n";
		}
	}
	out.close();
}

Vector2 lerp(Vector2 startpos, Vector2 finalpos, float speed) {
	Vector2 result = startpos;
	result.x = startpos.x + (speed * (finalpos.x - startpos.x));
	result.y = startpos.y + (speed * (finalpos.y - startpos.y));
	return result;
}

//selectedrow and column = 10 means no bead is selected currently
int selectedrow = 10;
int selectedcol = 10;

//resetgamegrid - used to reset board when new game
int startinggrid[5][5] = {
		{2, 2, 2, 2, 2},
		{2, 2, 2, 2, 2},
		{2, 2, 0, 1, 1},
		{1, 1, 1, 1, 1},
		{1, 1, 1, 1, 1},
};

//grid of positions(x and y) for all the places beads can be
Vector2 positions[5][5] = {
		{{130,75}, {130 * 2,75}, {130 * 3,75}, {130 * 4,75}, {130 * 5,75},},
		{{130,75 * 2}, {130 * 2,75 * 2}, {130 * 3,75 * 2}, {130 * 4,75 * 2}, {130 * 5,75 * 2},},
		{{130,75 * 3}, {130 * 2,75 * 3}, {130 * 3,75 * 3}, {130 * 4,75 * 3}, {130 * 5,75 * 3},},
		{{130,75 * 4}, {130 * 2,75 * 4}, {130 * 3,75 * 4}, {130 * 4,75 * 4}, {130 * 5,75 * 4},},
		{{130,75 * 5}, {130 * 2,75 * 5}, {130 * 3,75 * 5}, {130 * 4,75 * 5}, {130 * 5,75 * 5},},
};

Vector2 startingpositions[5][5] = {
		{{130,75 - 450}, {130 * 2,75 - 450}, {130 * 3,75 - 450}, {130 * 4,75 - 450}, {130 * 5,75 - 450},},
		{{130,75 * 2 - 455}, {130 * 2,75 * 2 - 455}, {130 * 3,75 * 2 - 455}, {130 * 4,75 * 2 - 455}, {130 * 5,75 * 2 - 455},},
		{{130,75 * 3 - 460}, {130 * 2,75 * 3 - 460}, {130 * 3,75 * 3 - 460}, {130 * 4,75 * 3 - 460}, {130 * 5,75 * 3 - 460},},
		{{130,75 * 4 - 475}, {130 * 2,75 * 4 - 475}, {130 * 3,75 * 4 - 475}, {130 * 4,75 * 4 - 475}, {130 * 5,75 * 4 - 475},},
		{{130,75 * 5 - 500}, {130 * 2,75 * 5 - 500}, {130 * 3,75 * 5 - 500}, {130 * 4,75 * 5 - 500}, {130 * 5,75 * 5 - 500},},
};

Vector2 ingamepositions[5][5] = {
		{{130,75}, {130 * 2,75}, {130 * 3,75}, {130 * 4,75}, {130 * 5,75},},
		{{130,75 * 2}, {130 * 2,75 * 2}, {130 * 3,75 * 2}, {130 * 4,75 * 2}, {130 * 5,75 * 2},},
		{{130,75 * 3}, {130 * 2,75 * 3}, {130 * 3,75 * 3}, {130 * 4,75 * 3}, {130 * 5,75 * 3},},
		{{130,75 * 4}, {130 * 2,75 * 4}, {130 * 3,75 * 4}, {130 * 4,75 * 4}, {130 * 5,75 * 4},},
		{{130,75 * 5}, {130 * 2,75 * 5}, {130 * 3,75 * 5}, {130 * 4,75 * 5}, {130 * 5,75 * 5},},
};

Vector2 lerpingpositions[5][5] = {
		{{130,75 - 450}, {130 * 2,75 - 450}, {130 * 3,75 - 450}, {130 * 4,75 - 450}, {130 * 5,75 - 450},},
		{{130,75 * 2 - 455}, {130 * 2,75 * 2 - 455}, {130 * 3,75 * 2 - 455}, {130 * 4,75 * 2 - 455}, {130 * 5,75 * 2 - 455},},
		{{130,75 * 3 - 460}, {130 * 2,75 * 3 - 460}, {130 * 3,75 * 3 - 460}, {130 * 4,75 * 3 - 460}, {130 * 5,75 * 3 - 460},},
		{{130,75 * 4 - 475}, {130 * 2,75 * 4 - 475}, {130 * 3,75 * 4 - 475}, {130 * 4,75 * 4 - 475}, {130 * 5,75 * 4 - 475},},
		{{130,75 * 5 - 500}, {130 * 2,75 * 5 - 500}, {130 * 3,75 * 5 - 500}, {130 * 4,75 * 5 - 500}, {130 * 5,75 * 5 - 500},},
};

void DrawBeads(int grid[5][5], Texture2D bead_green, Texture2D bead_red, Texture2D empty, Texture2D circle, bool& ingame) {
	if (ingame) {
		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 5; j++) {
				positions[i][j] = ingamepositions[i][j];
			}
		}
	}
	else {
		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 5; j++) {
				positions[i][j] = startingpositions[i][j];
			}
		}
	}
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			lerpingpositions[i][j] = lerp(lerpingpositions[i][j], positions[i][j], 0.06);
		}
	}
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			if (grid[i][j] == 0) {
				DrawTextureV(empty, { lerpingpositions[i][j].x - 58, lerpingpositions[i][j].y - 58 }, WHITE);
			}
			else if (grid[i][j] == 1) {
				DrawTextureV(bead_green, { lerpingpositions[i][j].x - 58, lerpingpositions[i][j].y - 58 }, WHITE);
			}
			else if (grid[i][j] == 2) {
				DrawTextureV(bead_red, { lerpingpositions[i][j].x - 58, lerpingpositions[i][j].y - 58 }, WHITE);
			}
			else if (grid[i][j] == 3) {
				DrawTextureV(circle, { lerpingpositions[i][j].x - 58, lerpingpositions[i][j].y - 58 }, WHITE);
				DrawTextureV(bead_green, { lerpingpositions[i][j].x - 58, lerpingpositions[i][j].y - 58 }, WHITE);
			}
			else if (grid[i][j] == 4) {
				DrawTextureV(circle, { lerpingpositions[i][j].x - 58, lerpingpositions[i][j].y - 58 }, WHITE);
				DrawTextureV(bead_red, { lerpingpositions[i][j].x - 58, lerpingpositions[i][j].y - 58 }, WHITE);
			}
			else if (grid[i][j] == 5) {
				DrawTextureV(bead_green, { lerpingpositions[i][j].x - 58, lerpingpositions[i][j].y - 58 }, WHITE);
			}
			else if (grid[i][j] == 6) {
				DrawTextureV(bead_red, { lerpingpositions[i][j].x - 58, lerpingpositions[i][j].y - 58 }, WHITE);
			}
			else if (grid[i][j] == 7) {
				DrawTextureV(empty, { lerpingpositions[i][j].x - 58, lerpingpositions[i][j].y - 58 }, BLACK);
			}
			else if (grid[i][j] == 8) {
				DrawTextureV(empty, { lerpingpositions[i][j].x - 58, lerpingpositions[i][j].y - 58 }, RED);
			}
		}
	}
}

void unselect(int grid[5][5]) {
	selectedrow = 10;
	selectedcol = 10;
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			if (grid[i][j] == 3) grid[i][j] = 1;
			if (grid[i][j] == 4) grid[i][j] = 2;
			if (grid[i][j] == 7) grid[i][j] = 0;
			if (grid[i][j] == 5) grid[i][j] = 1;
			if (grid[i][j] == 6) grid[i][j] = 2;
			if (grid[i][j] == 8) grid[i][j] = 0;
		}
	}
}

void showoptions(int i, int j, int grid[5][5]) {
	//horizontal and vertical
	if (i > 0 && grid[i - 1][j] == 0) grid[i - 1][j] = 7;	//down
	if (i < 4 && grid[i + 1][j] == 0) grid[i + 1][j] = 7;	//up
	if (j > 0 && grid[i][j - 1] == 0) grid[i][j - 1] = 7;	//left
	if (j < 4 && grid[i][j + 1] == 0) grid[i][j + 1] = 7;	//right
	//diagonals
	if ((i + j) % 2 == 0) {
		if (i < 4 && j < 4 && grid[i + 1][j + 1] == 0) grid[i + 1][j + 1] = 7;	//bottom right
		if (i < 4 && j > 0 && grid[i + 1][j - 1] == 0) grid[i + 1][j - 1] = 7;	//bottom left
		if (i > 0 && j < 4 && grid[i - 1][j + 1] == 0) grid[i - 1][j + 1] = 7;	//top right
		if (i > 0 && j > 0 && grid[i - 1][j - 1] == 0) grid[i - 1][j - 1] = 7;	//top left
	}
	//targets
	if (grid[i][j] == 3) {	//greensturn
		//horizontal and vertical
		if (i > 1 && grid[i - 2][j] == 0 && (grid[i - 1][j] == 2 || grid[i - 1][j] == 10)) {
			grid[i - 1][j] = 6;
			grid[i - 2][j] = 8;
		}
		if (i < 3 && grid[i + 2][j] == 0 && (grid[i + 1][j] == 2 || grid[i + 1][j] == 10)) {
			grid[i + 1][j] = 6;
			grid[i + 2][j] = 8;
		}
		if (j > 1 && grid[i][j - 2] == 0 && (grid[i][j - 1] == 2 || grid[i][j - 1] == 10)) {
			grid[i][j - 1] = 6;
			grid[i][j - 2] = 8;
		}
		if (j < 3 && grid[i][j + 2] == 0 && (grid[i][j + 1] == 2 || grid[i][j + 1] == 10)) {
			grid[i][j + 1] = 6;
			grid[i][j + 2] = 8;
		}
		//diagonal
		if ((i + j) % 2 == 0) {
			if (i > 1 && j > 1 && grid[i - 2][j - 2] == 0 && (grid[i - 1][j - 1] == 2 || grid[i - 1][j - 1] == 10)) {
				grid[i - 1][j - 1] = 6;
				grid[i - 2][j - 2] = 8;
			}
			if (i > 1 && j < 3 && grid[i - 2][j + 2] == 0 && (grid[i - 1][j + 1] == 2 || grid[i - 1][j + 1] == 10)) {
				grid[i - 1][j + 1] = 6;
				grid[i - 2][j + 2] = 8;
			}
			if (i < 3 && j > 1 && grid[i + 2][j - 2] == 0 && (grid[i + 1][j - 1] == 2 || grid[i + 1][j - 1] == 10)) {
				grid[i + 1][j - 1] = 6;
				grid[i + 2][j - 2] = 8;
			}
			if (i < 3 && j < 3 && grid[i + 2][j + 2] == 0 && (grid[i + 1][j + 1] == 2 || grid[i + 1][j + 1] == 10)) {
				grid[i + 1][j + 1] = 6;
				grid[i + 2][j + 2] = 8;
			}
		}
	}
	else if (grid[i][j] == 4) {	//redssturn
		//horizontal and vertical
		if (i > 1 && grid[i - 2][j] == 0 && (grid[i - 1][j] == 1 || grid[i - 1][j] == 9)) {
			grid[i - 1][j] = 5;
			grid[i - 2][j] = 8;
		}
		if (i < 3 && grid[i + 2][j] == 0 && (grid[i + 1][j] == 1 || grid[i + 1][j] == 9)) {
			grid[i + 1][j] = 5;
			grid[i + 2][j] = 8;
		}
		if (j > 1 && grid[i][j - 2] == 0 && (grid[i][j - 1] == 1 || grid[i][j - 1] == 9)) {
			grid[i][j - 1] = 5;
			grid[i][j - 2] = 8;
		}
		if (j < 3 && grid[i][j + 2] == 0 && (grid[i][j + 1] == 1 || grid[i][j + 1] == 9)) {
			grid[i][j + 1] = 5;
			grid[i][j + 2] = 8;
		}
		//diagonal
		if ((i + j) % 2 == 0) {
			if (i > 1 && j > 1 && grid[i - 2][j - 2] == 0 && (grid[i - 1][j - 1] == 1 || grid[i - 1][j - 1] == 9)) {
				grid[i - 1][j - 1] = 5;
				grid[i - 2][j - 2] = 8;
			}
			if (i > 1 && j < 3 && grid[i - 2][j + 2] == 0 && (grid[i - 1][j + 1] == 1 || grid[i - 1][j + 1] == 9)) {
				grid[i - 1][j + 1] = 5;
				grid[i - 2][j + 2] = 8;
			}
			if (i < 3 && j > 1 && grid[i + 2][j - 2] == 0 && (grid[i + 1][j - 1] == 1 || grid[i + 1][j - 1] == 9)) {
				grid[i + 1][j - 1] = 5;
				grid[i + 2][j - 2] = 8;
			}
			if (i < 3 && j < 3 && grid[i + 2][j + 2] == 0 && (grid[i + 1][j + 1] == 1 || grid[i + 1][j + 1] == 9)) {
				grid[i + 1][j + 1] = 5;
				grid[i + 2][j + 2] = 8;
			}
		}
	}
}

void MouseClick(int grid[5][5], bool& greensturn, Vector2& initialposition, Vector2& finalposition, Vector2& currentposition, float& t, int& soundnum) {
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			if (CheckCollisionPointCircle({ (float)GetMouseX(),(float)GetMouseY() }, lerpingpositions[i][j], 28.0)) {
				std::cout << "---------------------------------------" << std::endl;
				std::cout << "row " << i << std::endl;
				std::cout << "col " << j << std::endl;
				if (grid[i][j] == 7) {	//if the clicked place is option to move
					if (selectedrow >= 0 && selectedrow < 5 && selectedcol >= 0 && selectedcol < 5) {

						grid[selectedrow][selectedcol] = 0;

						initialposition = positions[selectedrow][selectedcol];
						currentposition = positions[selectedrow][selectedcol];
						finalposition = positions[i][j];
						t = 0;
						soundnum = 1;
						if (greensturn) {
							//grid[i][j] = 1;
							grid[i][j] = 9;
						}
						else if (!greensturn) {
							//grid[i][j] = 2;
							grid[i][j] = 10;
						}
						greensturn = !greensturn;
			
						unselect(grid);
					}
				}
				else if (grid[i][j] == 1 && greensturn) {	//if a green bead is clicked
					unselect(grid);
					grid[i][j] = 3;
					selectedrow = i;
					selectedcol = j;
					showoptions(i, j, grid);
				}
				else if (grid[i][j] == 2 && !greensturn) { //if a red bead is clicked
					unselect(grid);
					grid[i][j] = 4;
					selectedrow = i;
					selectedcol = j;
					showoptions(i, j, grid);
				}
				else if (grid[i][j] == 8) {	//attacking point is clicked

					soundnum = 2;	//lol

					initialposition = positions[selectedrow][selectedcol];
					currentposition = positions[selectedrow][selectedcol];
					finalposition = positions[i][j];

					t = 0;
					/*if (grid[selectedrow][selectedcol] == 3) grid[i][j] = 1;
					if (grid[selectedrow][selectedcol] == 4) grid[i][j] = 2;*/
					if (grid[selectedrow][selectedcol] == 3) grid[i][j] = 9;
					if (grid[selectedrow][selectedcol] == 4) grid[i][j] = 10;
					grid[selectedrow][selectedcol] = 0;
					int targetrow = (selectedrow + i) / 2;
					int targetcol = (selectedcol + j) / 2;
					grid[targetrow][targetcol] = 0;
					greensturn = !greensturn;
					unselect(grid);
				}
			}
		}
	}
}

void DrawTurn(bool& greensturn, Texture2D greens_turn, Texture2D reds_turn, Vector2 drawturnVector) {
	if (greensturn) {
		DrawTextureEx(greens_turn, drawturnVector, 0.0, 1, WHITE);
	}
	else {
		DrawTextureEx(reds_turn, drawturnVector, 0.0, 1, WHITE);
	}
}

int Check_Win(int grid[5][5], bool& won) {
	int greenbeads = 0;
	int redbeads = 0;
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			if (grid[i][j] == 1 || grid[i][j] == 3 || grid[i][j] == 5 || grid[i][j] == 9) {
				greenbeads++;
			}
			else if (grid[i][j] == 2 || grid[i][j] == 4 || grid[i][j] == 6 || grid[i][j] == 10) {
				redbeads++;
			}
		}
	}
	if (redbeads == 0) {
		return 1;
	}
	else if (greenbeads == 0) {
		return 2;
	}
	else {
		return 0;
	}
}

void randomizeturn(bool& greensturn) {
	srand(time(0));
	greensturn = rand() % 2;
}

void load(bool& greensturn, int grid[5][5]) {
	
	std::ifstream in;
	in.open("gamedata.txt");
	if (in.fail()) { DrawRectangle(0, 0, 100, 100, RED); }//function to show no savedata found!

	//loading greensturn
	char ch = in.get();
	ch -= '0';
	greensturn = ch;

	in.get(); // goes to next next line - where grid starts

	//loading grid
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			char gch = in.get();
			gch -= '0';
			grid[i][j] = gch;
			in.get();
		}
	}

	in.close();

	unselect(grid);
}

void resetgrid(int grid[5][5]) {
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			positions[i][j] = startingpositions[i][j];
			grid[i][j] = startinggrid[i][j];
		}
	}
}