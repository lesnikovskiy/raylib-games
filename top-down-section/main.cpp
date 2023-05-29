#include "raylib.h"
#include "raymath.h"
#include "Character.h"
#include "Enemy.h"
#include "Prop.h"

int main()
{
    const int windowWidth{384};
    const int windowHeight{384};

    InitWindow(windowWidth, windowHeight, "Open World Game");

    Texture2D map = LoadTexture("nature_tileset/OpenWorldMap24x24.png");
    Vector2 mapPos{0.0, 0.0};
    const float mapScale{4.0f};

    Character knight{windowWidth, windowHeight};

    Enemy goblin{
        Vector2{},
        LoadTexture("characters/goblin_idle_spritesheet.png"),
        LoadTexture("characters/goblin_run_spritesheet.png")};
    goblin.setTarget(&knight);

    Prop props[2]{
        Prop{Vector2{600.f, 300.f}, LoadTexture("nature_tileset/Rock.png")},
        Prop{Vector2{400.f, 500.f}, LoadTexture("nature_tileset/Log.png")},
    };

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(WHITE);

        mapPos = Vector2Scale(knight.getWorldPos(), -1.f);

        // draw the map
        DrawTextureEx(map, mapPos, 0.0, mapScale, WHITE);

        //  draw the props
        for (auto prop : props)
        {
            prop.Render(knight.getWorldPos());
        }

        knight.tick(GetFrameTime());

        // check map bounce
        if (knight.getWorldPos().x < 0.f ||
            knight.getWorldPos().y < 0.f ||
            knight.getWorldPos().x + windowWidth > map.width * mapScale ||
            knight.getWorldPos().y + windowHeight > map.height * mapScale)
        {
            knight.undoMovement();
        }

        // check prop collisions
        for (auto prop : props)
        {
            if (CheckCollisionRecs(prop.getCollisionRect(knight.getWorldPos()), knight.getCollisionRect()))
            {
                knight.undoMovement();
            }
        }

        goblin.tick(GetFrameTime());

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            if(CheckCollisionRecs(goblin.getCollisionRect(), knight.getWeaponCollisionRect()))
            {
                goblin.setAlive(false);
            }
        }

        EndDrawing();
    }

    UnloadTexture(map);
    // UnloadTexture(knight_idle);
    // UnloadTexture(knight_run);

    CloseWindow();

    return 0;
}