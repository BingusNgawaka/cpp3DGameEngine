#pragma once
#include "PointLight.hpp"
#include "MeshHandler.hpp"
#include <vector>
#include <glm/glm.hpp>
#include <map>

class LightManager{
	private:
		int num;
	public:
		std::vector<PointLight> pointLights;
		void addPointLight(std::map<std::string, Mesh>* meshes, glm::vec3 pos, glm::vec3 diffuse, glm::vec3 specular, float diffStrength, float specStrength);
		void update(std::map<std::string, Mesh>* meshes, ShaderManager shader);
};
