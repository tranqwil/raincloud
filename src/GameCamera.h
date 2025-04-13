//
// Created by Will on 13/04/2025.
//

#ifndef GAMECAMERA_H
#define GAMECAMERA_H
#include "raylib.h"
#include "raymath.h"
#include <AL/al.h>
#include <AL/alc.h>

class GameCamera {

public:
    explicit GameCamera(Vector3 position);
    ~GameCamera();

    void update();

    void SetPosition(Vector3 position);


    Camera3D camera;
    Vector3 position;

private:
    void Move();

    float yaw = 0.0f;
    float pitch = 0.0f;
    float sensitivity = 0.002f;
    float speed = 0.1f;





};



#endif //GAMECAMERA_H
