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

    const int sizeOfNebulae{8};

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

    float finishLine{nebulae[sizeOfNebulae - 1].pos.x};

    // nebular X velocity (pixel/second)
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

    Texture2D background = LoadTexture("textures/far-buildings.png");
    float bgX{};

    Texture2D midground = LoadTexture("textures/back-buildings.png");
    float mgX{};

    Texture2D foreground = LoadTexture("textures/foreground.png");
    float fgX{};

    float collision{false};

    InitAudioDevice();
    Sound jumpSound = LoadSound("resources/jump.wav");
    Music mainTheme = LoadMusicStream("resources/mario_theme.mp3");
    Music loseGame = LoadMusicStream("resources/lose_game.mp3");
    Music winGame = LoadMusicStream("resources/win_game.mp3");

    PlayMusicStream(mainTheme);

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        UpdateMusicStream(mainTheme);

        if (IsKeyDown(KEY_SPACE))
        {
            PlaySound(jumpSound);
        }

        // Delta time (timeff since last frame)
        const float dT{GetFrameTime()};

        BeginDrawing();

        ClearBackground(WHITE);

        // calc background pos
        bgX -= 20 * dT;
        if (bgX <= -background.width * 2)
        {
            bgX = 0.0;
        }

        mgX -= 40 * dT;
        if (mgX <= -midground.width * 2)
        {
            mgX = 0.0;
        }

        fgX -= 80 * dT;
        if (fgX <= -foreground.width * 2)
        {
            fgX = 0.0;
        }

        // draw the background
        Vector2 bg1Pos{bgX, 0.0};
        DrawTextureEx(background, bg1Pos, 0.0, 2.0, WHITE);
        Vector2 bg2Pos{bgX + background.width * 2, 0.0};
        DrawTextureEx(background, bg2Pos, 0.0, 2.0, WHITE);

        // draw the middleground
        Vector2 mg1Pos{mgX, 0.0};
        DrawTextureEx(midground, mg1Pos, 0.0, 2.0, WHITE);
        Vector2 mg2Pos{mgX + midground.width * 2, 0.0};
        DrawTextureEx(midground, mg2Pos, 0.0, 2.0, WHITE);

        // Draw the foreground
        Vector2 fg1Pos{fgX, 0.0};
        DrawTextureEx(foreground, fg1Pos, 0.0, 2.0, WHITE);
        Vector2 fg2Pos{fgX + foreground.width * 2, 0.0};
        DrawTextureEx(foreground, fg2Pos, 0.0, 2.0, WHITE);

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

        // Update finish line
        finishLine += nebularVelocity * dT;

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

        for (AnimData nebular : nebulae)
        {
            float pad{50};

            Rectangle nebRec{
                nebular.pos.x + pad,
                nebular.pos.y + pad,
                nebular.rect.width - 2 * pad,
                nebular.rect.height - 2 * pad};

            Rectangle scarfyRec{
                scarfyData.pos.x,
                scarfyData.pos.y,
                scarfyData.rect.width,
                scarfyData.rect.height};

            if (CheckCollisionRecs(nebRec, scarfyRec))
            {
                collision = true;
            }
        }

        if (collision)
        {
            // lose the game
            DrawText("Game Over!", windowDimensions[0] / 4, windowDimensions[1] / 2, 50, RED);
            StopMusicStream(mainTheme);
            PlayMusicStream(loseGame);
            UpdateMusicStream(loseGame);
        }
        else if (scarfyData.pos.x >= finishLine)
        {
            // win game
            DrawText("You win!", windowDimensions[0] / 4, windowDimensions[1] / 2, 60, GREEN);
            StopMusicStream(mainTheme);
            PlayMusicStream(winGame);
            UpdateMusicStream(winGame);
        }
        else
        {
            // draw nebular
            for (int i = 0; i < sizeOfNebulae; i++)
            {
                DrawTextureRec(nebularTexture, nebulae[i].rect, nebulae[i].pos, nebulae[i].color);
            }

            // draw scarfy
            DrawTextureRec(scarfyTexture, scarfyData.rect, scarfyData.pos, scarfyData.color);
        }

        EndDrawing();
    }

    UnloadSound(jumpSound);
    UnloadMusicStream(mainTheme);
    UnloadMusicStream(loseGame);
    UnloadMusicStream(winGame);

    UnloadTexture(background);
    UnloadTexture(midground);
    UnloadTexture(foreground);
    UnloadTexture(nebularTexture);
    UnloadTexture(scarfyTexture);
    CloseWindow();

    return 0;
}