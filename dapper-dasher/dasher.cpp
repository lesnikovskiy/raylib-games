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

    AnimData nebulae[2]{
        {
            {0.0, 0.0, nebularTexture.width / 8, nebularTexture.height / 8},        // Rectangle rec
            {windowDimensions[0], windowDimensions[1] - nebularTexture.height / 8}, // Vector2 pos
            0,                                                                      // int frame
            1.0 / 12.0,                                                             // float updateTime
            0                                                                       // float runtime
        },
        {
            {0.0, 0.0, nebularTexture.width / 8, nebularTexture.height / 8},              // Rectangle rec
            {windowDimensions[0] + 300, windowDimensions[1] - nebularTexture.height / 8}, // Vector2 pos
            0,                                                                            // int frame
            1.0 / 16.0,                                                                   // float updateTime
            0                                                                             // float runtime
        }};

    int nebularVelocity{-200};

    // scarfy variables
    Texture2D scarfyTexture = LoadTexture("textures/scarfy.png");
    AnimData scarfyData = {
        {0.0, 0.0, scarfyTexture.width / 6, scarfyTexture.height},                                           // Rectangle rect
        {windowDimensions[0] / 2 - scarfyData.rect.width / 2, windowDimensions[1] - scarfyData.rect.height}, // Vector2 pos
        0,                                                                                                   // int frame
        1.0 / 12.0,                                                                                          // int updateTime
        0                                                                                                    // int runningTime
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
        nebulae[0].pos.x += nebularVelocity * dT;
        nebulae[1].pos.x += nebularVelocity * dT;

        // update scarfy position
        scarfyData.pos.y += velocity * dT;

        // update nebular animation frame

        nebulae[0].runningTime += dT;
        if (nebulae[0].runningTime >= nebulae[0].updateTime)
        {
            nebulae[0].runningTime = 0.0;

            nebulae[0].rect.x = nebulae[0].frame * nebulae[0].rect.width;
            nebulae[0].frame++;
            if (nebulae[0].frame > 7)
            {
                nebulae[0].frame = 0;
            }
        }

        nebulae[1].runningTime += dT;
        if (nebulae[1].runningTime >= nebulae[1].updateTime)
        {
            nebulae[1].runningTime = 0.0;

            nebulae[1].rect.x = nebulae[1].frame * nebulae[1].rect.width;
            nebulae[1].frame++;
            if (nebulae[1].frame > 7)
            {
                nebulae[1].frame = 0;
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
        DrawTextureRec(nebularTexture, nebulae[0].rect, nebulae[0].pos, WHITE);
        DrawTextureRec(nebularTexture, nebulae[1].rect, nebulae[1].pos, RED);
        // draw scarfy
        DrawTextureRec(scarfyTexture, scarfyData.rect, scarfyData.pos, WHITE);

        EndDrawing();
    }

    UnloadTexture(nebularTexture);
    UnloadTexture(scarfyTexture);
    CloseWindow();

    return 0;
}