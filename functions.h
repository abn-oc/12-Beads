#include"raylib.h"

void resetgrid(int[5][5]);
void save(bool, int[5][5]);
void load(bool&, int[5][5]);
void DrawBeads(int[5][5], Texture2D, Texture2D, Texture2D, Texture2D, bool&);
void MouseClick(int[5][5], bool&, Vector2&, Vector2&, Vector2&, float&, int&);
void DrawTurn(bool&, Texture2D, Texture2D, Vector2);
int Check_Win(int[5][5], bool&);
void randomizeturn(bool&);
Vector2 lerp(Vector2, Vector2, float);