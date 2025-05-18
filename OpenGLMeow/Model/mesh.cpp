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
	std::vector<glm::vec2> temp_vertices_uv;


	std::vector<unsigned int> temp_indice;
	std::vector<unsigned int> temp_indice_uv;



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

			else if (type == "vt")
			{
				_stringStream >> x;
				_stringStream >> y;

				temp_vertices_uv.push_back(glm::vec2(x, y));
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

					unsigned int indice = std::stoi(line[0]);
					unsigned int indice_uv = std::stoi(line[1]);

					temp_indice.push_back(indice);
					temp_indice_uv.push_back(indice_uv);

				}
			}
		}
	}
	else
	{
		std::cout << "File is not good." << std::endl;
	}


	// Set geometric vertexes
	for (unsigned int i = 0; i < temp_indice.size(); i++) {
		unsigned int vertexIndex = temp_indice[i];
		glm::vec3 vertex = temp_vertices[vertexIndex - 1];
		m_Vertices.push_back(vertex);
	}
	// Set UV vertexes
	for (unsigned int i = 0; i < temp_indice_uv.size(); i++) {
		unsigned int vertexIndexUV = temp_indice_uv[i];
		glm::vec2 vertex = temp_vertices_uv[vertexIndexUV - 1];
		m_Vertices_uv.push_back(vertex);
	}

	// Load into one data file that stores
	// - Position
	// - UV
	// - Normals
	for (unsigned int i = 0; i < m_Vertices.size(); i++)
	{
		// Position
		m_Data.push_back(m_Vertices[i].x);
		m_Data.push_back(m_Vertices[i].y);
		m_Data.push_back(m_Vertices[i].z);
		// UV
		m_Data.push_back(m_Vertices_uv[i].x);
		m_Data.push_back(m_Vertices_uv[i].y);
	}

	// Create OpenGL Buffers
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_EBO);

	glBindVertexArray(m_VAO);

	// Position
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, m_Data.size() * sizeof(float), &m_Data[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	// UV attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	isReadyToDraw = true;
}

Mesh::~Mesh(){
}

void Mesh::OutputVertices()
{
	std::cout << m_Vertices.size() << std::endl;
}

void Mesh::OutputIndices()
{
	std::cout << m_Vertices_uv.size() << std::endl;

	std::cout << m_Data.size() << std::endl;

}

void Mesh::Draw()
{
	if (isReadyToDraw)
	{
		glBindVertexArray(m_VAO);
		glDrawArrays(GL_TRIANGLES, 0, m_Data.size());
	}
}