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
	bool ShouldClose() const 
	{
		return glfwWindowShouldClose(window);
	} 
	void CreateWindowSurface(VkInstance instance, VkSurfaceKHR* surface);
private:
	void InitWindow();
private:
	GLFWwindow* window;
	std::string windowName;
	int width;
	int height;
};