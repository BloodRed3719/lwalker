#include "enemy.hpp"
#include <raylib.h>
#include <cmath>

Enemy::Enemy() {
    x = 0;
    y = 0;
    w = 50;
    h = 50;
    speed = 1.5f;
    roundness = 0.2f;
    rect = {x, y, w, h};
}

Enemy::~Enemy() {}

void Enemy::Draw() {
    DrawRectangleRounded({x, y, w, h}, roundness, 0, RED);
}

void Enemy::Update(vec targetPosition) {
    float dx = targetPosition.x - x;
    float dy = targetPosition.y - y;
    float distance = sqrt(dx * dx + dy * dy);

    if (distance > 0.0f) {
        struct vec normalized_direction;
        normalized_direction.x = dx / distance;
        normalized_direction.y = dy / distance;

        x += normalized_direction.x * speed;
        y += normalized_direction.y * speed;
        rect = {x, y, w, h};
    }
}

bool operator==(const Enemy& a, const Enemy& b) {
    return a.rect.x == b.rect.x &&
           a.rect.y == b.rect.y &&
           a.rect.width == b.rect.width &&
           a.rect.height == b.rect.height;
}
