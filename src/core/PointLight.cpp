#include "../../include/core/PointLight.hpp"

PointLight::PointLight(glm::vec3 pos, glm::vec3 diffuse, glm::vec3 specular, float diffStrength, float specStrength):
	pos(pos), diffuse(diffuse), specular(specular),
	diffStrength(diffStrength),specStrength(specStrength){}

void PointLight::update(ShaderManager shader, int lightNum){
	std::string lightIndex = "pointLights[" + std::to_string(lightNum) + "].";
	shader.sendVec3((lightIndex + "pos").c_str(), pos);
	shader.sendVec3((lightIndex + "diffuse").c_str(), diffuse);
	shader.sendVec3((lightIndex + "specular").c_str(), specular);
	shader.sendFloat((lightIndex + "diffStrength").c_str(), diffStrength);
	shader.sendFloat((lightIndex + "specStrength").c_str(), specStrength);
}
