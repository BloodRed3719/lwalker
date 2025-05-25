#pragma once
#include "structs.hpp"
#include <raylib.h>

class Enemy {
public:
    Enemy();
    ~Enemy();
    void Draw();
    void Update(vec targetPosition);
    float x;
    float y;
    float w;
    float h;
    float roundness;
    float speed;
    Rectangle rect;
};

bool operator==(const Enemy& a, const Enemy& b);
