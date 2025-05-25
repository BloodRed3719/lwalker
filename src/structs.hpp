#ifndef STRUCTS_HPP
#define STRUCTS_HPP

#include <cmath>

struct vec {
    float x;
    float y;

    vec operator-(const vec& other) const {
        return {x - other.x, y - other.y};
    }

    float length() const {
        return std::sqrt(x * x + y * y);
    }

    vec normalized() const {
        float len = length();
        return len != 0 ? vec{x / len, y / len} : vec{0, 0};
    }
};

#endif // STRUCTS_HPP
