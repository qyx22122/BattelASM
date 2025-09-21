#include <raylib.h>

#define NUM_OF_COLORS 21
#define FPS 500

//#define MEMMON_DEBUG

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


void memmon_update_pixle(uint16_t addr, Color color) {
	int x = addr % 256;
	int y = addr / 256;
	BeginDrawing();
		DrawRectangleV((Vector2){x*4,y*4}, (Vector2){4,4}, color);
	EndDrawing();
#ifdef MEMMON_DEBUG
	printf("\nMEMMON : FPS = %d\n", GetFPS());
#endif
}

