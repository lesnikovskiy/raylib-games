#include "raylib.h"

struct AnimData
{
    Rectangle rect;
    Vector2 pos;
    int frame;
    float updateTime;
    float runningTime;
};

int main()
{
    int windowDimensions[2] = {512, 380};

    InitWindow(windowDimensions[0], windowDimensions[1], "Dapper Dasher");

    bool isInAir{};
    // jump velocity (pixels/second)
    const int jumpVelocity{-600};
    // acceleration due to gravity (pixel/s)/s
    const int gravity{1'000};

    // nebular variables
    Texture2D nebularTexture = LoadTexture("textures/12_nebula_spritesheet.png");

    // AnimData for nebula
    AnimData nebData{
        {0.0, 0.0, nebularTexture.width / 8, nebularTexture.height / 8}, // Rectangle rec
        {windowDimensions[0], windowDimensions[1] - nebularTexture.height / 8},         // Vector2 pos
        0,                                                               // int frame
        1.0 / 12.0,                                                      // float updateTime
        0                                                                // float runtime
    };

    AnimData neb2Data{
        {0.0, 0.0, nebularTexture.width / 8, nebularTexture.height / 8}, // Rectangle rec
        {windowDimensions[0] + 300, windowDimensions[1] - nebularTexture.height / 8},   // Vector2 pos
        0,                                                               // int frame
        1.0 / 16.0,                                                      // float updateTime
        0                                                                // float runtime
    };

    int nebularVelocity{-200};

    // scarfy variables
    Texture2D scarfyTexture = LoadTexture("textures/scarfy.png");
    AnimData scarfyData = {
        {0.0, 0.0, scarfyTexture.width / 6, scarfyTexture.height},                            // Rectangle rect
        {windowDimensions[0] / 2 - scarfyData.rect.width / 2, windowDimensions[1] - scarfyData.rect.height}, // Vector2 pos
        0,                                                                                    // int frame
        1.0 / 12.0,                                                                           // int updateTime
        0                                                                                     // int runningTime
    };

    int velocity{};

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        // Delta time (time since last frame)
        const float dT{GetFrameTime()};

        BeginDrawing();

        ClearBackground(WHITE);

        // perform ground check
        if (scarfyData.pos.y >= windowDimensions[1] - scarfyData.rect.height)
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
        nebData.pos.x += nebularVelocity * dT;
        neb2Data.pos.x += nebularVelocity * dT;

        // update scarfy position
        scarfyData.pos.y += velocity * dT;

        // update nebular animation frame
        nebData.runningTime += dT;
        if (nebData.runningTime >= nebData.updateTime)
        {
            nebData.runningTime = 0.0;

            nebData.rect.x = nebData.frame * nebData.rect.width;
            nebData.frame++;
            if (nebData.frame > 7)
            {
                nebData.frame = 0;
            }
        }

        neb2Data.runningTime += dT;
        if (neb2Data.runningTime >= neb2Data.updateTime)
        {
            neb2Data.runningTime = 0.0;

            neb2Data.rect.x = neb2Data.frame * neb2Data.rect.width;
            neb2Data.frame++;
            if (neb2Data.frame > 7)
            {
                neb2Data.frame = 0;
            }
        }

        // update scarfy's animation frame
        if (!isInAir)
        {
            scarfyData.runningTime += dT;
            if (scarfyData.runningTime >= scarfyData.updateTime)
            {
                scarfyData.runningTime = 0.0;

                // Update animation frame
                scarfyData.rect.x = scarfyData.frame * scarfyData.rect.width;
                scarfyData.frame++;
                if (scarfyData.frame > 5)
                {
                    scarfyData.frame = 0;
                }
            }
        }

        // draw nebular
        DrawTextureRec(nebularTexture, nebData.rect, nebData.pos, WHITE);
        DrawTextureRec(nebularTexture, neb2Data.rect, neb2Data.pos, RED);
        // draw scarfy
        DrawTextureRec(scarfyTexture, scarfyData.rect, scarfyData.pos, WHITE);

        EndDrawing();
    }

    UnloadTexture(nebularTexture);
    UnloadTexture(scarfyTexture);
    CloseWindow();

    return 0;
}