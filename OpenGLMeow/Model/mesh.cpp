#include "mesh.h"
#include <ios>

#include <cstddef>
std::vector<std::string> split(std::string line, std::string delimiter) { // from GetIntoGameDev

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

	std::vector<glm::vec3> temp_vertices;

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

		// read line, dependant on type read vertex/indices
		while (std::getline(objFile, currentText))
		{
			std::istringstream _stringStream(currentText);
			_stringStream >> type;

			// read vertex
			if (type == "v")
			{
				_stringStream >> x;
				_stringStream >> y;
				_stringStream >> z;

				temp_vertices.push_back(glm::vec3(x, y, z));
			}

			// read indices
			else if (type == "f")
			{
				std::string string;
				for (int i = 0; i < 3; i++)
				{
					_stringStream >> string;
					std::vector<std::string> line;

					line = split(string, "/");

					int indice = std::stoi(line[0]);

					indices.push_back(indice);
				}
			}
		}
	}
	else
	{
		std::cout << "File is not good." << std::endl;
	}

	for (unsigned int i = 0; i < indices.size(); i++) {
		unsigned int vertexIndex = indices[i];
		glm::vec3 vertex = temp_vertices[vertexIndex - 1];
		vertices.push_back(vertex);
	}

	isReadyToDraw = true;



	// Create OpenGL Buffers
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_EBO);

	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

Mesh::~Mesh(){
}

void Mesh::OutputVertices()
{
	for (int i = 0; i < vertices.size() - 1; i++)
	{
		//std::cout << vertices[i] << " ";
	}
	//std::cout << vertices.size() / 3 << std::endl;
}

void Mesh::OutputIndices()
{
	for (int i = 0; i < indices.size() - 1; i++)
	{
		std::cout << indices[i] << " ";
	}
	//std::cout << indices.size() << std::endl;
}

void Mesh::Draw()
{
	if (isReadyToDraw)
	{
		glBindVertexArray(m_VAO);
		//glDrawElements(GL_LINE, vertices.size(), GL_UNSIGNED_INT, 0);
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());
	}
}