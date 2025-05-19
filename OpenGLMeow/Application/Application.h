#ifndef APPLICATION_H
#define APPLICATION_H

#include <iostream>
#include "../glad/glad.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <math.h>
#include "../stb_image/stb_image.h"

#include "../Shaders/Shader.h"
#include "../Camera/Camera.h"
#include "../Model/mesh.h"


const int SCREEN_HEIGHT = 1080;
const int SCREEN_WIDTH = 1920;

class Application
{
public:
	Application();
	~Application();
	
	void Init();
	int InitWindow();
	void InitCamera();
	void InitCallbacks();

	void SetInputMode(); // just make it cursor for now
	void ProcessInput(Camera* camera);

	int Run();

	void Update();
	void Render();

	void AddToRenderQueue(Mesh& mesh) {
		m_renderQueue.push_back(mesh);
	}

	void DeleteBuffers();

private:
	void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
	void MouseInputFunc(GLFWwindow* window, double xpos, double ypos);
	void ScrollInputFunc(GLFWwindow* window, double xoffset, double yoffset);

private:
	GLFWwindow* m_window;
	std::vector<Mesh> m_renderQueue;
	Camera* m_camera;

	float m_deltaTime = 0.0f;	// Time between current frame and last frame
	float m_lastFrame = 0.0f; // Time of last frame
};

#endif

