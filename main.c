#include "./include/raylib.h"
#include <stdio.h>
#include <string.h>

#define BUFFER_LEN 1024

static int WINDOW_WIDTH = 800;
static int WINDOW_HEIGHT = 600;

#define PADDING_X 24
#define PADDING_Y 24

const static Color gruvbox_red = (Color) {.r = 251,.g = 73,.b = 51,.a = 255};
const static Color gruvbox_black = (Color) {.r = 29, .g = 32, .b = 33, .a = 255};
const static Color gruvbox_white = (Color) {.r = 212, .g = 190, .b = 152, .a = 255};

struct widget_time {
	int elapsed_secs;
	Rectangle dim;
	char buf[BUFFER_LEN];
};

int main(int argc, char **argv) {
	struct widget_time central = {
		.elapsed_secs = 0.0,
		.dim = {
			.width = WINDOW_WIDTH * 0.25,
			.height =  WINDOW_HEIGHT * 0.30,
			.x = (WINDOW_WIDTH * 0.15),
			.y = (WINDOW_HEIGHT * 0.14),
		}
	};

	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Kanban");
	SetWindowState(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_TOPMOST);
	SetWindowPosition(0, 0);

	while (!WindowShouldClose()) {

		if (IsWindowResized()) {
			WINDOW_WIDTH = GetRenderWidth();
			WINDOW_HEIGHT = GetRenderHeight();
			central.dim.width = WINDOW_WIDTH * 0.25;
			central.dim.height =  WINDOW_HEIGHT * 0.30;
			central.dim.x = (WINDOW_WIDTH * 0.375);
			central.dim.y = (WINDOW_HEIGHT * 0.35);
		}

		central.elapsed_secs = GetTime();
		snprintf(central.buf, BUFFER_LEN, "%02d:%02d", central.elapsed_secs / 60, central.elapsed_secs % 60);
		size_t len = strspn(central.buf, " ");

		if (central.elapsed_secs / 60 == 25) {
			break;
		}

		Vector2 text_dim = MeasureTextEx(
			GetFontDefault(),
			central.buf + len, 
			(WINDOW_WIDTH * 0.04) + (WINDOW_HEIGHT * 0.03),
			0
		);

		BeginDrawing();
		ClearBackground(gruvbox_black);

		#ifdef DEBUG_MODE
		DrawRectangleLines(
			central.dim.x,
			central.dim.y,
			central.dim.width,
			central.dim.height,
			gruvbox_red
		);
		#endif /* ifdef DEBUG_MODE */

		DrawText(
			central.buf,
			(WINDOW_WIDTH - text_dim.x) / 2,
			(WINDOW_HEIGHT - text_dim.y) / 2,
			(WINDOW_WIDTH * 0.04) + (WINDOW_HEIGHT * 0.03),
			gruvbox_white
		);

		EndDrawing();
	}

	CloseWindow();
	return 0;
}
