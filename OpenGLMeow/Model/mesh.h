#ifndef MESH_H
#define MESH_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "../glad/glad.h"
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
// Add the ability to import meshes/models (preferably .obj files maybe using assimp)

class Mesh {
public:
	Mesh(std::string fileLocation);
	~Mesh();

	void Draw();

	void OutputVertices();
	void OutputIndices();

	unsigned int m_VAO;
	unsigned int m_VBO;
	unsigned int m_EBO;

private:
	//std::vector<glm::vec3> vertices;
	std::vector<float> vertices;
	std::vector<unsigned int> indices;

	bool isReadyToDraw = false;
};

#endif