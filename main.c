#include "./include/raylib.h"
#include <stdio.h>
#include <string.h>

#define BUFFER_LEN 1024

static int WINDOW_WIDTH = 800;
static int WINDOW_HEIGHT = 600;

#define PADDING_X 24
#define PADDING_Y 24

struct widget_time {
	int elapsed_secs;
	Rectangle dim;
	char buf[BUFFER_LEN];
};

int main(int argc, char **argv) {
	struct widget_time central = {
		.elapsed_secs = 0.0,
		.dim = {
			.width = WINDOW_WIDTH / 4.0,
			.height =  WINDOW_HEIGHT / 6.0,
			.x = (WINDOW_WIDTH) / 2.0,
			.y = (WINDOW_HEIGHT) / 2.0,
		}
	};

	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Kanban");
	SetWindowState(FLAG_WINDOW_RESIZABLE);

	while (!WindowShouldClose()) {

		if (IsWindowResized()) {
			WINDOW_WIDTH = GetRenderWidth();
			WINDOW_HEIGHT = GetRenderHeight();
			central.dim.width = WINDOW_WIDTH * 0.25;
			central.dim.height =  WINDOW_HEIGHT * 0.30;
			central.dim.x = (WINDOW_WIDTH - central.dim.width) / 2;
			central.dim.y = (WINDOW_HEIGHT - central.dim.height) / 2;
		}

		central.elapsed_secs = GetTime();
		snprintf(central.buf, BUFFER_LEN, "%02d:%02d", central.elapsed_secs / 60, central.elapsed_secs % 60);
		size_t len = strspn(central.buf, " ");
		Vector2 text_dim = MeasureTextEx(
			GetFontDefault(),
			central.buf + len, 
			(WINDOW_WIDTH * 0.04) + (WINDOW_HEIGHT * 0.03),
			0
		);

		BeginDrawing();
		ClearBackground(RAYWHITE);

		DrawRectangleRec(central.dim, RED);
		DrawText(
			central.buf,
			(WINDOW_WIDTH - text_dim.x) / 2,
			(WINDOW_HEIGHT - text_dim.y) / 2,
			(WINDOW_WIDTH * 0.04) + (WINDOW_HEIGHT * 0.03),
			BLACK
		);

		EndDrawing();
	}

	CloseWindow();
	return 0;
}
