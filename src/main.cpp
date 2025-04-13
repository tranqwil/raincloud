//std
#include <stdio.h>

//Audio
#include "SoundDevice.h"
#include "SoundBuffer.h"
#include "SoundSource.h"
#include "Stream.h"

//raylib
#include <raylib.h>

//Game
#include "GameCamera.h"


int main() {

	InitWindow(800, 600, "RainCloud");
	DisableCursor();

	SoundDevice* device = SoundDevice::Get();
	Stream stream ("../samples/song.wav");
	stream.Play();
	SetTargetFPS(60);

	auto* cam = new GameCamera(Vector3(0, 5.0, 5.0));




	while (!WindowShouldClose())
	{
		ClearBackground(BLACK);
		BeginDrawing();
		BeginMode3D(cam->camera);

		DrawGrid(10, .5f);



		EndMode3D();
		EndDrawing();

		stream.Update();
		cam->update();


	}

	CloseWindow();



	return 0;

}
