#include "Enemy.h"

Enemy::Enemy(Vector2 pos, Texture2D idleTexture, Texture2D runTexture)
{
    worldPos = pos;
    texture = idleTexture;
    idle = idleTexture;
    run = runTexture;

    width = texture.width / maxFrames;
    height = texture.height;
    speed = 3.5f;
}

void Enemy::tick(float deltaTime)
{

    // get toTarget
    Vector2 toTarget = Vector2Subtract(target->getScreenPos(), screenPos);
    // normalize toTarge
    toTarget = Vector2Normalize(toTarget);
    // multiply by speed
    toTarget = Vector2Scale(toTarget, speed);
    // move the enemy
    worldPos = Vector2Add(worldPos, toTarget);

    screenPos = Vector2Subtract(worldPos, target->getWorldPos());
    BaseCharacter::tick(deltaTime);
}
