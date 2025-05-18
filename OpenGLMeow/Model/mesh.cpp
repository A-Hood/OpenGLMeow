#include "mesh.h"
#include <ios>

#include <cstddef>
std::vector<std::string> split(std::string line, std::string delimiter) {

	std::vector<std::string> splitLine;

	size_t pos = 0;
	std::string token;
	while ((pos = line.find(delimiter)) != std::string::npos) {
		token = line.substr(0, pos);
		splitLine.push_back(token);
		line.erase(0, pos + delimiter.length());
	}
	splitLine.push_back(line);

	return splitLine;
}

Mesh::Mesh(std::string modelName){

	std::ifstream objFile(modelName);
	std::string currentText;

	int numberOfLines = 0;

	// check if file is open
	if (objFile.is_open()) {
		std::getline(objFile, currentText);
	}
	else
	{
		std::cout << "Could not read input file." << std::endl;
	}

	if (objFile.good()) {
		float x, y, z;
		std::string type;

		unsigned int currentIndice = 0;
		while (std::getline(objFile, currentText))
		{
			std::istringstream _stringStream(currentText);
			_stringStream >> type;

			// TODO: function that loads vertexes (geometric only for now)
			if (type == "v")
			{
				_stringStream >> x;
				_stringStream >> y;
				_stringStream >> z;

				//glm::vec3 test = glm::vec3(x, y, z);

				vertices.push_back(x);
				vertices.push_back(y);
				vertices.push_back(z);
			}

			// TODO: function that loads indices (geometric only for now)
			if (type == "f")
			{
				std::string string;
				_stringStream >> string;
				std::vector<std::string> line;

				line = split(string, "/");

				int indice = std::stoi(line[0]);
				indices.push_back(indice);
				//glm::vec3 pos = vertices2[indice - 1];

				//std::cout << pos[0] << std::endl;
				//vertices.push_back(pos[0]);
			}
		}
	}
	else
	{
		std::cout << "File is not good." << std::endl;
	}

	isReadyToDraw = true;

	// Create OpenGL Buffers
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_EBO);

	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
}

Mesh::~Mesh(){
}

void Mesh::OutputVertices()
{
	//for (int i = 0; i < vertices.size() - 1; i++)
	//{
		//std::cout << vertices[i] << std::endl;
	//}
	std::cout << vertices.size() / 3 << std::endl;
}

void Mesh::OutputIndices()
{
	//for (int i = 0; i < indices.size() - 1; i++)
	//{
		//std::cout << indices[i] << std::endl;
	//}
	std::cout << indices.size() << std::endl;
}

void Mesh::Draw()
{
	if (isReadyToDraw)
	{
		glBindVertexArray(m_VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
		glDrawElements(GL_TRIANGLES, vertices.size() - 1, GL_UNSIGNED_INT, 0);
		//glDrawArrays(GL_POLYGON, 0, vertices.size() - 1);
	}
}