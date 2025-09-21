#include <raylib.h>

#define NUM_OF_COLORS 21
#define FPS 30

#define memmon_close CloseWindow

static const int screenWidth = 1024;
static const int screenHeight = 1024;

static const Color colors[NUM_OF_COLORS] = {
        GREEN, MAROON, ORANGE, DARKGREEN, DARKBLUE, DARKPURPLE, DARKBROWN,
        GRAY, RED, GOLD, LIME, BLUE, VIOLET, BROWN, LIGHTGRAY, YELLOW,
        PINK, SKYBLUE, PURPLE, BEIGE, DARKGRAY};

void memmon_init() {
	InitWindow(screenWidth, screenHeight, "MemMon");
    SetTargetFPS(FPS);
	BeginDrawing();
		ClearBackground(RAYWHITE);
	EndDrawing();
}

void memon_reset() {
	BeginDrawing();
		ClearBackground(RAYWHITE);
	EndDrawing();
}


void memmon_update_pixle(uint16_t x, uint16_t y, Color color) {
	BeginDrawing();
		DrawRectangleV((Vector2){x*4,y*4}, (Vector2){4,4}, color);
	EndDrawing();
}

