#pragma once

#include "Window.h"
#include "Pipeline.h"
#include "EngineDevice.h"
#include "EngineSwapChain.h"
#include <memory>

class FirstApp
{
public:
	static constexpr int width = 800;
	static constexpr int height = 600;

	FirstApp();
	~FirstApp();
	FirstApp(const FirstApp&) = delete;
	FirstApp& operator=(const FirstApp&) = delete;
	void run();
private:
	void createPipelineLayout();
	void createPipeline();
	void createCommandBuffers();
	void drawFrame();
private:
	Window window{width, height, "Vulkan Framework"};
	EngineDevice engDevice{window};
	EngineSwapChain engSwapChain{ engDevice, window.getExtent() };
	std::unique_ptr<Pipeline> pipeline;
	VkPipelineLayout pipelineLayout;
	std::vector<VkCommandBuffer> commandBuffers;
};