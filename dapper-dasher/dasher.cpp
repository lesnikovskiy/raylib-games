#include "raylib.h"

struct AnimData
{
    Rectangle rect;
    Vector2 pos;
    int frame;
    float updateTime;
    float runningTime;
    Color color;
};

bool isOnGround(AnimData data, int windowHeight)
{
    return data.pos.y >= windowHeight - data.rect.height;
}

AnimData updateAnimData(AnimData data, float deltaTime, int maxFrame)
{
    data.runningTime += deltaTime;

    if (data.runningTime >= data.updateTime)
    {
        data.runningTime = 0.0;

        // Update animation frame
        data.rect.x = data.frame * data.rect.width;
        data.frame++;
        if (data.frame > maxFrame)
        {
            data.frame = 0;
        }
    }

    return data;
}

int main()
{
    int windowDimensions[2]{512, 380};

    InitWindow(windowDimensions[0], windowDimensions[1], "Dapper Dasher");

    bool isInAir{};
    // jump velocity (pixels/second)
    const int jumpVelocity{-600};
    // acceleration due to gravity (pixel/s)/s
    const int gravity{1'000};

    // nebular variables
    Texture2D nebularTexture = LoadTexture("textures/12_nebula_spritesheet.png");

    const int sizeOfNebulae{1000};

    AnimData nebulae[sizeOfNebulae]{};

    for (int i = 0; i < sizeOfNebulae; i++)
    {
        nebulae[i].rect.x = 0.0;
        nebulae[i].rect.y = 0.0;
        nebulae[i].rect.width = nebularTexture.width / 8;
        nebulae[i].rect.height = nebularTexture.height / 8;
        nebulae[i].pos.x = windowDimensions[0] + i * 300;
        nebulae[i].pos.y = windowDimensions[1] - nebularTexture.height / 8;
        nebulae[i].frame = 0;
        nebulae[i].runningTime = 0.0;
        nebulae[i].updateTime = 1.0 / 16.0;
        nebulae[i].color = i % 2 == 0 ? WHITE : RED;
    }

    int nebularVelocity{-200};

    // scarfy variables
    Texture2D scarfyTexture = LoadTexture("textures/scarfy.png");
    AnimData scarfyData = {
        {0.0, 0.0, scarfyTexture.width / 6, scarfyTexture.height},                                           // Rectangle rect
        {windowDimensions[0] / 2 - scarfyData.rect.width / 2, windowDimensions[1] - scarfyData.rect.height}, // Vector2 pos
        0,                                                                                                   // int frame
        1.0 / 12.0,                                                                                          // int updateTime
        0,                                                                                                   // int runningTime
        WHITE                                                                                                // Color color
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
        if (isOnGround(scarfyData, windowDimensions[1]))
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
        for (int i = 0; i < sizeOfNebulae; i++)
        {
            nebulae[i].pos.x += nebularVelocity * dT;
        }

        // update scarfy position
        scarfyData.pos.y += velocity * dT;

        // update nebular animation frame
        for (int i = 0; i < sizeOfNebulae; i++)
        {
            nebulae[i] = updateAnimData(nebulae[i], dT, 7);
        }

        // update scarfy's animation frame
        if (!isInAir)
        {
            scarfyData = updateAnimData(scarfyData, dT, 5);
        }

        // draw nebular
        for (int i = 0; i < sizeOfNebulae; i++)
        {
            DrawTextureRec(nebularTexture, nebulae[i].rect, nebulae[i].pos, nebulae[i].color);
        }

        // draw scarfy
        DrawTextureRec(scarfyTexture, scarfyData.rect, scarfyData.pos, scarfyData.color);

        EndDrawing();
    }

    UnloadTexture(nebularTexture);
    UnloadTexture(scarfyTexture);
    CloseWindow();

    return 0;
}