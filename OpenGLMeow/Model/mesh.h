#ifndef MESH_H
#define MESH_H
#include <iostream>
#include <fstream>
#include <vector>

// Add the ability to import meshes/models (preferably .obj files maybe using assimp)

class Mesh {
public:
	Mesh(std::string fileLocation);
	~Mesh();

private:
	std::vector<float> vertices;
};

#endif