#include "Application.h"

Application::Application()
{
	Init();
}

Application::~Application()
{
}

void Application::Init()
{
	glfwInit();
	InitWindow();
	InitCamera(); // TODO: add the ability to add more camera

	SetInputMode();
}

int Application::InitWindow()
{
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
	glfwMakeContextCurrent(m_window);
	glfwSetWindowUserPointer(m_window, this);

	InitCallbacks();
	// Initiliase glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	glEnable(GL_DEPTH_TEST);
}

void Application::InitCamera()
{
	m_camera = new Camera(SCREEN_WIDTH, SCREEN_HEIGHT);
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

void Application::SetInputMode()
{
	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

int Application::Run()
{
	Shader ourShader("Shaders/v.glsl", "Shaders/f.glsl");

	// load and create a texture 
	// -------------------------
	unsigned int texture1;
	// texture 1
	// ---------
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	unsigned char* data = stbi_load("C:/Users/dabpo/Downloads/GqhdpJrW0AAlxBx.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	ourShader.use();
	ourShader.setInt("texture1", 0);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Load mesh
	Mesh newMesh("C:/Users/dabpo/Documents/ahhhh.obj");

	// render loop
	while (!glfwWindowShouldClose(m_window))
	{
		// Delta time
		float currentFrame = glfwGetTime();
		m_deltaTime = currentFrame - m_lastFrame;
		m_lastFrame = currentFrame;

		// input
		// -----
		ProcessInput(m_camera);

		// render
		// ------
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// bind textures on corresponding texture units
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		//glActiveTexture(GL_TEXTURE1);
		//glBindTexture(GL_TEXTURE_2D, texture2);

		ourShader.use();
		// Update Camera
		glm::mat4 view = m_camera->UpdateView(m_deltaTime);
		glm::mat4 projection = m_camera->UpdatePerspective(); // glm::mat4(1.0f);

		//glm::mat4 view = glm::mat4(1.0f);

		// pass transformation matrices to the shader
		ourShader.setMat4("projection", projection); // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
		ourShader.setMat4("view", view);


		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0, 0, -5));
		ourShader.setMat4("model", model);
		newMesh.Draw();

		glfwSwapBuffers(m_window);
		glfwPollEvents();
	}

	DeleteBuffers();
	glfwTerminate();
	return 0;
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

void Application::DeleteBuffers()
{
	for (unsigned int i = 0; i < m_renderQueue.size(); ++i) {
		m_renderQueue[i].DeleteBuffers();
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
	m_camera->MouseInput(m_deltaTime, xpos, ypos);
}

 void Application::ScrollInputFunc(GLFWwindow* window, double xoffset, double yoffset)
{
	m_camera->MouseScrollInput(xoffset, yoffset);
}

 void Application::ProcessInput(Camera* camera)
 {
	 if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		 glfwSetWindowShouldClose(m_window, true);

	 camera->ProcessInput(m_window, m_deltaTime);
 }