#include "utilgui.h"
#include <iostream>
#include <raylib.h>

using namespace std;

Button::Button() {}

Button::~Button() {}

void Button::Draw() {
	DrawRectangleRounded((Rectangle){x, y, w, h}, 0.2, 0, color);
	DrawText(text.c_str(), x + xpad, y + ypad, textSize, textColor);
}

bool Button::CheckClick(float mousePosX, float mousePosY) {
    if (mousePosX > x && mousePosX < x + w &&
        mousePosY > y && mousePosY < y + h &&
        IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        return true;
    }
    return false;
}
