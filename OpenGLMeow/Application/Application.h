#ifndef APPLICATION_H
#define APPLICATION_H

#include <iostream>
#include "../glad/glad.h"
#include <GLFW/glfw3.h>

#include "../Camera/Camera.h"
#include "../Model/mesh.h"

const int SCREEN_HEIGHT = 800;
const int SCREEN_WIDTH = 600;

class Application
{
	Application();
	~Application();
	
public:
	int Init();
	void InitCallbacks();

	void Run();

	void Update();
	void Render();

	void AddToRenderQueue(Mesh& mesh) {
		m_renderQueue.push_back(mesh);
	}

private:
	void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
	void MouseInputFunc(GLFWwindow* window, double xpos, double ypos);
	void ScrollInputFunc(GLFWwindow* window, double xoffset, double yoffset);

private:
	GLFWwindow* m_window;
	std::vector<Mesh> m_renderQueue;
	Camera* m_camera;

};

#endif

