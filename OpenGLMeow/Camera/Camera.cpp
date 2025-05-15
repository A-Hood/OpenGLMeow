#include "Camera.h"

Camera::Camera(unsigned int screenWidth, unsigned int screenHeight) :
	m_ScreenWidth(screenWidth),
	m_ScreenHeight(screenHeight)
{
};

Camera::~Camera()
{
};

glm::mat4 Camera::UpdateView(float deltaTime)
{
	float yaw = -90.0f;
	glm::vec3 direction;
	direction.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	direction.y = sin(glm::radians(m_Pitch));
	direction.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));

	return glm::lookAt(m_CameraPos, m_CameraPos + m_CameraFront, m_CameraUp);
}

glm::mat4 Camera::UpdatePerspective() {
	return glm::perspective(glm::radians(m_FOV), (float)m_ScreenWidth / (float)m_ScreenHeight, 0.1f, 100.0f);
}

void Camera::ProcessInput(GLFWwindow* window, float deltaTime)
{
	// camera movement
	float cameraSpeed = 5.0f * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		m_CameraPos += cameraSpeed * m_CameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		this->m_CameraPos -= cameraSpeed * m_CameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		this->m_CameraPos -= glm::normalize(glm::cross(m_CameraFront, m_CameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		m_CameraPos += glm::normalize(glm::cross(m_CameraFront, m_CameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		m_CameraPos += cameraSpeed * m_CameraUp;
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		m_CameraPos -= cameraSpeed * m_CameraUp;
}

void Camera::MouseInput(float deltaTime, double xpos, double ypos)
{
	if (m_FirstMouse)
	{
		m_LastX = xpos;
		m_LastY = ypos;
		m_FirstMouse = false;
	}

	float xoffset = xpos - m_LastX;
	float yoffset = m_LastY - ypos;
	m_LastX = xpos;
	m_LastY = ypos;

	float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	m_Yaw += xoffset;
	m_Pitch += yoffset;

	if (m_Pitch > 89.0f)
		m_Pitch = 89.0f;
	if (m_Pitch < -89.0f)
		m_Pitch = -89.0f;

	glm::vec3 direction;
	direction.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	direction.y = sin(glm::radians(m_Pitch));
	direction.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	m_CameraFront = glm::normalize(direction);
}

void Camera::MouseScrollInput(double xoffset, double yoffset)
{
	m_FOV -= (float)yoffset;
	if (m_FOV < 1.0f)
		m_FOV = 1.0f;
	if (m_FOV > 45.0f)
		m_FOV = 45.0f;
}