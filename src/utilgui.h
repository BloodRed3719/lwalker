#pragma once

#include <iostream>
#include <raylib.h>

using namespace std;

class Button {
public:
	Button();
	~Button();
	void Draw();
	bool CheckClick(float mousePosX, float mousePosY);
	float x;
	float y;
	float w;
	float h;
	string text;
	float xpad;
	float ypad;
	Color color;
	int textSize;
	Color textColor;
};