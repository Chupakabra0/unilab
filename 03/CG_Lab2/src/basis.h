#ifndef BASIS_H
#define BASIS_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

using namespace glm;

const vec3 X = vec3(1.0f, 0.0f, 0.0f);
const vec3 Y = vec3(0.0f, 1.0f, 0.0f);
const vec3 Z = vec3(0.0f, 0.0f, 1.0f);

// Default camera values
const float BASIS_MOVE_SPEED   =  0.5f;
const float BASIS_ROTATE_SPEED =  0.5f;
const float BASIS_ANIMATE_SPEED = 1.0f;

class Basis {
public:
    mat4 view;
    vec3 animation;
    quat rotation;
    bool camera;

    Basis(bool camera = false);
    void move(vec3 distances);
    void rotate(vec3 direction);

    vec4 get_camera_position();
    vec4 get_camera_direction();

    vec4 get_position();

    mat4 get_full_view();

    void animate(float deltaTime);
};

#endif // BASIS_H
