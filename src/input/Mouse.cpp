#include "../../include/input/Mouse.hpp"
#include <iostream>

void Mouse::update(GLFWwindow* window){
	int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
	double x;
	double y;
	glfwGetCursorPos(window, &x, &y);
	std::pair<double, double> oldPos = pos; // can use this for even managing but am lazy rn
	pos = std::pair<double,double>(x, y);
	
	if (state == GLFW_PRESS) {
		buttonStates.left = (buttonStates.left == "Up") ? "Pressed" : "Down";
	} else {
		buttonStates.left = "Up";
	}

	// Update the middle button
	state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE);
	if (state == GLFW_PRESS) {
		buttonStates.middle = (buttonStates.middle == "Up") ? "Pressed" : "Down";
	} else {
		buttonStates.middle = "Up";
	}

	// Update the right button
	state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);
	if (state == GLFW_PRESS) {
		buttonStates.right = (buttonStates.right == "Up") ? "Pressed" : "Down";
	} else {
		buttonStates.right = "Up";
	}
}

Mouse::Buttons Mouse::getButtonStates(){
	return buttonStates;
}
std::pair<double, double> Mouse::getPos(){
	return pos;
}
