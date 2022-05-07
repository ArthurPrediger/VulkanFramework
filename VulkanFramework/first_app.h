#pragma once

#include "Window.h"
#include "Pipeline.h"

class First_App
{
public:
	static constexpr int width = 800;
	static constexpr int height = 600;

	void Run();

private:
	Window window{width, height, "Vulkan Framework"};
	Pipeline pipeline{ "Shaders/simple_shader.vert.spv", "Shaders/simple_shader.frag.spv" };
};