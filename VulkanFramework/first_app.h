#pragma once

#include "Window.h"
#include "Pipeline.h"
#include "EngineDevice.h"
#include "EngineSwapChain.h"
#include "Model.h"
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
	void loadModels();
	void createPipelineLayout();
	void createPipeline();
	void createCommandBuffers();
	void drawFrame();
private:
	Window window{width, height, "Vulkan Framework"};
	EngineDevice device{window};
	EngineSwapChain engSwapChain{ device, window.getExtent() };
	std::unique_ptr<Pipeline> pipeline;
	VkPipelineLayout pipelineLayout;
	std::vector<VkCommandBuffer> commandBuffers;
	std::unique_ptr<Model> model;
};