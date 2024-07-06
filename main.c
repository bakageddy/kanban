#include "./include/raylib.h"
#include <limits.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <glob.h>

#define BUFFER_LEN 1024

#define PADDING_X 24
#define PADDING_Y 24

#define PLAYLIST_LEN 9

static int WINDOW_WIDTH = 800;
static int WINDOW_HEIGHT = 600;

const static Color gruvbox_red = (Color) {.r = 251,.g = 73,.b = 51,.a = 255};
const static Color gruvbox_black = (Color) {.r = 29, .g = 32, .b = 33, .a = 255};
const static Color gruvbox_white = (Color) {.r = 212, .g = 190, .b = 152, .a = 255};

static Music playlist[PLAYLIST_LEN] = {0};

struct widget_time {
	int elapsed_secs;
	int limit_secs;
	int start_secs;
	size_t current_song;
	bool starting_screen;
	Rectangle dim;
	char buf[BUFFER_LEN];
};

typedef bool (clicked_t)(Rectangle dim);

struct widget_button {
	char c;
	Rectangle dim;
	clicked_t *clicked;
};

bool button_clicked(Rectangle dim) {
	if (IsMouseButtonPressed(KEY_RIGHT)) {
		Vector2 pos = GetMousePosition();
		return (
			dim.x <= pos.x 
			&& pos.x >= dim.x + dim.width
			&& dim.y <= pos.y
			&& pos.y >= dim.y + dim.height
		);
	} else {
		return false;
	}
}

void load_playlist(void) {
	playlist[0] = LoadMusicStream("./assets/mixkit-game-level-music-689.wav");
	playlist[1] = LoadMusicStream("./assets/mixkit-big-thunder-rumble-1297.wav");
	playlist[2] = LoadMusicStream("./assets/mixkit-waterfall-ambience-2513.wav");
	playlist[3] = LoadMusicStream("./assets/mixkit-campfire-night-wind-1736.wav");
	playlist[4] = LoadMusicStream("./assets/mixkit-bubbling-volcano-lava-2440.wav");
	playlist[5] = LoadMusicStream("./assets/mixkit-heavy-storm-rain-loop-2400.wav");
	playlist[6] = LoadMusicStream("./assets/mixkit-forest-with-birds-singing-1235.wav");
	playlist[7] = LoadMusicStream("./assets/mixkit-forest-ambience-with-croaking-frogs-1241.wav");
	playlist[8] = LoadMusicStream("./assets/mixkit-crickets-and-insects-in-the-wild-ambience-39.wav");
}

int main(int argc, char **argv) {
	struct widget_time central = {
		.elapsed_secs = 0,
		.limit_secs = 0,
		.starting_screen = true,
		.start_secs = 0,
		.current_song = 0,
		.dim = {
			.width = WINDOW_WIDTH * 0.25,
			.height =  WINDOW_HEIGHT * 0.30,
			.x = (WINDOW_WIDTH * 0.15),
			.y = (WINDOW_HEIGHT * 0.14),
		}
	};

	struct widget_button left_button = {
		.c = '<',
		.clicked = button_clicked,
		.dim = (Rectangle) {
			.x = 20,
			.y = (float) WINDOW_HEIGHT / 2,
			.height = 50,
			.width = 60,
		}
	};

	struct widget_button right_button = {
		.c = '>',
		.clicked = button_clicked,
	};

	Image icon = LoadImageSvg("./assets/kanban.svg", 50, 50);

	InitAudioDevice();
	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Kanban");
	SetWindowIcon(icon);
	SetWindowState(FLAG_WINDOW_RESIZABLE);

	load_playlist();
	PlayMusicStream(playlist[central.current_song]);


	while (!WindowShouldClose()) {
		UpdateMusicStream(playlist[central.current_song]);

		if (left_button.clicked(left_button.dim)) {
			StopMusicStream(playlist[central.current_song]);
			central.current_song += 1;
			central.current_song %= PLAYLIST_LEN;
			PlayMusicStream(playlist[central.current_song]);
		}

		if (IsKeyPressed(KEY_H)) {
			StopMusicStream(playlist[central.current_song]);
			if (central.current_song - 1 <= 0) {
				central.current_song = 0;
			} else {
				central.current_song--;
			}
			central.current_song %= PLAYLIST_LEN;
			PlayMusicStream(playlist[central.current_song]);
		}

		if (IsKeyPressed(KEY_L)) {
			StopMusicStream(playlist[central.current_song]);
			central.current_song += 1;
			central.current_song %= PLAYLIST_LEN;
			PlayMusicStream(playlist[central.current_song]);
		}
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
			}

			if (IsKeyPressed(KEY_K)) {
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
				central.elapsed_secs = 0;
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

		DrawRectangleRec(left_button.dim, gruvbox_white);

		DrawText(
			central.buf,
			(WINDOW_WIDTH - text_dim.x) / 2,
			(WINDOW_HEIGHT - text_dim.y) / 2,
			(WINDOW_WIDTH * 0.04) + (WINDOW_HEIGHT * 0.03),
			gruvbox_white
		);

		EndDrawing();
	}

	for (int i = 0; i < PLAYLIST_LEN; i++) {
		UnloadMusicStream(playlist[i]);
	}
	CloseAudioDevice();
	CloseWindow();
	return 0;
}
