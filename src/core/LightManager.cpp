#include "../../include/core/LightManager.hpp"

void LightManager::update(std::map<std::string, Mesh>* meshes, ShaderManager shader){
	num = pointLights.size();
	int index = 0;
	for(auto& light: pointLights){
		std::string strIndex = "light" + std::to_string(index);
		light.update(shader, index);
		(*meshes)[strIndex].translate(light.pos);
		index++;
	}
}

void LightManager::addPointLight(std::map<std::string, Mesh>* meshes, glm::vec3 pos, glm::vec3 diffuse, glm::vec3 specular, float diffStrength, float specStrength){
	pointLights.push_back(PointLight(pos, diffuse, specular, diffStrength, specStrength));
	num = pointLights.size();
	std::string index = "light" + std::to_string(num-1);
	(*meshes)[index] = Mesh::loadOBJ("../assets/resources/lightSource");
	(*meshes)[index].scale(glm::vec3(0.1f,0.1f,0.1f));
}
