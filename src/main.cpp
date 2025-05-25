#include <iostream>
#include <raylib.h>
#include <cmath>
#include <cstdlib>
#include <list>
#include "structs.hpp"
#include "enemy.hpp"
#include "utilgui.h"

using namespace std;

int main(void)
{
    const int width = 1080;
    const int height = 700;

    Button button = Button();
    button.x = 50;
    button.y = 50;
    button.w = 150;
    button.h = 50;
    button.text = "Click me!";
    button.xpad = button.x / 2;
    button.ypad = button.y / 2 - 5;
    button.color = (Color)RED;
    button.textColor = (Color)BLACK;
    button.textSize = 20;

    Button plusOneEnemies = Button();
    plusOneEnemies.x = width / 2 - 50;
    plusOneEnemies.y = 100;
    plusOneEnemies.w = 150;
    plusOneEnemies.h = 50;
    plusOneEnemies.color = (Color)RED;
    plusOneEnemies.textColor = (Color)BLACK;
    plusOneEnemies.textSize = 20;
    plusOneEnemies.text = "+1 enemy \nper round";
    plusOneEnemies.xpad = 20;
    plusOneEnemies.ypad = 10;

    Button plusTwoEnemies = Button();
    plusTwoEnemies.x = width / 2 - 50;
    plusTwoEnemies.y = 200;
    plusTwoEnemies.w = 150;
    plusTwoEnemies.h = 50;
    plusTwoEnemies.color = (Color)RED;
    plusTwoEnemies.textColor = (Color)BLACK;
    plusTwoEnemies.textSize = 20;
    plusTwoEnemies.text = "+2 enemies\nper round";
    plusTwoEnemies.xpad = 20;
    plusTwoEnemies.ypad = 10;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_HIGHDPI);
    InitWindow(width, height, "Loopwalker");
    Color dark_green = Color{20, 160, 133, 255};
    double initTime = GetTime();
    double timeAtDeath = 0.0;
    float x = 50;
    float y = height / 2;
    float w = 50;
    float h = 50;
    float mouseW = 25;
    float mouseH = 25;
    Rectangle player = {x, y, w, h};
    float speed = 3;
    int num_of_enemies_to_spawn = 10;
    list<Enemy> enemies;
    int money = 0;
    bool isInSkillTreeChoosingScreen = false;

    int plusOneUpgradeCost = 10;
    int plusTwoUpgradeCost = 20;

    bool hasPlusOneEnemiesUpgrade = false;
    bool hasPlusTwoEnemiesUpgrade = true;

    double attackCooldown = 2.0;
    double lastAttackTime = 0.0;
    double attackVisualStartTime = -10.0;
    bool isAttacking = false;

    SetTargetFPS(60);

    for (int i = 0; i < num_of_enemies_to_spawn; i++) {
        Enemy newEnemy = Enemy();
        newEnemy.x = GetRandomValue(150, width);
        newEnemy.y = GetRandomValue(0, height);
        newEnemy.w = 50;
        newEnemy.h = 50;
        newEnemy.rect = {newEnemy.x, newEnemy.y, newEnemy.w, newEnemy.h};
        enemies.push_back(newEnemy);
    }

    while (!WindowShouldClose()) {
        player = {x, y, w, h};
        vec mousePos;
        mousePos.x = GetMousePosition().x;
        mousePos.y = GetMousePosition().y;
        double currentTime = GetTime();
        double timeElapsed = currentTime - initTime;
        string strTimeElapsed = std::to_string((int)round(timeElapsed));
        const char* charTimeElapsed = strTimeElapsed.c_str();
        double timeAllowed = 60.0;
        bool shouldUpdateTime = true;
        const vec player_pos = {x, y};

        if (timeElapsed >= timeAllowed or enemies.size() == 0) {
            shouldUpdateTime = false;
            timeAtDeath = GetTime() - initTime;
        }

        for (auto& enemy : enemies) {
            if (CheckCollisionRecs(player, enemy.rect)) {
                shouldUpdateTime = false;
                enemies.clear();
                x = 50;
                y = height / 2;
                break;
            }
        }

        BeginDrawing();
        ClearBackground(DARKGRAY);

        string moneyText = "$" + to_string(money);
        DrawText(moneyText.c_str(), 25, 25, 20, BLACK);

        if (shouldUpdateTime) {
            DrawRectangleRounded({x, y, w, h}, 0.2f, 0, BLUE);
            Rectangle mouseRect = {mousePos.x - (mouseW / 2), mousePos.y - (mouseH / 2), mouseW, mouseH};

            if (GetTime() - lastAttackTime >= attackCooldown) {
                lastAttackTime = GetTime();
                attackVisualStartTime = GetTime();
                isAttacking = true;
            }

            if (isAttacking) {
                if (GetTime() - attackVisualStartTime < 1.0) {
                    DrawRectangleRounded({mousePos.x - (int)12.5, mousePos.y - (int)12.5, 25, 25}, 1, 0, GREEN);
                    for (auto it = enemies.begin(); it != enemies.end();) {
                if (CheckCollisionRecs(mouseRect, it->rect)) {
                    it = enemies.erase(it);
                    money += 1;
                } else {
                    ++it;
                }
            }
                } else {
                    isAttacking = false;
                }
            }

            vec player_pos = {x, y};
            for (auto& enemy : enemies) {
                enemy.Draw();
                enemy.Update(player_pos);
            }

            if (IsKeyDown(KEY_W)) y -= speed;
            if (IsKeyDown(KEY_A)) x -= speed;
            if (IsKeyDown(KEY_S)) y += speed;
            if (IsKeyDown(KEY_D)) x += speed;

            DrawText(charTimeElapsed, width / 2, 50, 30, RED);
        } else if (!shouldUpdateTime and !isInSkillTreeChoosingScreen) {
            DrawText("Press B to go back to the game", width - 350, height - 50, 20, BLACK);
            button.Draw();
            button.text = "Skill tree";
            if (button.CheckClick(mousePos.x, mousePos.y)) {
                isInSkillTreeChoosingScreen = true;
            }
            if (IsKeyDown(KEY_B)) {
                for (int i = 0; i < num_of_enemies_to_spawn; i++) {
                    Enemy newEnemy = Enemy();
                    newEnemy.x = GetRandomValue(150, width);
                    newEnemy.y = GetRandomValue(0, height);
                    newEnemy.w = 50;
                    newEnemy.h = 50;
                    newEnemy.rect = {newEnemy.x, newEnemy.y, newEnemy.w, newEnemy.h};
                    enemies.push_back(newEnemy);
                }
                x = 50;
                y = height / 2;
                shouldUpdateTime = true;
                isInSkillTreeChoosingScreen = false;
            }
        } else if (!shouldUpdateTime and isInSkillTreeChoosingScreen) {
            DrawText("Press B to go back to the game", width - 350, height - 50, 20, BLACK);
            if (!hasPlusOneEnemiesUpgrade) {
                plusOneEnemies.Draw();
            }
            if (plusOneEnemies.CheckClick(mousePos.x, mousePos.y) and money >= plusOneUpgradeCost) {
                money -= plusOneUpgradeCost;
                hasPlusOneEnemiesUpgrade = true;
                hasPlusTwoEnemiesUpgrade = false;
                num_of_enemies_to_spawn += 1;
                for (int i = 0; i < num_of_enemies_to_spawn; i++) {
                    Enemy newEnemy = Enemy();
                    newEnemy.x = GetRandomValue(150, width);
                    newEnemy.y = GetRandomValue(0, height);
                    newEnemy.w = 50;
                    newEnemy.h = 50;
                    newEnemy.rect = {newEnemy.x, newEnemy.y, newEnemy.w, newEnemy.h};
                    enemies.push_back(newEnemy);
                }
                isInSkillTreeChoosingScreen = false;
                shouldUpdateTime = true;
                x = 50;
                y = height / 2;
            }
            if (IsKeyDown(KEY_B)) {
                for (int i = 0; i < num_of_enemies_to_spawn; i++) {
                    Enemy newEnemy = Enemy();
                    newEnemy.x = GetRandomValue(150, width);
                    newEnemy.y = GetRandomValue(0, height);
                    newEnemy.w = 50;
                    newEnemy.h = 50;
                    newEnemy.rect = {newEnemy.x, newEnemy.y, newEnemy.w, newEnemy.h};
                    enemies.push_back(newEnemy);
                }
                x = 50;
                y = height / 2;
                shouldUpdateTime = true;
                isInSkillTreeChoosingScreen = false;
            }

            if (!hasPlusTwoEnemiesUpgrade) {
                plusTwoEnemies.Draw();
            }

            if (plusTwoEnemies.CheckClick(mousePos.x, mousePos.y) and money >= plusTwoUpgradeCost) {
                money -= plusTwoUpgradeCost;
                hasPlusTwoEnemiesUpgrade = true;
                num_of_enemies_to_spawn += 2;
                isInSkillTreeChoosingScreen = false;
                shouldUpdateTime = false;
                x = 50;
                y = height / 2;
                for (int i = 0; i < num_of_enemies_to_spawn; i++) {
                    Enemy newEnemy = Enemy();
                    newEnemy.x = GetRandomValue(150, width);
                    newEnemy.y = GetRandomValue(0, height);
                    newEnemy.w = 50;
                    newEnemy.h = 50;
                    newEnemy.rect = {newEnemy.x, newEnemy.y, newEnemy.w, newEnemy.h};
                    enemies.push_back(newEnemy);
                }
            }
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}