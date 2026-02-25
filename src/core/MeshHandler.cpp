#include "../../include/core/MeshHandler.hpp"
void printMat(const glm::mat4& matrix) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            std::cout << matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

void Mesh::setupMesh() {
	glGenVertexArrays(1, &vao);

	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	// Vertex positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(0);

	// Texture coordinates
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// normals
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(5 * sizeof(float))); //skip 5floats, 3 for pos 2 for texCoords
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	// Validate texture loading
	if (textureID == 0) {
		// Texture loading failed
		std::cerr << "Error: Texture loading failed for mesh." << std::endl;
	}
}

// Constructor
Mesh::Mesh() : vao(0), vbo(0), ebo(0), textureID(0) {}

Mesh::Mesh(vector<Vertex> vertices, vector<unsigned int> indices, GLuint textureID, vector<glm::vec3> localAABB):
	vertices(vertices), indices(indices), textureID(textureID), localSpaceAABB(localAABB){

	Model = glm::mat4(1.0f);  // Identity matrix
	translation = glm::vec3(0.0f);
	scaleVec = glm::vec3(1,1,1);
	rotationRads = 0.0f;	rotationAxis = glm::vec3(1,0,0);
    originalModel = glm::mat4(1.0f);  // Identity matrix
	
	setupMesh();
	/*
	cout << "\nVertices:" << endl;
	for (const Vertex& vert : vertices) {
		cout << "Position: (" << vert.position[0] << ", " << vert.position[1] << ", " << vert.position[2] << ")";
		cout << " TexCoord: (" << vert.texcoord[0] << ", " << vert.texcoord[1] << ")";
		cout << " Normal: (" << vert.normal[0] << ", " << vert.normal[1] << ", " << vert.normal[2] << ")" << endl;
	}
	cout << "Indices:" << endl;
	for (unsigned int index : indices) {
		cout << index << " ";
	}
	cout << endl;
	*/
}

Mesh Mesh::loadOBJ(const string& filename) {
	ifstream file(filename + ".obj");
	if (!file.is_open()) {
		cerr << "Failed to open " << filename << ".obj" << endl;
		exit(EXIT_FAILURE);
	}
	string line;
	vector<Vertex> out_vertices;
	vector<unsigned int> out_indices;
	vector<glm::vec3> out_faces;

	vector<glm::vec3> temp_vertices;
	vector<glm::vec2> temp_texcoords;
	vector<glm::vec3> temp_normals;

	glm::vec2 xPoints(INFINITY, -INFINITY);
	glm::vec2 yPoints(INFINITY, -INFINITY);
	glm::vec2 zPoints(INFINITY, -INFINITY);

	while(getline(file, line)) {
		if(line.substr(0, 2) == "v ") {
			glm::vec3 vertex;
			sscanf(line.c_str(), "v %f %f %f", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);
		}else if(line.substr(0, 3) == "vt ") {
			glm::vec2 texcoord;
			sscanf(line.c_str(), "vt %f %f", &texcoord.x, &texcoord.y);
			temp_texcoords.push_back(texcoord);
		}else if(line.substr(0, 3) == "vn ") {
			glm::vec3 normal;
			sscanf(line.c_str(), "vn %f %f %f", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
		}else if(line.substr(0, 2) == "f ") {
			unsigned int vIndices[3], tIndices[3], nIndices[3];
			const char* ch = line.c_str();
			sscanf(ch, "f %d/%d/%d %d/%d/%d %d/%d/%d",
				&vIndices[0], &tIndices[0], &nIndices[0], 
				&vIndices[1], &tIndices[1], &nIndices[1], 
				&vIndices[2], &tIndices[2], &nIndices[2]);

			for(int i = 0; i < 3; i++){
				Vertex vert;
				vert.position[0] = temp_vertices[vIndices[i] - 1].x;
				vert.position[1] = temp_vertices[vIndices[i] - 1].y;
				vert.position[2] = temp_vertices[vIndices[i] - 1].z;

				if(vert.position[0] < xPoints[0]){
					xPoints[0] = vert.position[0];
				}else if(vert.position[0] > xPoints[1]){
					xPoints[1] = vert.position[0];
				}
				if(vert.position[1] < yPoints[0]){
					yPoints[0] = vert.position[1];
				}else if(vert.position[1] > yPoints[1]){
					yPoints[1] = vert.position[1];
				}
				if(vert.position[2] < zPoints[0]){
					zPoints[0] = vert.position[2];
				}else if(vert.position[2] > zPoints[1]){
					zPoints[1] = vert.position[2];
				}
				vert.texcoord[0] = temp_texcoords[tIndices[i] - 1].x;
				vert.texcoord[1] = -temp_texcoords[tIndices[i] - 1].y;
				vert.normal[0] = temp_normals[nIndices[i] - 1].x;
				vert.normal[1] = temp_normals[nIndices[i] - 1].y;
				vert.normal[2] = temp_normals[nIndices[i] - 1].z;

				// iterate through out_verts and see if vert exists
				auto itr = find(out_vertices.begin(), out_vertices.end(), vert);
				
				if(itr != out_vertices.end()){
					// if it does reuse index
					out_indices.push_back(distance(out_vertices.begin(), itr));
				}else{
					// new vert
					out_vertices.push_back(vert);
					out_indices.push_back(out_vertices.size() - 1);
				}
				glm::vec3 temp_face(vIndices[i], tIndices[i], nIndices[i]);
				out_faces.push_back(temp_face);
			}
		}
	}
	string mtlPath = filename + ".mtl";
	GLuint temp_texture_ID = LoadTextureFromMTL(mtlPath);

	vector<glm::vec3> tempAABB;

	for(int i = 0; i < 8; i++){
		tempAABB.push_back(glm::vec3(
			(i & 1) ? xPoints[1] : xPoints[0],
			(i & 2) ? yPoints[1] : yPoints[0],
			(i & 4) ? zPoints[1] : zPoints[0]));
	}
	return Mesh(out_vertices, out_indices, temp_texture_ID, tempAABB);
}

void Mesh::translate(glm::vec3 translationVec) {
	translation = translationVec;
}

void Mesh::scale(glm::vec3 scaleVec2) {
	scaleVec = scaleVec2;
}

void Mesh::rotate(float rotationInDegrees, glm::vec3 rotAxis) {
	rotationRads = glm::radians(rotationInDegrees);
	rotationAxis = rotAxis;
}

void Mesh::calculateModel() {
	Model = originalModel;
	Model = glm::translate(Model, translation);
	Model = glm::scale(Model, scaleVec);
	Model = glm::rotate(Model, rotationRads, rotationAxis);
}

glm::vec3 Mesh::getMiddleVector() {
	std::array<glm::vec2, 3> maxMins = getWorldSpaceMaxMins();
	for(int i = 0; i < 3; i++){
		//std::cout << "maxmins: " << maxMins[i].x << ", " << maxMins[i].y << std::endl;
	}
	glm::vec3 centerB = 0.5f * glm::vec3(
		maxMins[0][0]+maxMins[0][1],
		maxMins[1][0]+maxMins[1][1],
		maxMins[2][0]+maxMins[2][1]
	);
	return centerB;
}

void Mesh::calculateWorldSpaceMaxMins() {
	calculateModel();
    glm::vec2 xPoints(INFINITY, -INFINITY);
	glm::vec2 yPoints(INFINITY, -INFINITY);
	glm::vec2 zPoints(INFINITY, -INFINITY);

	for(int i = 0; i < 8; i++){
		glm::vec3 temp_vec;
		temp_vec = Model * glm::vec4(localSpaceAABB[i], 1.0f);
		if(temp_vec[0] < xPoints[0]){
			xPoints[0] = temp_vec[0];
		}else if(temp_vec[0] > xPoints[1]){
			xPoints[1] = temp_vec[0];
		}
		if(temp_vec[1] < yPoints[0]){
			yPoints[0] = temp_vec[1];
		}else if(temp_vec[1] > yPoints[1]){
			yPoints[1] = temp_vec[1];
		}
		if(temp_vec[2] < zPoints[0]){
			zPoints[0] = temp_vec[2];
		}else if(temp_vec[2] > zPoints[1]){
			zPoints[1] = temp_vec[2];
		}
	}

	MaximumMinimums[0] = xPoints;
	MaximumMinimums[1] = yPoints;
	MaximumMinimums[2] = zPoints;
}

array<glm::vec2, 3> Mesh::getWorldSpaceMaxMins() {
	calculateWorldSpaceMaxMins();
	return MaximumMinimums;
}
void printActiveUniforms(GLuint program) {
    GLint numUniforms = 0;
    glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &numUniforms);

    std::cout << "Number of active uniforms: " << numUniforms << std::endl;

    for (int i = 0; i < numUniforms; ++i) {
        GLsizei bufSize = 256;
        GLsizei length = 0;
        GLint size = 0;
        GLenum type = 0;
        GLchar uniformName[256];

        glGetActiveUniform(program, i, bufSize, &length, &size, &type, uniformName);
        GLint location = glGetUniformLocation(program, uniformName);

        std::cout << "Uniform #" << i << " Name: " << uniformName << " Type: " << type << " Size: " << size
                  << " Location: " << location << std::endl;
    }
}
void Mesh::draw(ShaderManager* shader) {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);

	calculateModel();
	shader->sendMatrix4("model", Model);
	shader->sendInt("textureSampler", 0);

	//printActiveUniforms(shader->getShaderID());
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

