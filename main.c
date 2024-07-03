#include "./include/raylib.h"
#include <time.h>

#define BUFFER_LEN 1024

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#define PADDING_X 24
#define PADDING_Y 24

struct widget_time {
	time_t time;
	Rectangle dim;
	char buf[BUFFER_LEN];
};

int main(int argc, char **argv) {
	struct widget_time central = {
		.time = time(0),
		.dim = {
			.width = 300,
			.height = 160,
			.x = (float) (WINDOW_WIDTH - 300) / 2,
			.y = (float) (WINDOW_HEIGHT - 160) / 2,
		}
	};

	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Kanban");

	while (!WindowShouldClose()) {
		strftime(central.buf, BUFFER_LEN, "%H-%M", localtime(&central.time));
		Vector2 text_dim = MeasureTextEx(GetFontDefault(), central.buf, 50, 0);
		BeginDrawing();
		ClearBackground(RAYWHITE);
		DrawRectangleRec(central.dim, RED);
		DrawText(central.buf, central.dim.x + text_dim.x * 0.5, central.dim.y + text_dim.y * 0.5, 50, BLACK);
		EndDrawing();
	}

	CloseWindow();
	return 0;
}
