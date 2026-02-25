#include "../../include/core/Camera.hpp"
#include <iostream>

void printMat4(const glm::mat4& matrix) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            std::cout << matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }
}
void printVec3(const glm::vec3& vector) {
    std::cout << "X: " << vector.x << ", Y: " << vector.y << ", Z: " << vector.z << std::endl;
}
void Camera::matrixCalc(ShaderManager* shader, int width, int height, bool line){
	float aspect = (float)width/(float)height;
	Proj = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 1000.0f);
	
	dir.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	dir.y = sin(glm::radians(pitch));
	dir.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	glm::vec3 cDir = dir;
	//std::cout << "dir: " << dir.x << ", " << dir.y << ", " << dir.z << std::endl;
	front = glm::normalize(cDir);
	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
	cDir.y = 0;
	//playerFront = glm::normalize(cDir);
	

	View = glm::lookAt(
		position,
		position + front,
		up
	);

	
	printMat4(View);
	printMat4(Proj);
	if(!line){
		printVec3(position);
		shader->sendVec3("viewPos", position);
	}
	shader->sendMatrix4("view", View);
	shader->sendMatrix4("proj", Proj);
}
