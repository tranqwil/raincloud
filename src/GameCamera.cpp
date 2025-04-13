//
// Created by Will on 13/04/2025.
//

#include "GameCamera.h"

#include <iostream>
#include <ostream>

GameCamera::GameCamera(const Vector3 position)
{
    this->position = position;
    camera.position = this->position;
    camera.target = Vector3(0.0f, 0.0f, 0.0f);
    camera.up = Vector3(0.0f, 1.0f, 0.0f);
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

}

GameCamera::~GameCamera() {

}

void GameCamera::SetPosition(const Vector3 position) {
    this->position = position;
}

void GameCamera::update() {

    Move();

    //Move audio listener
    alListener3f(AL_POSITION, position.x, position.y, position.z);

    if (alGetError() != AL_NO_ERROR) {
        std::cerr << "Error updating camera listener position" << std::endl;
    }


}

void GameCamera::Move() {
    //take input
    Vector3 forward = Vector3Normalize(Vector3Subtract(camera.target, camera.position));
    Vector3 right = Vector3CrossProduct(forward, camera.up);



    if (IsKeyDown(KEY_W)) {

        position = Vector3Add(camera.position, Vector3Scale(forward, speed));
        camera.target = Vector3Add(camera.target, Vector3Scale(forward, speed));
    }
    if (IsKeyDown(KEY_S)) {

        position = Vector3Subtract(camera.position, Vector3Scale(forward, speed));
        camera.target = Vector3Subtract(camera.target, Vector3Scale(forward, speed));
    }

    if (IsKeyDown(KEY_A)) {
        position = Vector3Subtract(camera.position, Vector3Scale(right, speed));
        camera.target = Vector3Subtract(camera.target, Vector3Scale(right, speed));

    }
    if (IsKeyDown(KEY_D)) {
        position = Vector3Add(camera.position, Vector3Scale(right, speed));
        camera.target = Vector3Add(camera.target, Vector3Scale(right, speed));

    }

    //Update params
    camera.position = this->position;

}



