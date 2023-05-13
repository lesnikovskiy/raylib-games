#include "raylib.h"

int main()
{
    const int windowWidth{512};
    const int windowHeight{380};

    InitWindow(windowWidth, windowHeight, "Dapper Dasher");

    bool isInAir{};
    // jump velocity (pixels/second)
    const int jumpVelocity{-600};
    // acceleration due to gravity (pixel/s)/s
    const int gravity{1'000};

    // nebular variables
    Texture2D nebularTexture = LoadTexture("textures/12_nebula_spritesheet.png");
    Rectangle nebularRectangle{0.0, 0.0, nebularTexture.width / 8, nebularTexture.height / 8};
    Vector2 nebularPos{windowWidth, windowHeight - nebularRectangle.height};

    Rectangle nebular2Rectangle{0.0, 0.0, nebularTexture.width / 8, nebularTexture.height / 8};
    Vector2 nebular2Pos{windowWidth + 300, windowHeight - nebularRectangle.height};

    // nebular animation variables
    int nebFrame{};
    const float nebUpdateTime{1.0 / 12.0};
    float nebRunningTime{};

    int neb2Frame{};
    const float neb2UpdateTime{1.0 / 16.0};
    float neb2RunningTime{};

    int nebularVelocity{-200};

    // scarfy variables
    Texture2D scarfyTexture = LoadTexture("textures/scarfy.png");
    Rectangle scarfyRectangle;
    scarfyRectangle.width = scarfyTexture.width / 6;
    scarfyRectangle.height = scarfyTexture.height;
    scarfyRectangle.x = 0;
    scarfyRectangle.y = 0;
    Vector2 scarfyPos;
    scarfyPos.x = windowWidth / 2 - scarfyRectangle.width / 2;
    scarfyPos.y = windowHeight - scarfyRectangle.height;

    // animation frame
    int frame{};
    // amount of time before we update the animation frame (12 times/second)
    float updateTime{1.0 / 12.0};
    float runningTime{};

    int velocity{};

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        // Delta time (time since last frame)
        const float dT{GetFrameTime()};

        BeginDrawing();

        ClearBackground(WHITE);

        // perform ground check
        if (scarfyPos.y >= windowHeight - scarfyRectangle.height)
        {
            // rectangle is on the ground
            isInAir = false;
            velocity = 0;
        }
        else
        {
            // rectangle is in the air
            isInAir = true;
            // apply gravity
            velocity += gravity * dT;
        }

        // Jump check
        if (!isInAir && IsKeyPressed(KEY_SPACE))
        {
            velocity += jumpVelocity;
        }

        // update nebular position
        nebularPos.x += nebularVelocity * dT;
        nebular2Pos.x += nebularVelocity * dT;
        // update scarfy position
        scarfyPos.y += velocity * dT;

        // update nebular animation frame
        nebRunningTime += dT;
        if (nebRunningTime >= nebUpdateTime)
        {
            nebRunningTime = 0.0;

            nebularRectangle.x = nebFrame * nebularRectangle.width;
            nebFrame++;
            if (nebFrame > 7)
            {
                nebFrame = 0;
            }
        }

        neb2RunningTime += dT;
        if (neb2RunningTime >= neb2UpdateTime)
        {
            neb2RunningTime = 0.0;

            nebular2Rectangle.x = neb2Frame * nebular2Rectangle.width;
            neb2Frame++;
            if (neb2Frame > 7)
            {
                neb2Frame = 0;
            }
        }

        // update scarfy's animation frame
        if (!isInAir)
        {
            runningTime += dT;
            if (runningTime >= updateTime)
            {
                runningTime = 0.0;

                // Update animation frame
                scarfyRectangle.x = frame * scarfyRectangle.width;
                frame++;
                if (frame > 5)
                {
                    frame = 0;
                }
            }
        }

        // draw nebular
        DrawTextureRec(nebularTexture, nebularRectangle, nebularPos, WHITE);
        DrawTextureRec(nebularTexture, nebular2Rectangle, nebular2Pos, RED);
        // draw scarfy
        DrawTextureRec(scarfyTexture, scarfyRectangle, scarfyPos, WHITE);

        EndDrawing();
    }

    UnloadTexture(nebularTexture);
    UnloadTexture(scarfyTexture);
    CloseWindow();

    return 0;
}