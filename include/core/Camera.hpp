#pragma once
#include "../../include/graphics/Shader.hpp"

class Camera{
	public:
		glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 position;
		glm::vec3 front;
		glm::vec3 right;
		glm::vec3 up;
		glm::vec3 dir;
		float pitch = 0.0f;
		float yaw = -90.0f;

		glm::mat4 Proj;
		glm::mat4 View;
	
		void matrixCalc(ShaderManager* shader, int width, int height, bool line);

};
