#pragma once
#include "glm/glm.hpp"
#include "../graphics/Shader.hpp"

class PointLight{
	public:
		glm::vec3 pos;
		glm::vec3 diffuse;
		glm::vec3 specular;
		float diffStrength;
		float specStrength;
		PointLight(glm::vec3 pos, glm::vec3 diffuse, glm::vec3 specular, float diffStrength, float specStrength);
		void update(ShaderManager shader, int lightNum);	
};
