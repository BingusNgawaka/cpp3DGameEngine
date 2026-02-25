#pragma once
#include <map>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <functional>
#include "../../include/core/Events.hpp"
#include <unordered_set>

class Keyboard {
	public:
	    static void setEventManager(EventManager* eventManager);
	    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	    static bool isKeyPressed(int key);

	private:
	    static EventManager* events;
	    static std::unordered_set<int> pressedKeys;
};

