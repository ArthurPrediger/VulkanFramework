#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <string>

class Window
{
public:
	Window(int w, int h, const std::string& name);
	~Window();
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
	bool shouldClose() const 
	{
		return glfwWindowShouldClose(window);
	} 
	VkExtent2D getExtent() const 
	{
		return { static_cast<uint32_t>(width), static_cast<uint32_t>(height) };
	};
	bool wasWindowResized() const
	{
		return frameBufferResized;
	}
	void resetWindowResized()
	{
		frameBufferResized = false;
	}
	void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);
private:
	static void frameBufferResizeCallback(GLFWwindow* window, int width, int height);
	void initWindow();
private:
	GLFWwindow* window;
	std::string windowName;
	int width;
	int height;
	bool frameBufferResized = false;
};