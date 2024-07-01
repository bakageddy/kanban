#include "./include/raylib.h"
#include <string.h>
#include <time.h>

#define BUFFER_LEN 1024

typedef struct {
	int wwidth;
	int wheight;
	time_t time;
	char buf[BUFFER_LEN];
	size_t buf_len;
} timer_state;

void handle_resize(timer_state *state) {
	if (IsWindowResized()) {
		state -> wwidth = GetScreenWidth();
		state -> wheight = GetScreenHeight();
	}
}

int main(void) {

	timer_state state = {
		.wwidth = 800,
		.wheight = 600,
		.time = time(0),
		.buf = {0},
		.buf_len = 0,
	};

	InitWindow(state.wwidth, state.wheight, "Timer");
	SetWindowState(FLAG_WINDOW_RESIZABLE);
	while (!WindowShouldClose()) {

		// Time Stuff
		strncpy(state.buf, ctime(&state.time), BUFFER_LEN);
		state.buf_len = strlen(state.buf);
		state.time = time(0);
		handle_resize(&state);

		BeginDrawing();
		ClearBackground(RAYWHITE);
		DrawText(state.buf, (state.wwidth - state.buf_len) / 2, state.wheight / 2, 20, BLACK);
		EndDrawing();

	}
	CloseWindow();
	return 0;
}
