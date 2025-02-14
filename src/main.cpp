#include <iostream>
#include <raylib.h>

using namespace std;

int player_score = 0;
int cpu_score = 0;

/*---ball class---*/
class Ball {
public:
    float x, y;
    int speed_x, speed_y;
    int radius;

    void Draw() {
        DrawCircle(x, y, radius, WHITE);
    }

    void Update() {
        x += speed_x;
        y += speed_y;

        if (y + radius >= GetScreenHeight() || y - radius <= 0) {
            speed_y *= -1;
        }
        if (x + radius >= GetScreenWidth()) {
            player_score++;
            ResetBall();
        }
        if (x - radius <= 0) {
            cpu_score++;
            ResetBall();
        }
    }

    void ResetBall() {
        x = GetScreenWidth() / 2;
        y = GetScreenHeight() / 2;

        int speed_choices[2] = {-1, 1};
        speed_x *= speed_choices[GetRandomValue(0,1)];
        speed_y *= speed_choices[GetRandomValue(0,1)];
    }
};

class Paddle {
protected:
    void LimitMovement() {
        if (y <= 0) {
            y = 0;
        }
        if (y >= GetScreenHeight() - height) {
            y = GetScreenHeight() - height;
        }
    }

public:
    float x,y;
    float width, height;
    int speed;

    void Draw() {
        DrawRectangle(x, y, width, height, WHITE);
    }

    void Update() {
        if (IsKeyDown(KEY_UP)) {
            y -= speed;
        }
        if (IsKeyDown(KEY_DOWN)) {
            y += speed;
        }

        LimitMovement();
    }
};

class CpuPaddle: public Paddle {
public:
    void Update(int ball_y) {
        if (y + height/2 > ball_y) {
            y = y - speed;
        }
        if (y + height/2 <= ball_y) {
            y = y + speed;
        }

        LimitMovement();
    }
};

Ball ball; // ball instance
Paddle player;
CpuPaddle cpu;


int main() {
    cout << "Starting the game" << endl;

    const int SCREEN_WIDTH = 1280;
    const int SCREEN_HEIGHT = 800;

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Pong Game"); // create window
    SetTargetFPS(60); //setting fps

    // ball:
    ball.radius = 20;
    ball.x = SCREEN_WIDTH/2;
    ball.y = SCREEN_HEIGHT/2;
    ball.speed_x = 7;
    ball.speed_y = 7;

    // player:
    player.width = 25;
    player.height = 120;
    player.x = 10;
    player.y = SCREEN_HEIGHT / 2 - player.height / 2;
    player.speed = 6;

    // cpu:
    cpu.height = 120;
    cpu.width = 25;
    cpu.x = SCREEN_WIDTH - cpu.width - 10;
    cpu.y = SCREEN_HEIGHT / 2 - cpu.height / 2;
    cpu.speed = 6;

    /*---game loop---*/
    while(WindowShouldClose() == false) {
        BeginDrawing();

        // Updating:
        ball.Update();
        player.Update();
        cpu.Update(ball.y);

        // Checking for collisions:
        if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{player.x, player.y, player.width, player.height})) {
            ball.speed_x *= -1;
        }
        if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{cpu.x, cpu.y, cpu.width, cpu.height})) {
            ball.speed_x *= -1;
        }

        // Drawing:
        ClearBackground(BLACK);
        DrawLine(SCREEN_WIDTH / 2, 0, SCREEN_WIDTH / 2, SCREEN_HEIGHT, WHITE);
        ball.Draw();
        player.Draw();
        cpu.Draw();
        DrawText(TextFormat("%i", player_score), SCREEN_WIDTH/4 - 20, 20, 80, WHITE);
        DrawText(TextFormat("%i", cpu_score), 3 * SCREEN_WIDTH/4 - 20, 20, 80, WHITE);

        EndDrawing();
    }


    CloseWindow(); // close game
    return 0;
}