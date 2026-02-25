#pragma once
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#define GLM_FORCE_CTOR_INIT
#include <glm/glm.hpp>

class Mouse{
	public:
		struct Buttons{
			std::string left;
			std::string right;
			std::string middle;
		};

	private:
		Buttons buttonStates;
		std::pair<double,double> pos;
		
	public:
		void update(GLFWwindow* window);

		Buttons getButtonStates();
		std::pair<double,double> getPos();
};
