#include "raylib.h"
#include <iostream>

using namespace std;

Color Dark_Blue   = {28, 37, 65, 255};
Color Light_Blue  = {58, 80, 107, 255};
Color Super_Light_Blue  = {78, 165, 217, 100};
Color Light_Green = {111, 255, 233, 255};

int player_score = 0;
int bot_score = 0;

class Ball{
public:
    float x,y;
    float speed_x, speed_y;
    int radius;

    void Draw(void){
        DrawCircle(x, y, radius, Light_Green);
    }

    void Update(void){
        x += speed_x;
        y += speed_y;

        if (y+radius >= GetScreenHeight() || y-radius <= 0) {
            speed_y *= -1;
        }

        if (x+radius >= GetScreenWidth()) {
            player_score++;
            Reset();
        }

        if (x-radius <= 0) {
            bot_score++;
            Reset();
        }
    }

    void Reset(){
        x = GetScreenWidth()/2;
        y = GetScreenHeight()/2;
        
        int direction_rand[2] = {-1, 1};
        speed_x = 7 * direction_rand[GetRandomValue(0, 1)];
        speed_y = 7 * direction_rand[GetRandomValue(0, 1)];
    }
};

class Player{
public:
    float x,y;
    float width, height;
    int speed;

    void Draw(void){
        DrawRectangleRounded(Rectangle{x, y, width, height}, 0.8, 0, WHITE);
    }

    void Update(void){
        if(IsKeyDown(KEY_W)){
            y -= speed;
        }
        if(IsKeyDown(KEY_S)){
            y += speed;
        }

        // the following part could have been a protected method inherited by the bot
        // but it's pointless since it's small 
        if(y <= 0){
            y = 0;
        }

        if(y + height >= GetScreenHeight()){
            y = GetScreenHeight() - height;
        }
    }
};

class Bot: public Player{
    public: 
    void Update(float ball_y){
        if(y + height/2 > ball_y){
            y -= speed;
        }

        if(y + height/2 <= ball_y){
            y += speed;
        }

        if(y <= 0){
            y = 0;
        }

        if(y + height >= GetScreenHeight()){
            y = GetScreenHeight() - height;
        }
    }
};

Ball ball;
Player player;
Bot bot;

int main(void){
    cout << "test" ;
    const int screen_width = 1200;
    const int screen_height = 800;

    InitWindow(screen_width, screen_height, "PongTae");
    SetTargetFPS(60);

    ball.radius = 10;
    ball.x = screen_width/2;
    ball.y = screen_height/2;
    ball.speed_x = 7;
    ball.speed_y = 7;

    player.width = 20;
    player.height = 120;
    player.x = 10;
    player.y = screen_height/2;
    player.speed = 6;

    bot.width = 20;
    bot.height = 120;
    bot.x = screen_width - bot.width - 10;
    bot.y = screen_height / 2 - bot.height/2;
    bot.speed = 6;

    while (WindowShouldClose()==false) {
        BeginDrawing();

        //update
        ball.Update();
        player.Update();
        bot.Update(ball.y);

        //collision
        if(CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{player.x, player.y, player.width, player.height})){
            ball.speed_x *= -1.1;
        }

        if(CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{bot.x, bot.y, bot.width, bot.height})){
            ball.speed_x *= -1.1;
        }

        //drawing
        ClearBackground(Dark_Blue);
        DrawRectangle(screen_width/2, 0, screen_width/2, screen_height, Light_Blue);
        DrawCircle(screen_width/2, screen_height/2, 150, Super_Light_Blue);
        DrawLine(screen_width / 2, 0, screen_width / 2, screen_height, WHITE);
        ball.Draw();
        bot.Draw();
        player.Draw();
        DrawText(TextFormat("%i", player_score), screen_width/4 - 20, 20, 80, WHITE);
        DrawText(TextFormat("%i", bot_score), 3 * screen_width/4 - 20, 20, 80, WHITE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}