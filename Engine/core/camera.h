#pragma once
#include <SDL_events.h> 

class Camera {
public:
    glm::vec3 velocity = glm::vec3(0.f);
    glm::vec3 position = glm::vec3(30.f, -00.f, -085.f);;
    // vertical rotation
    float pitch { 0.f };
    // horizontal rotation
    float yaw { 0.f };

    glm::mat4 getViewMatrix() const;
    glm::mat4 getRotationMatrix() const;

    void processSDLEvent(SDL_Event& e);

    void update();
};
