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

	//Raylib
	InitWindow(800, 600, "RainCloud");
	SetTargetFPS(60);
	Camera camera = { 0 };
	camera.position = Vector3( 0.0f, 10.0f, 10.0f );  // Camera position	camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };       // Camera looking at point
	camera.up = Vector3( 0.0f, 1.0f, 0.0f );           // Camera up vector
	camera.fovy = 45.0f;                                 // Camera field-of-view Y
	camera.projection = CAMERA_PERSPECTIVE;              // Perspective mode


	//Audio
	SoundDevice* device = SoundDevice::Get();
	SoundBuffer* soundBuffer = SoundBuffer::Get();

	ALuint track = soundBuffer->AddSoundEffect("../samples/song.wav");
	SoundSource speaker;
	speaker.SetGain(1.0f);
	speaker.SetPitch(1.0f);
	speaker.SetPosition(0.0f,0.5f, 0.0f);
	speaker.Play(track);


	//Cube
	Vector3 cubePosition (3.0f, 0.5f, -6.0f);







	while (!WindowShouldClose())
	{

		//input
		if (IsKeyDown(KEY_UP))
		{
			cubePosition.z -= 0.1f;
		}
		if (IsKeyDown(KEY_DOWN)) {
			cubePosition.z += 0.1f;
		}
		if (IsKeyDown(KEY_LEFT)) {
			cubePosition.x -= 0.1f;
		}
		if (IsKeyDown(KEY_RIGHT))
		{
			cubePosition.x += 0.1f;
		}



		UpdateCamera(&camera, CAMERA_ORBITAL);

		alListener3f(AL_POSITION, camera.position.x, camera.position.y, camera.position.z);


		Vector3 forward = Vector3Normalize(Vector3Subtract(camera.target, camera.position)); // Forward direction
		Vector3 up = Vector3Normalize(camera.up); // Already usually normalized, but safe to do

		float orientation[6] = {
			forward.x, forward.y, forward.z,   // "at" vector
			up.x, up.y, up.z                   // "up" vector
		};

		alListenerfv(AL_ORIENTATION, orientation);

		// Optionally set velocity to 0 (or compute based on delta)
		alListener3f(AL_VELOCITY, 0.0f, 0.0f, 0.0f);

		// Draw
		BeginDrawing();
		ClearBackground(DARKBLUE);

		BeginMode3D(camera);
		DrawGrid(100, 1.0f); // Draw grid (20x20 lines, 1 unit spacing)
		DrawCube(cubePosition, 1.0f, 1.0f, 1.0f, PINK); // Center cube
		DrawCubeWires(cubePosition, 1.0f, 1.0f, 1.0f, GREEN); // Wireframe
		EndMode3D();

		EndDrawing();

		speaker.SetPosition(cubePosition.x, cubePosition.y, cubePosition.z);
	}
	CloseWindow();



	return 0;

}
