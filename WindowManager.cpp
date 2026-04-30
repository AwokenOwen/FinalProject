#include "WindowManager.h"
#include "InputManager.h"
#include <ext/matrix_clip_space.hpp>

#include "Material.h"
#include "MeshRenderer.h"
#include "Object.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

int m_windowWidth;
int m_windowHeight;
int m_viewportWidth;
int m_viewportHeight;
bool fixedAspect;
float aspectRatio;

//Initialize the Window manager and call the create window function
int WindowManager::initialize()
{
	glfwInit();
	createWindow();
	setMaximized();
	fixedAspect = false;
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	if (Input.initialize(p_window))
	{
		cout << "InputManager failed to start\n";
	}
	cout << "InputManager started\n";

	return 0;
}

//free any data on the heap
void WindowManager::terminate()
{
}

//Singleton get function
WindowManager& WindowManager::getInstance()
{
	static WindowManager single;
	return single;
}

//get function for the private window variable
GLFWwindow* WindowManager::getWindow()
{
	if (p_window == nullptr)
		createWindow();
	return p_window;
}

//get functions for the width and height of the screen
int WindowManager::getWindowWidth()
{
	return m_windowWidth;
}
int WindowManager::getWindowHeight()
{
	return m_windowHeight;
}

int WindowManager::getViewportWidth() {
	return m_viewportWidth;
}

int WindowManager::getViewportHeight() {
	return m_viewportHeight;
}

//Clear the color and depth buffers
void WindowManager::clear()
{
	//rendering commands 
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void WindowManager::swap() const {
	glfwSwapBuffers(p_window);
	glfwPollEvents(); 
}

void WindowManager::setMaximized(const bool maximized) const {
	if (maximized)
	{
		glfwMaximizeWindow(p_window);
	}
	else {
		glfwRestoreWindow(p_window);
	}
}

mat4 WindowManager::getPerspectiveMatrix()
{
	return glm::perspective(glm::radians(60.0f), static_cast<float>(Window.getViewportWidth()) / static_cast<float>(Window.getViewportHeight()), 0.1f, 1000.0f);;
}

void WindowManager::setFixedAspect(bool fixed)
{
	fixedAspect = fixed;
}

void WindowManager::setAspectRatio(float aspect)
{
	aspectRatio = aspect;
}

void WindowManager::activateFrameBuffer() {
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // we're not using the stencil buffer now
	glEnable(GL_DEPTH_TEST);
}

void WindowManager::deactivateFrameBuffer() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

}

unsigned int WindowManager::getTextureColorbuffer() const {
	return textureColorbuffer;
}

void WindowManager::deleteFrameBuffer() {
	glDeleteRenderbuffers(1, &rbo);
	glDeleteFramebuffers(1, &framebuffer);
	glDeleteTextures(1, &textureColorbuffer);
}

//Private constructor for singleton functionality
WindowManager::WindowManager()
= default;

//creates the window by grabbing the primary monitor setting the window to be 
//"windowed fullscreen", then initializing glad, then finally creating the window
int WindowManager::createWindow()
{
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();

	const GLFWvidmode* mode = glfwGetVideoMode(monitor);

	glfwWindowHint(GLFW_RED_BITS, mode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

	m_windowWidth = mode->width;
	m_windowHeight = mode->height;
	m_viewportWidth = m_windowWidth;
	m_viewportHeight = m_windowHeight;

	p_window = glfwCreateWindow(m_windowWidth, m_windowHeight, "Game Engine", nullptr, nullptr);

	glfwSetInputMode(p_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwMakeContextCurrent(p_window);
	// Disable VSync to uncap framerate
	glfwSwapInterval(0);


	if (p_window == nullptr)
	{
		cout << "Failed to create GLFW window\n";
		glfwTerminate();
		return 0;
	}
	glfwMakeContextCurrent(p_window);

	//NOTE: Init GLAD
	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//NOTE: Set OpenGL Viewport
	glViewport(0, 0, m_windowWidth, m_windowHeight);

	createFrameBuffer();

	glfwSetFramebufferSizeCallback(p_window, framebuffer_size_callback);

	return 0;
}

void WindowManager::createFrameBuffer() {
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	// create a color attachment texture
	glGenTextures(1, &textureColorbuffer);
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_viewportWidth, m_viewportHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
	// create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_viewportWidth, m_viewportHeight); // use a single renderbuffer object for both a depth AND stencil buffer.
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
	// now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now

	auto fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete: " << fboStatus << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	if (frameBufferObject != nullptr) {
		sendFrameBufferTexture();
	}
}

void WindowManager::setFrameBufferObject(Object *object) {
	frameBufferObject = object;
}

void WindowManager::sendFrameBufferTexture() {
	frameBufferObject->getComponent<MeshRenderer>()->getMaterials()[0]->setTexture(textureColorbuffer);
}

void framebuffer_size_callback(GLFWwindow* window, const int width, const int height)
{
	Window.deleteFrameBuffer();
	m_windowWidth = width;
	m_windowHeight = height;
	if (fixedAspect)
	{
		if (static_cast<float>(width) / static_cast<float>(height) > aspectRatio)
		{
			m_viewportHeight = height;
			m_viewportWidth = static_cast<int>(m_windowHeight * aspectRatio);
		}
		else if (static_cast<float>(width) / static_cast<float>(height) < aspectRatio)
		{
			m_viewportWidth = width;
			m_viewportHeight = static_cast<int>(m_windowWidth * (1.0f/aspectRatio));
		}
		else
		{
			m_viewportWidth = width;
			m_viewportHeight = height;
		}
	}
	else
	{
		m_viewportWidth = width;
		m_viewportHeight = height;
	}
	glViewport(static_cast<int>((width - m_viewportWidth) / 2.0f), static_cast<int>((height - m_viewportHeight) / 2.0f), m_viewportWidth, m_viewportHeight);
	Window.createFrameBuffer();
}
