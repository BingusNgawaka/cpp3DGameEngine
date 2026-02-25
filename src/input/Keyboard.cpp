#include "../../include/input/Keyboard.hpp"
#include <iostream>

EventManager* Keyboard::events = nullptr;
std::unordered_set<int> Keyboard::pressedKeys;

void Keyboard::setEventManager(EventManager* eventManager) {
    events = eventManager;
}

void Keyboard::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (events) {
        if (action == GLFW_PRESS) {
            // Key pressed
            pressedKeys.insert(key);
            events->triggerEvent(std::to_string(key), 0);
        } else if (action == GLFW_RELEASE) {
            // Key released
            pressedKeys.erase(key);
        }
    }
}

bool Keyboard::isKeyPressed(int key) {
    return pressedKeys.find(key) != pressedKeys.end();
}
