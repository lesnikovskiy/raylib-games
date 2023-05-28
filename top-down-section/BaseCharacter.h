#ifndef BASE_CHARACTER
#define BASE_CHARACTER

#include "raylib.h"
#include "raymath.h"

class BaseCharacter
{
public:
    BaseCharacter();
    Vector2 getWorldPos() { return worldPos; }
    Rectangle getCollisionRect();
    void undoMovement();
    virtual void tick(float deltaTime);
    virtual Vector2 getScreenPos() = 0;

protected:
    Texture2D texture{LoadTexture("characters/knight_idle_spritesheet.png")};
    Texture2D idle{LoadTexture("characters/knight_idle_spritesheet.png")};
    Texture2D run{LoadTexture("characters/knight_run_spritesheet.png")};
    Vector2 worldPos{};
    Vector2 worldPosLastFrame{};
    Vector2 velocity{};
    // 1 : facing right, -1 : facing left
    float rightLeft{1.f};

    // animation variables
    float runningTime{};
    int frame{};
    int maxFrames{6};
    float updateTime{1.f / 12.f};
    float speed{4.f};

    float width{};
    float height{};
    float scale{4.0f};
};

#endif