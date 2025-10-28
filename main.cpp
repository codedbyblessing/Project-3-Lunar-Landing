#include "CS3113/Entity.h"

constexpr int SCREEN_WIDTH  = 7000 / 2; //trying to perserve the dimentions of the background to scale
constexpr int SCREEN_HEIGHT = 2800 / 2;
constexpr int FPS           = 120;

constexpr Vector2 ORIGIN = { SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f };

enum AppStatus { RUNNING, TERMINATED };
AppStatus gAppStatus = RUNNING;

float gPreviousTicks = 0.0f;

Entity *space_ship = nullptr; //doesnt exists yer
Texture2D background = LoadTexture("assets/background.jpg");
DrawTexture(background, 0, 0, BLUE);


bool landed = false;
bool crashed = false;
void initialise();
void processInput();
void update();
void render();
void shutdown();

void initialise()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Lunar Lander");
    background = LoadTexture("space.png");
    space_ship = new Entity("rocket.png", ORIGIN, {64, 64});
    SetTargetFPS(FPS);
}

void processInput()
{
    if (IsKeyPressed(KEY_Q) || WindowShouldClose()) gAppStatus = TERMINATED;
    if (IsKeyDown(KEY_UP))    space_ship->applyThrustUp();
    if (IsKeyDown(KEY_LEFT))  space_ship->applyThrustLeft();
    if (IsKeyDown(KEY_RIGHT)) space_ship->applyThrustRight();
}

void update()
{
    float ticks = (float)GetTime();
    float deltaTime = ticks - gPreviousTicks;
    gPreviousTicks = ticks;
    space_ship->update(deltaTime);
    if (space_ship->getPosition().y >= SCREEN_HEIGHT - 20 && !landed && !crashed)
    {
        float vy = fabs(space_ship->getVelocity().y);
        if (vy < 150.0f) landed = true;
        else crashed = true;
    }
}

void render()
{
    BeginDrawing();
    ClearBackground(BLACK);
    DrawTextureEx(background, {0, 0}, 0.0f, 1.0f, WHITE);
    space_ship->render();
    //raylibs default font (also used in proj2 for the retro text look)
    DrawText(TextFormat("Fuel: %.0f", space_ship->getFuel()), 20, 20, 20, RAYWHITE);
    if (landed) DrawText("Landed", SCREEN_WIDTH / 2 - 60, SCREEN_HEIGHT / 2, 30, GREEN);
    if (crashed) DrawText("Yikes the ship sunk", SCREEN_WIDTH / 2 - 150, SCREEN_HEIGHT / 2, 30, RED);
    EndDrawing();
}

void shutdown()
{
    UnloadTexture(background);
    //getting rid of dangling pointers spaceship was initialized as a null pointer
    delete space_ship;
    CloseWindow();
}

int main(void)
{
    initialise();

    while (gAppStatus == RUNNING)
    {
        processInput();
        update();
        render();
    }
    shutdown();
    return 0;
}
