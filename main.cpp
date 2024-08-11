#include<iostream>
#include "raylib.h"
#include"functions.h"
using namespace std;

void soundsFunc(Sound, Sound, Sound, Sound, Sound, int&);

int main()
{
    //initializing grid at start of game where 0 = empty; 1 = green; 2 = red; 3 = greenselected;
    //4 = redselected; 5 = greentarget; 6 = redtarget; 7 = option; 8 = option to take targetbead
    //9 = place green bead here after timer; 10 same as 9 but red
    int grid[5][5] = {
        {2, 2, 2, 2, 2},
        {2, 2, 2, 2, 2},
        {2, 2, 0, 1, 1},
        {1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1},
    };

    ///making window and initializing audio device
    const int screenWidth = 800;
    const int screenHeight = 450;
    InitWindow(screenWidth, screenHeight, "12Beads");
    InitAudioDevice();
    SetTargetFPS(60);
    
    //window icon
    Image winicon = LoadImage("./assets/windowicon.png");
    SetWindowIcon(winicon);


    //loading all required assets
    Texture2D bgcolor = LoadTexture("./assets/background_blue.png");
    Texture2D mainmenutext = LoadTexture("./assets/12beads_text.png");
    Texture2D starttext = LoadTexture("./assets/start_text.png");
    Texture2D resumetext = LoadTexture("./assets/resume_text.png");
    Texture2D exittext = LoadTexture("./assets/exit_text.png");
    Texture2D aboutgame = LoadTexture("./assets/about_game_1.png");
    Texture2D mainmenu = LoadTexture("./assets/main_menu.png");
    Texture2D aboutgametext = LoadTexture("./assets/aboutgametext.png");
    Texture2D x = LoadTexture("./assets/X.png");
    Texture2D aboutgamewindow = LoadTexture("./assets/aboutgame.png");
    Texture2D mute = LoadTexture("./assets/mute.png");
    Texture2D unmute = LoadTexture("./assets/unmute.png");
    Texture2D gridlines = LoadTexture("./assets/grid.png");
    Texture2D bead_green = LoadTexture("./assets/bead_green.png");
    Texture2D bead_red = LoadTexture("./assets/bead_red.png");
    Texture2D empty = LoadTexture("./assets/empty.png");
    Texture2D circle = LoadTexture("./assets/circle.png");
    Texture2D greenwins = LoadTexture("./assets/greenWINS.png");
    Texture2D redwins = LoadTexture("./assets/redWINS.png");
    Texture2D greens_turn = LoadTexture("./assets/greens_turn.png");
    Texture2D reds_turn = LoadTexture("./assets/reds_turn.png");

    Sound placebeadsfx = LoadSound("./assets/placebead.mp3");
    Sound takebeadsfx = LoadSound("./assets/takebead.mp3");
    Sound winsfx = LoadSound("./assets/winsfx.mp3");
    Sound whoosh1sfx = LoadSound("./assets/whoosh1.mp3");
    Sound whoosh2sfx = LoadSound("./assets/whoosh2.mp3");

    //initializing Vector2s for positions of options
    Vector2 startrecVector = { 263 - 24, 199 };
    Vector2 resumerecVector = { 263 - 24, 199 };
    Vector2 aboutgamerecVector = { 262 + 22, 270 };
    Vector2 exitrecVector = { 360 - 20, 350 };
    Vector2 mainmenutextVector = { 178, 50 - 50};
    Vector2 gridlinespos = { 0, -450 };
    Vector2 mainmenuVector = { 660+150, 420 };
    Vector2 drawturnVector = { -350, 0 };
    Vector2 aboutgamewindowVector = { 0, 450 };
    Vector2 xrecVector = { 608, 77 + 450 };
    Vector2 winscreenVector = { 0,450 };
    Vector2 mainmenu2Vector = { 271,210 + 450 };

    //initializing their finalpositions(same as them while initializing)
    Vector2 finalstartrecVector = { 100, 199 };
    Vector2 finalresumerecVector = { 410, 199 };
    Vector2 finalaboutgamerecVector = { 262, 270 };
    Vector2 finalexitrecVector = { 360, 350 };
    Vector2 finalmainmenutextvector = { 178, 50 };
    Vector2 finalgridlinespos = { 0, -450 };
    Vector2 finalmainmenuVector = { 660+150, 420 };
    Vector2 finaldrawturnVector = { -350, 0 };
    Vector2 finalaboutgamewindowVector = { 0,450 };
    Vector2 finalxrecVector = { 608, 77 + 450};
    Vector2 finalwinscreenVector = { 0,450 };
    Vector2 finalmainmenu2Vector = { 271,210 + 450};

    //initializing vectors for the animated bead
    Vector2 initialposition = { -100,-100 };
    Vector2 finalposition = { -100, -100 };
    Vector2 currentposition = { -100, -100 };

    //blackscreen bool is true if the screen should be darker(during about screen or win screen)
    //alpha variable is the value of transparency
    bool blackscreen = false;
    float alpha = 0.8;

    bool greensturn = true;

    bool won = false;   //write laters

    bool ingame = false;

    bool inabout = false;

    bool lastwingreen = true;   //this bool is used to decide which win screen to draw above

    float t = 0;    //timer - for managing animated bead

    float volume = 1.0; //master volume

    float gvolume = 0.0;    //game music volume

    float mvolume = 0.0;    //mainmenu music volume

    int winint = 0; //this represents who won. 1- green won, 2- red won, 0- noone won (yet)

    int soundnum = 0;   //changing this int plays various sounds

    bool winsfxplayed = false;  //variable used for NOT repeating the winsfx

    //music initializing
    Music gamemusic = LoadMusicStream("./assets/gamemusic.mp3");
    PlayMusicStream(gamemusic);

    Music menumusic = LoadMusicStream("./assets/mainmenumusic.mp3");
    PlayMusicStream(menumusic);

    //game loop
    while (!WindowShouldClose()) {
        // Update
        UpdateMusicStream(menumusic);
        SetMusicVolume(menumusic, mvolume);

        UpdateMusicStream(gamemusic);
        SetMusicVolume(gamemusic, gvolume);

        if (ingame) {
            gvolume = volume;
            mvolume = 0.0;
        }
        else {
            gvolume = 0.0;
            mvolume = volume;
        }

        //this plays various sounds based on the 'soundnum' integar.
        soundsFunc(placebeadsfx, takebeadsfx, winsfx, whoosh1sfx, whoosh2sfx, soundnum);

        //updating variables 1

        Color a = Fade(BLACK, alpha);
        if (blackscreen) {
            if (alpha < 0.75) alpha += 0.06;
        }
        else if (!blackscreen) {
            if (alpha > 0) alpha -= 0.06;
        }

        //making rectangles for detecting mouse clicks(in while loop so their position also changes)
        Rectangle startrec = { startrecVector.x, startrecVector.y, 270, 50 };
        Rectangle resumerec = { resumerecVector.x, resumerecVector.y, 322, 52 };
        Rectangle aboutgamerec = { aboutgamerecVector.x, aboutgamerecVector.y, 300, 43 };
        Rectangle exitrec = { exitrecVector.x, exitrecVector.y, 75, 43 };
        Rectangle xrec = { xrecVector.x,xrecVector.y,34,40 };
        Rectangle mainmenurec = { mainmenu2Vector.x, mainmenu2Vector.y, 258, 51 };
        Rectangle muterec = { 0,400,50,50 };

        //checking for collision on options
        bool mouseonstart = CheckCollisionPointRec({ (float)GetMouseX(), (float)GetMouseY() }, startrec);
        bool mouseonresume = CheckCollisionPointRec({ (float)GetMouseX(), (float)GetMouseY() }, resumerec);
        bool mouseonexit = CheckCollisionPointRec({ (float)GetMouseX(), (float)GetMouseY() }, exitrec);
        bool mouseonaboutgame = CheckCollisionPointRec({ (float)GetMouseX(), (float)GetMouseY() }, aboutgamerec);
        bool mouseonx = CheckCollisionPointRec({ (float)GetMouseX(), (float)GetMouseY() }, xrec);
        bool mouseonmuterec = (CheckCollisionPointRec({ (float)GetMouseX(), (float)GetMouseY() }, muterec) && !inabout);


        //updating vector2s
        startrecVector = lerp(startrecVector, finalstartrecVector, 0.08);
        resumerecVector = lerp(resumerecVector, finalresumerecVector, 0.08);
        aboutgamerecVector = lerp(aboutgamerecVector, finalaboutgamerecVector, 0.07);
        exitrecVector = lerp(exitrecVector, finalexitrecVector, 0.06);
        mainmenutextVector = lerp(mainmenutextVector, finalmainmenutextvector, 0.04);
        mainmenuVector = lerp(mainmenuVector, finalmainmenuVector, 0.06);
        drawturnVector = lerp(drawturnVector, finaldrawturnVector, 0.06);
        aboutgamewindowVector = lerp(aboutgamewindowVector, finalaboutgamewindowVector, 0.2);
        xrecVector = lerp(xrecVector, finalxrecVector, 0.2);
        winscreenVector = lerp(winscreenVector, finalwinscreenVector, 0.07);
        mainmenu2Vector = lerp(mainmenu2Vector, finalmainmenu2Vector, 0.07);

        //mouse clicks
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
            if (mouseonstart && !inabout) {
                soundnum = 4;
                resetgrid(grid);
                finalstartrecVector = { finalstartrecVector.x, finalstartrecVector.y + 260 };
                finalresumerecVector = { finalresumerecVector.x, finalresumerecVector.y + 260 };
                finalaboutgamerecVector = { finalaboutgamerecVector.x, finalaboutgamerecVector.y + 240 };
                finalexitrecVector = { finalexitrecVector.x, finalexitrecVector.y + 220 };
                finalmainmenutextvector = { finalmainmenutextvector.x, finalmainmenutextvector.y - 150 };
                finalgridlinespos.y = 0;
                finalmainmenuVector.x -= 150;
                finaldrawturnVector.x += 350;
                ingame = true;
                randomizeturn(greensturn);
            }
            else if (mouseonresume && !inabout) {
                soundnum = 4;
                load(greensturn, grid);
                finalstartrecVector = { finalstartrecVector.x, finalstartrecVector.y + 260 };
                finalresumerecVector = { finalresumerecVector.x, finalresumerecVector.y + 260 };
                finalaboutgamerecVector = { finalaboutgamerecVector.x, finalaboutgamerecVector.y + 240 };
                finalexitrecVector = { finalexitrecVector.x, finalexitrecVector.y + 220 };
                finalmainmenutextvector = { finalmainmenutextvector.x, finalmainmenutextvector.y - 150 };
                finalgridlinespos.y = 0;
                finalmainmenuVector.x -= 150;
                finaldrawturnVector.x += 350;
                ingame = true;
            }
            else if (mouseonaboutgame) {
                soundnum = 5;
                finalaboutgamewindowVector.y = 15;
                finalxrecVector.y = 77;
                blackscreen = true;
                inabout = true;
            }
            else if (mouseonx) {
                soundnum = 5;
                finalaboutgamewindowVector.y = 450;
                finalxrecVector.y = 77 + 450;
                blackscreen = false;
                inabout = false;
            }
            else if (mouseonmuterec) {
                std::cout << "toggled mute!!" << std::endl;
                if (volume == 1.0) volume = 0.0;
                else if (volume == 0.0) volume = 1.0;
            }
            else if (mouseonexit && !inabout) {
                break;
            }
        }

        //update variables 2

        if(ingame) winint = Check_Win(grid, won);

        currentposition = lerp(currentposition, finalposition, 0.4);

        gridlinespos = lerp(gridlinespos, finalgridlinespos, 0.07);

        Rectangle main_menu_button = { 660,420,130,40 };        
        bool mouseonmainmenubutton = CheckCollisionPointRec({ (float)GetMouseX(), (float)GetMouseY() }, main_menu_button);
        bool mouseonmainmenubutton2 = CheckCollisionPointRec({ (float)GetMouseX(), (float)GetMouseY() }, mainmenurec);

        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && mouseonmainmenubutton && ingame && winint == 0) {
            soundnum = 5;
            winsfxplayed = false;
            finalstartrecVector = { finalstartrecVector.x, finalstartrecVector.y - 260 };
            finalresumerecVector = { finalresumerecVector.x, finalresumerecVector.y - 260 };
            finalaboutgamerecVector = { finalaboutgamerecVector.x, finalaboutgamerecVector.y - 240 };
            finalexitrecVector = { finalexitrecVector.x, finalexitrecVector.y - 220 };
            finalmainmenutextvector = { finalmainmenutextvector.x, finalmainmenutextvector.y + 150 };
            finalgridlinespos.y = -450;
            finalmainmenuVector.x += 150;
            finaldrawturnVector.x -= 350;
            initialposition.y -= 450;
            finalposition.y -= 450;
            ingame = false;
        }
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            MouseClick(grid, greensturn, initialposition, finalposition, currentposition, t, soundnum);
        }
        t += 1;
        if (t > 15) {
            for (int i = 0; i < 5; i++) {
                for (int j = 0; j < 5; j++) {
                    if (grid[i][j] == 9) grid[i][j] = 1;
                    else if (grid[i][j] == 10) grid[i][j] = 2;
                }
            }
            if (ingame) {
                save(greensturn, grid); //saving after timer to not save a 9 or a 10.
            }
        }

        // Draw
        BeginDrawing();

        //main menu drawing
        //DrawRectangleRec(muterec, mouseonmuterec ? WHITE : GRAY); //  DELETE after playtest

        DrawTextureV(bgcolor, { 0, 0 }, WHITE);
        DrawTextureV(mainmenutext, mainmenutextVector, WHITE);
        DrawTextureV(starttext, startrecVector, mouseonstart ? WHITE : GRAY);
        DrawTextureV(resumetext, resumerecVector, mouseonresume ? WHITE : GRAY);
        DrawTextureV(aboutgametext, aboutgamerecVector, mouseonaboutgame ? WHITE : GRAY);
        DrawTextureV(exittext, exitrecVector, mouseonexit ? WHITE : GRAY);
        if (volume == 0.0) DrawTextureRec(mute, muterec, { 0, 400 }, mouseonmuterec ? WHITE : GRAY);
        else DrawTextureRec(unmute, muterec, { 0, 400 }, mouseonmuterec ? WHITE : GRAY);
        DrawRectangle(0, 0, 800, 450, a);
        DrawTextureV(aboutgamewindow, aboutgamewindowVector, WHITE);
        DrawTextureV(x, xrecVector, mouseonx ? WHITE : GRAY);
        
        //play winsfx once if the win int is 1 or 2
        if (winint != 0 && !winsfxplayed) {
            soundnum = 3;
            winsfxplayed = true;
        }

        //game drawing

        DrawTurn(greensturn, greens_turn, reds_turn, drawturnVector);
        DrawTextureV(gridlines, gridlinespos, WHITE);
        //only draw animated bead if the timer has not ended
        if(t <= 15) {
            DrawTextureV(greensturn ? bead_red : bead_green, { currentposition.x - 58, currentposition.y - 58 }, WHITE);
        }
        DrawBeads(grid, bead_green, bead_red, empty, circle, ingame);
        DrawTextureEx(mainmenu, mainmenuVector, 0.0, 0.5, mouseonmainmenubutton ? WHITE : GRAY);

        //theses if statement slides up the appropriate winscreen and also contains code for the option in 
        //the winscreen, going back to main menu
        if (winint == 1) {
            blackscreen = true;
            bool mouseonmainmenubutton2 = CheckCollisionPointRec({ (float)GetMouseX(), (float)GetMouseY() }, mainmenurec);
            DrawTextureV(greenwins, winscreenVector, WHITE);
            finalwinscreenVector.y = 0;
            finalmainmenu2Vector.y = 210;
            DrawTextureV(mainmenu, mainmenu2Vector, mouseonmainmenubutton2 ? WHITE : GRAY);
            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && mouseonmainmenubutton2) {
                soundnum = 5;
                lastwingreen = true;
                ingame = false;
                finalstartrecVector = { finalstartrecVector.x, finalstartrecVector.y - 260 };
                finalresumerecVector = { finalresumerecVector.x, finalresumerecVector.y - 260 };
                finalaboutgamerecVector = { finalaboutgamerecVector.x, finalaboutgamerecVector.y - 240 };
                finalexitrecVector = { finalexitrecVector.x, finalexitrecVector.y - 220 };
                finalmainmenutextvector = { finalmainmenutextvector.x, finalmainmenutextvector.y + 150 };
                finalgridlinespos.y = -450;
                finalmainmenuVector.x += 150;
                finaldrawturnVector.x -= 350;
                initialposition.y -= 450;
                finalposition.y -= 450;
                winint = 0;
            }
            else if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && mouseonmainmenubutton) {
                soundnum = 5;
                lastwingreen = true;
                ingame = false;
                finalstartrecVector = { finalstartrecVector.x, finalstartrecVector.y - 260 };
                finalresumerecVector = { finalresumerecVector.x, finalresumerecVector.y - 260 };
                finalaboutgamerecVector = { finalaboutgamerecVector.x, finalaboutgamerecVector.y - 240 };
                finalexitrecVector = { finalexitrecVector.x, finalexitrecVector.y - 220 };
                finalmainmenutextvector = { finalmainmenutextvector.x, finalmainmenutextvector.y + 150 };
                finalgridlinespos.y = -450;
                finalmainmenuVector.x += 150;
                finaldrawturnVector.x -= 350;
                initialposition.y -= 450;
                finalposition.y -= 450;
                winint = 0;
            }
        }
        else if (winint == 2) {
            lastwingreen = false;
            blackscreen = true;
            DrawTextureV(redwins, winscreenVector, WHITE);
            finalwinscreenVector.y = 0;
            finalmainmenu2Vector.y = 210;
            DrawTextureV(mainmenu, mainmenu2Vector, mouseonmainmenubutton2? WHITE : GRAY);
            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && mouseonmainmenubutton2) { 
                soundnum = 5;
                winsfxplayed = false;
                ingame = false;
                finalmainmenu2Vector.y = 210+450;
                finalstartrecVector = { finalstartrecVector.x, finalstartrecVector.y - 260 };
                finalresumerecVector = { finalresumerecVector.x, finalresumerecVector.y - 260 };
                finalaboutgamerecVector = { finalaboutgamerecVector.x, finalaboutgamerecVector.y - 240 };
                finalexitrecVector = { finalexitrecVector.x, finalexitrecVector.y - 220 };
                finalmainmenutextvector = { finalmainmenutextvector.x, finalmainmenutextvector.y + 150 };
                finalgridlinespos.y = -450;
                finalmainmenuVector.x += 150;
                finaldrawturnVector.x -= 350;
                initialposition.y -= 450;
                finalposition.y -= 450;
                winint = 0;
            }
            else if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && mouseonmainmenubutton) {
                soundnum = 5;
                lastwingreen = true;
                ingame = false;
                finalstartrecVector = { finalstartrecVector.x, finalstartrecVector.y - 260 };
                finalresumerecVector = { finalresumerecVector.x, finalresumerecVector.y - 260 };
                finalaboutgamerecVector = { finalaboutgamerecVector.x, finalaboutgamerecVector.y - 240 };
                finalexitrecVector = { finalexitrecVector.x, finalexitrecVector.y - 220 };
                finalmainmenutextvector = { finalmainmenutextvector.x, finalmainmenutextvector.y + 150 };
                finalgridlinespos.y = -450;
                finalmainmenuVector.x += 150;
                finaldrawturnVector.x -= 350;
                initialposition.y -= 450;
                finalposition.y -= 450;
                winint = 0;
            }
        }
        else if (winint == 0) {
            if (inabout) {
                blackscreen = true;
            }
            else {
                blackscreen = false;
            }
            finalmainmenu2Vector.y = 210 + 450;
            finalwinscreenVector.y = 450;
            DrawTextureV(mainmenu, mainmenu2Vector, mouseonmainmenubutton2 ? WHITE : GRAY);
            if (lastwingreen) { 
                DrawTextureV(redwins, winscreenVector, WHITE);
                DrawTextureV(greenwins, winscreenVector, WHITE);
            }
            else {
                DrawTextureV(greenwins, winscreenVector, WHITE);
                DrawTextureV(redwins, winscreenVector, WHITE);
            }
        }

        EndDrawing();
    }
    CloseWindow();  
    return 0;
}


//i made this function so ican play a sound only once
void soundsFunc(Sound placebeadsfx, Sound takebeadsfx, Sound winsfx, Sound whoosh1sfx, Sound whoosh2sfx, int& soundnum)   {
    
    SetSoundVolume(placebeadsfx, 0.5);
    SetSoundVolume(takebeadsfx, 0.5);
    SetSoundVolume(winsfx, 0.5);
    SetSoundVolume(whoosh1sfx, 0.6);
    SetSoundVolume(whoosh2sfx, 1.3);

    if (soundnum == 1) {
        PlaySound(placebeadsfx);
        soundnum = 0;
    }
    else if (soundnum == 2) {
        PlaySound(takebeadsfx);
        soundnum = 0;
    }
    else if (soundnum == 3) {
        if(!IsSoundPlaying(winsfx))
            PlaySound(winsfx);
        soundnum = 0;
    }
    else if (soundnum == 4) {
        if (!IsSoundPlaying(whoosh1sfx))
            PlaySound(whoosh1sfx);
        soundnum = 0;
    }
    else if (soundnum == 5) {
        if (!IsSoundPlaying(whoosh2sfx))
            PlaySound(whoosh2sfx);
        soundnum = 0;
    }
}