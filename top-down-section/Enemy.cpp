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
    if (!getAlive())
        return;

    // get toTarget
    velocity = Vector2Subtract(target->getScreenPos(), getScreenPos());
    BaseCharacter::tick(deltaTime);

    if (CheckCollisionRecs(target->getCollisionRect(), getCollisionRect()))
    {
        target->takeDamage(damagePerSec * deltaTime);
    }
}

Vector2 Enemy::getScreenPos()
{
    return Vector2Subtract(worldPos, target->getWorldPos());
}
