#include "Application.h"

Application::Application()
{
	Init();
}

Application::~Application()
{
}

int Application::Init()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Initialise window
	m_window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "MeowGL", NULL, NULL);
	if (m_window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwSetWindowUserPointer(m_window, this);

	// Initialise camera
	m_camera = new Camera(SCREEN_WIDTH, SCREEN_HEIGHT);

	glfwMakeContextCurrent(m_window);

	// Initliase callbacks
	InitCallbacks();

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
}

void Application::InitCallbacks()
{
	glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* window, auto width, auto height) {
		auto app = reinterpret_cast<Application*>(glfwGetWindowUserPointer(window));
		glViewport(0, 0, width, height);
		});
	glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xpos, double ypos) {
		auto app = reinterpret_cast<Application*>(glfwGetWindowUserPointer(window));
		app->MouseInputFunc(window, xpos, ypos);
		});
	glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xoffset, double yoffset) {
		auto app = reinterpret_cast<Application*>(glfwGetWindowUserPointer(window));
		app->ScrollInputFunc(window, xoffset, yoffset);
		});
}


void Application::Run()
{
	// run update loop (using delta time)
	// run render queue


}

void Application::Update()
{
	// run update on input and stuff here
}

void Application::Render()
{
	for (unsigned int i = 0; i < m_renderQueue.size(); ++i) {
		m_renderQueue[i].Draw();
	}
}

void Application::FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

void Application::MouseInputFunc(GLFWwindow* window, double xpos, double ypos)
{
	//float deltaTime;
	//m_camera->MouseInput(deltaTime, xpos, ypos);
}

 void Application::ScrollInputFunc(GLFWwindow* window, double xoffset, double yoffset)
{
	m_camera->MouseScrollInput(xoffset, yoffset);
}