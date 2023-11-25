#include <raylib.h>
#include <raymath.h>
#include <stdio.h>

#include <math.h>

#define MAP_SIZE 24

int worldMap[MAP_SIZE][MAP_SIZE] =
{
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,2,2,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,3,0,0,0,3,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,2,0,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,0,0,0,5,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};


typedef struct Cord_S {
    int x, y;
} Cord;


int main(void)
{

    const int screenWidth  = 640;
    const int screenHeight = 480;
    const int fps = 60;

    // Initialization

    Vector2 pos   = { 22, 12    }; // vector represent the position of player
    Vector2 dir   = { -1, 0     }; // direction vector
    Vector2 plane = {  0, 0.66f }; // vector represent camera plane (which is just a 2d line)

    InitWindow(screenWidth, screenHeight, "vanilla ray casting demo");
    SetTargetFPS(fps);

    while(!WindowShouldClose()) {

        double moveSpeed = 5.0 * (1.0/fps);
        double rotSpeed  = 3.0 * (1.0/fps);

        if (IsKeyDown(KEY_UP)) {
            if (!worldMap[(int)(pos.x + dir.x * moveSpeed)][(int)pos.y])
                pos.x += dir.x * moveSpeed;
            if (!worldMap[(int)pos.x][(int)(pos.y + dir.y * moveSpeed)])
                pos.y += dir.y * moveSpeed;
        }


        if (IsKeyDown(KEY_DOWN)) {
            if (!worldMap[(int)(pos.x - dir.x * moveSpeed)][(int)pos.y])
                pos.x -= dir.x * moveSpeed;
            if (!worldMap[(int)pos.x][(int)(pos.y - dir.y * moveSpeed)])
                pos.y -= dir.y * moveSpeed;
        }

        if (IsKeyDown(KEY_RIGHT)) {
            puts("ROTATING RIGHT");

            dir = Vector2Rotate(dir, -rotSpeed);
            plane = Vector2Rotate(plane, -rotSpeed);

        }

        if (IsKeyDown(KEY_LEFT)) {
            puts("ROTATING LEFT");

            dir = Vector2Rotate(dir, rotSpeed);
            plane = Vector2Rotate(plane, rotSpeed);
        }

        BeginDrawing();

        ClearBackground(BLACK);

        DrawFPS(0, 0);

        for (int x = 0; x < screenWidth; x++) {

            float cameraX = 2 * x / (double)screenWidth - 1;
            Vector2 rayDir =
            {
                dir.x + plane.x * cameraX,
                dir.y + plane.y * cameraX,
            };

            Cord gridPos = { (int)pos.x, (int)pos.y };

            double sideDistX;
            double sideDistY;

            // optimized formula to calculate the Euclidian distance to travel from 1 x-side to another x-side
            double deltaDistX = (rayDir.x == 0) ? 1e30 : fabs(1.0 / rayDir.x);

            // optimized formula to calculate the Euclidian distance to travel from 1 y-side to another y-side
            double deltaDistY = (rayDir.y == 0) ? 1e30 : fabs(1.0 / rayDir.y);
            double perpWallDist;

            Cord step;
            int hit = 0;
            int side; // x-side or y-side is hit ?

            if (rayDir.x < 0)
            {
                step.x = -1;
                sideDistX = (pos.x - gridPos.x) * deltaDistX;
            } else {
                step.x = 1;
                sideDistX = (gridPos.x + 1.0 - pos.x) * deltaDistX;
            }
            if (rayDir.y < 0) {
                step.y = -1;
                sideDistY = (pos.y - gridPos.y) * deltaDistY;
            } else {
                step.y = 1;
                sideDistY = (gridPos.y + 1.0 - pos.y) * deltaDistY;
            }

            while (!hit) {
                if (sideDistX < sideDistY) {
                    sideDistX += deltaDistX;
                    gridPos.x += step.x;
                    side = 0;
                } else {
                    sideDistY += deltaDistY;
                    gridPos.y += step.y;
                    side = 1;
                }

                if (worldMap[gridPos.x][gridPos.y])
                    hit = 1;
            }

            // magic formular to calculate wall distance
            if (side == 0)
                perpWallDist = sideDistX - deltaDistX;
            else
                perpWallDist = sideDistY - deltaDistY;


            // calculate height of lines
            int lineHeight = (int)(screenHeight / perpWallDist);

            int drawStart = screenHeight / 2 - lineHeight / 2;
            if (drawStart < 0)
                drawStart = 0;

            int drawEnd = screenHeight / 2 + lineHeight / 2;
            if (drawEnd > screenHeight)
                drawEnd = screenHeight;

            Color col;
            switch (worldMap[gridPos.x][gridPos.y]) {
                case 1:
                    col = RED;
                    break;
                case 2:
                    col = GREEN;
                    break;
                case 3:
                    col = BLUE;
                    break;
                case 4:
                    col = WHITE;
                    break;
                default:
                    col = YELLOW;
                    break;
            }

            DrawLine(x, drawStart, x, drawEnd, col);
        }

        EndDrawing();
    }
    CloseWindow();

    return 0;
}

