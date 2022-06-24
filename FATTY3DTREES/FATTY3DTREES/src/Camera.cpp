#include "Camera.h"

namespace Camera {

    const glm::vec3 DEFAULT_DIRECTION(0.f, 0.f, -1.f);
    const glm::vec3 ORIGIN(0.f, 0.f, 0.f);
    const glm::vec3 DEFAULT_POSITION(0.f, 0.f, -50.f);
    const float DEFAULT_SENSITIVITY = 0.05f;
    glm::mat4 viewMatrix(1.f);
    glm::vec3 yawAxis(0.f, 1.f, 0.f);
    float dx = 0.f;
    float dy = 0.f;
    float yaw = 0.f;
    float sensitivity = DEFAULT_SENSITIVITY;
    float zoom = 0.f;
    void Update() {

        zoom = DEFAULT_POSITION.z + InputManager::GetMouseScrollYOffset()/4;
        sensitivity = GetZoomSensitivity();
        if (InputManager::GetKeyState(GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
            glm::vec3 tmp(InputManager::GetDeltaMouseX() * sensitivity, -InputManager::GetDeltaMouseY() * sensitivity, 0.f);
            dx += tmp.x;
            dy += tmp.y;
        }
        if (InputManager::GetKeyState(GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
            yaw += InputManager::GetDeltaMouseX();
        }

        viewMatrix = glm::translate(glm::mat4(1.f), glm::vec3(dx, dy, zoom));
        viewMatrix = glm::rotate(viewMatrix, glm::radians(yaw), yawAxis);

        
    }
    glm::mat4 GetViewMatrix() { return viewMatrix; }
    float GetZoomSensitivity() {
        return DEFAULT_SENSITIVITY * (abs(zoom) / -DEFAULT_POSITION.z);
    }
}