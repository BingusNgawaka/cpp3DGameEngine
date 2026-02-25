#pragma once
#include "Camera.hpp"
#include "../input/Keyboard.hpp"
#include "../input/Mouse.hpp"
#include "MeshHandler.hpp"
#include <GLFW/glfw3.h>
#include <iostream>

class Engine{
	public:
		GLFWwindow* window = nullptr;
		int W;
		int H;
		Mouse mouse;
		static Keyboard keyboard;
		static EventManager events;
		static ShaderManager shader;
		Camera camera;
		std::map<std::string, Mesh> meshes;
		void init(int width, int height, char* title);
		int main();
		void draw(float deltaTime);
		void update(float deltaTime);

};
