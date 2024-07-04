#include "./include/raylib.h"
#include <limits.h>
#include <stdio.h>
#include <stdbool.h>
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
	int limit_secs;
	int start_secs;
	bool starting_screen;
	Rectangle dim;
	char buf[BUFFER_LEN];
};

int main(int argc, char **argv) {
	struct widget_time central = {
		.elapsed_secs = 0,
		.limit_secs = 0,
		.starting_screen = true,
		.dim = {
			.width = WINDOW_WIDTH * 0.25,
			.height =  WINDOW_HEIGHT * 0.30,
			.x = (WINDOW_WIDTH * 0.15),
			.y = (WINDOW_HEIGHT * 0.14),
		}
	};

	Image icon = LoadImageSvg("./assets/kanban.svg", 50, 50);

	InitAudioDevice();
	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Kanban");
	SetWindowIcon(icon);
	SetWindowState(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_TOPMOST);

	Music game_music = LoadMusicStream("./assets/mixkit-big-thunder-rumble-1297.wav");
	PlayMusicStream(game_music);

	while (!WindowShouldClose()) {
		UpdateMusicStream(game_music);
		if (IsWindowResized()) {
			WINDOW_WIDTH = GetRenderWidth();
			WINDOW_HEIGHT = GetRenderHeight();
			central.dim.width = WINDOW_WIDTH * 0.25;
			central.dim.height =  WINDOW_HEIGHT * 0.30;
			central.dim.x = (WINDOW_WIDTH * 0.375);
			central.dim.y = (WINDOW_HEIGHT * 0.35);
		}

		if (central.starting_screen) {
			if (IsKeyPressed(KEY_ENTER)) {
				central.starting_screen = false;
				central.start_secs = GetTime();
			}
			if (IsKeyPressed(KEY_J)) {
				if (central.limit_secs <= 0) {
					central.limit_secs = 0;
				} else {
					central.limit_secs -= 1;
				}
			} else if (IsKeyPressed(KEY_K)) {
				central.limit_secs += 1;
				if (central.limit_secs == INT_MAX) {
					central.limit_secs = INT_MAX;
				}
			}

			if (IsKeyPressedRepeat(KEY_K)) {
				central.limit_secs += 10;
			}
			if (IsKeyPressedRepeat(KEY_J)) {
				central.limit_secs -= 10;
				if (central.limit_secs <= 0) {
					central.limit_secs = 0;
				}
			}

			snprintf(
				central.buf,
				BUFFER_LEN,
				"%02d:%02d",
				central.limit_secs / 60,
				central.limit_secs % 60
			);
		} else {
			central.elapsed_secs = GetTime() - central.start_secs;
			snprintf(central.buf, BUFFER_LEN, "%02d:%02d", central.elapsed_secs / 60, central.elapsed_secs % 60);
			if (central.elapsed_secs == central.limit_secs) {
				central.starting_screen = true;
			}
		}

		size_t len = strspn(central.buf, " ");
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

	UnloadMusicStream(game_music);
	CloseAudioDevice();
	CloseWindow();
	return 0;
}
