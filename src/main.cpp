#include "SoundDevice.h"
#include "SoundBuffer.h"
#include "SoundSource.h"
#include "Stream.h"
#include <stdio.h>
#include <raylib.h>


int main() {

	InitWindow(800, 600, "RainCloud");

	SoundDevice* device = SoundDevice::Get();
	Stream stream ("../samples/song.wav");

	stream.Play();


	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(PINK);
		DrawText("RainCloud Demo", 190, 200, 20, BLACK);

		EndDrawing();
		stream.Update();


	}

	CloseWindow();



	return 0;

}
