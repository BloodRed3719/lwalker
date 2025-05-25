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
    button.text = "Enemy Amount\nSkill Tree";
    button.xpad = 5;
    button.ypad = 5;
    button.color = (Color)RED;
    button.textColor = (Color)BLACK;
    button.textSize = 20;

    Button attackSpeedButton = Button();
    attackSpeedButton.x = 250;
    attackSpeedButton.y = 50;
    attackSpeedButton.w = 150;
    attackSpeedButton.h = 50;
    attackSpeedButton.text = "Attack Speed\nSkill Tree";
    attackSpeedButton.xpad = 5;
    attackSpeedButton.ypad = 5;
    attackSpeedButton.color = (Color)RED;
    attackSpeedButton.textColor = (Color)BLACK;
    attackSpeedButton.textSize = 20;

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

    Button plusQuarterSecondAttackSpeed = Button();
    plusQuarterSecondAttackSpeed.x = width / 2 - 50;
    plusQuarterSecondAttackSpeed.y = 100;
    plusQuarterSecondAttackSpeed.w = 150;
    plusQuarterSecondAttackSpeed.h = 50;
    plusQuarterSecondAttackSpeed.color = (Color)RED;
    plusQuarterSecondAttackSpeed.textColor = (Color)BLACK;
    plusQuarterSecondAttackSpeed.textSize = 20;
    plusQuarterSecondAttackSpeed.text = "-0.25 second attack speed";
    plusQuarterSecondAttackSpeed.xpad = 20;
    plusQuarterSecondAttackSpeed.ypad = 10;

    Button plusHalfSecondAttackSpeed = Button();
    plusHalfSecondAttackSpeed.x = width / 2 - 50;
    plusHalfSecondAttackSpeed.y = 200;
    plusHalfSecondAttackSpeed.w = 150;
    plusHalfSecondAttackSpeed.h = 50;
    plusHalfSecondAttackSpeed.color = (Color)RED;
    plusHalfSecondAttackSpeed.textColor = (Color)BLACK;
    plusHalfSecondAttackSpeed.textSize = 20;
    plusHalfSecondAttackSpeed.text = "-0.5 second attack speed";
    plusHalfSecondAttackSpeed.xpad = 20;
    plusHalfSecondAttackSpeed.ypad = 10;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_HIGHDPI);
    InitWindow(width, height, "Loopwalker");
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
    int money = 200;
    bool isInSkillTreeChoosingScreen = false;
    bool isInAttackSpeedSkillTreeChoosingScreen = false;
    float attackSpeed = 1;

    int plusOneUpgradeCost = 10;
    int plusTwoUpgradeCost = 20;
    int plusQuarterSecondAttackSpeedCost = 50;
    int plusHalfSecondAttackSpeedCost = 150;

    bool hasPlusOneEnemiesUpgrade = false;
    bool hasPlusTwoEnemiesUpgrade = true;
    bool hasPlusFiveEnemiesUpgrade = true;
    bool hasPlusQuarterSecondAttackSpeed = false;
    bool hasPlusHalfSecondAttackSpeed = false;

    double attackCooldown = 1.0;
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

            if (!isAttacking && GetTime() - lastAttackTime >= attackCooldown) {
                isAttacking = true;
                attackVisualStartTime = GetTime();
            }

            if (isAttacking) {
                if (GetTime() - attackVisualStartTime < attackSpeed) {
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
                    lastAttackTime = GetTime();  // Reset cooldown timer here, when attack ends
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
        } else if (!shouldUpdateTime and !isInSkillTreeChoosingScreen and !isInAttackSpeedSkillTreeChoosingScreen) {
            DrawText("Press B to go back to the game", width - 350, height - 50, 20, BLACK);
            button.Draw();
            attackSpeedButton.Draw();
            if (button.CheckClick(mousePos.x, mousePos.y)) {
                isInSkillTreeChoosingScreen = true;
            }

            if (attackSpeedButton.CheckClick(mousePos.x, mousePos.y)) {
                isInAttackSpeedSkillTreeChoosingScreen = true;
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
                isInAttackSpeedSkillTreeChoosingScreen = false;
            }
        } else if (!shouldUpdateTime and isInSkillTreeChoosingScreen and !isInAttackSpeedSkillTreeChoosingScreen) {
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
                isInAttackSpeedSkillTreeChoosingScreen = false;
            }

            if (!hasPlusTwoEnemiesUpgrade) {
                plusTwoEnemies.Draw();
            }

            if (plusTwoEnemies.CheckClick(mousePos.x, mousePos.y) and money >= plusTwoUpgradeCost) {
                money -= plusTwoUpgradeCost;
                hasPlusTwoEnemiesUpgrade = true;
                num_of_enemies_to_spawn += 2;
                isInSkillTreeChoosingScreen = false;
                shouldUpdateTime = true;
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
        } else if (!shouldUpdateTime and !isInSkillTreeChoosingScreen and isInAttackSpeedSkillTreeChoosingScreen) {
                        DrawText("Press B to go back to the game", width - 350, height - 50, 20, BLACK);

            if (!hasPlusQuarterSecondAttackSpeed) {
                plusQuarterSecondAttackSpeed.Draw();
            }

            if (plusQuarterSecondAttackSpeed.CheckClick(mousePos.x, mousePos.y) and money >= plusQuarterSecondAttackSpeedCost) {
                money -= plusQuarterSecondAttackSpeedCost;
                hasPlusQuarterSecondAttackSpeed = true;
                attackCooldown -= 0.25;
                isInAttackSpeedSkillTreeChoosingScreen = false;
                shouldUpdateTime = true;
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

            if (!hasPlusHalfSecondAttackSpeed) {
                plusHalfSecondAttackSpeed.Draw();
            }

            if (plusHalfSecondAttackSpeed.CheckClick(mousePos.x, mousePos.y) and money >= plusHalfSecondAttackSpeedCost) {
                money -= plusHalfSecondAttackSpeedCost;
                hasPlusHalfSecondAttackSpeed = true;
                attackCooldown -= 0.5;
                isInAttackSpeedSkillTreeChoosingScreen = false;
                shouldUpdateTime = true;
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
                isInAttackSpeedSkillTreeChoosingScreen = false;
            }
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}