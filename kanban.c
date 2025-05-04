#include "./include/raylib.h"
#include <assert.h>
#include <stdbool.h>

typedef struct {
	int window_height;
	int window_width;
	int font_size;
} Kanban;

typedef struct {
	long minutes;
	long seconds;
} Session;

char *format_session(Session s) {
	assert(s.minutes < 100);
	assert(s.seconds < 100);
	static char buffer[6] = "00:00"; 
	buffer[0] = '0' + s.minutes / 10;
	buffer[1] = '0' + s.minutes % 10;
	buffer[3] = '0' + s.seconds / 10;
	buffer[4] = '0' + s.seconds % 10;
	return buffer;
}

void render_session(Kanban state, Session s) {
	char *text = format_session(s);
	Vector2 size = MeasureTextEx(GetFontDefault(), text, state.font_size, 1);
	int posx = (state.window_width - size.x) / 2;
	int posy = (state.window_height - size.y) / 2;
	DrawText(text, posx, posy, state.font_size, WHITE);
}

int main(int argc, char** argv) {
	Kanban state = {
		.window_width = 800,
		.window_height = 600,
		.font_size = 800 / 20
	};

	Session session = {
		.minutes = 10,
		.seconds = 20
	};

	InitWindow(state.window_width, state.window_height, "Kanban");
	SetWindowState(FLAG_WINDOW_RESIZABLE);
	SetTargetFPS(60);

	while (!WindowShouldClose()) {
		// State Update Part
		if (IsWindowResized()) {
			state.window_height = GetScreenHeight();
			state.window_width = GetScreenWidth();
			state.font_size = state.window_width / 20;
		}

		// Rendering Part
		BeginDrawing();

		ClearBackground(BLACK);
		render_session(state, session);

		EndDrawing();
	}

	CloseWindow();
	return 0;
}
