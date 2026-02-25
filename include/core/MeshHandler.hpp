#pragma once

#include <cstdio>
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include "TextureLoader.hpp"
#include "../graphics/Shader.hpp"
#include <map>
#include <array>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstring>
#include <algorithm>
#include <iostream>
#include <limits>

using namespace std;

class Mesh {
    private:
        struct Vertex{
			float position[3];
			float texcoord[2];
			float normal[3];
			unsigned int faces[3];

			bool operator==(const Vertex& other) const {
				return memcmp(this, &other, sizeof(Vertex)) == 0;
			}
		};
        glm::mat4 Model;
        glm::mat4 originalModel;
        glm::vec3 translation;
        glm::vec3 scaleVec;
        float rotationRads;
        glm::vec3 rotationAxis;
        vector<glm::vec3> localSpaceAABB;
        array<glm::vec2, 3> MaximumMinimums;
        vector<Vertex> vertices;
        vector<unsigned int> indices;
        GLuint vao, vbo, ebo, textureID;

        void setupMesh();

    public:
        Mesh();
        Mesh(vector<Vertex> vertices, vector<unsigned int> indices, GLuint textureID, vector<glm::vec3> localAABB);
        static Mesh loadOBJ(const string& filename);
        void translate(glm::vec3 translationVec);
        void scale(glm::vec3 scaleVec2);
        void rotate(float rotationInDegrees, glm::vec3 rotAxis);
        void calculateModel();
        glm::vec3 getMiddleVector();
        void calculateWorldSpaceMaxMins();
        array<glm::vec2, 3> getWorldSpaceMaxMins();
        void draw(ShaderManager* shader);
};

