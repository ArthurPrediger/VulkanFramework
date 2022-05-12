#pragma once

#include "Window.h"
#include "Pipeline.h"
#include "EngineDevice.h"
#include "EngineSwapChain.h"
#include "GameObject.h"
#include <memory>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

class FirstApp
{
	struct SimplePushConstantData
	{
		glm::mat2 transform{1.0f};
		glm::vec2 offset;
		alignas(16) glm::vec3 color;
	};
public:
	static constexpr int width = 800;
	static constexpr int height = 600;

	FirstApp();
	~FirstApp();
	FirstApp(const FirstApp&) = delete;
	FirstApp& operator=(const FirstApp&) = delete;
	void run();
private:
	void loadGameObjects();
	void createPipelineLayout();
	void createPipeline();
	void createCommandBuffers();
	void freeCommandBuffers();
	void drawFrame();
	void recreateSwapChain();
	void recordCommandBuffer(int imageIndex);
	void renderGameObjects(VkCommandBuffer commandBuffer);
private:
	Window window{width, height, "Vulkan Framework"};
	EngineDevice device{window};
	std::unique_ptr<EngineSwapChain> engSwapChain;
	std::unique_ptr<Pipeline> pipeline;
	VkPipelineLayout pipelineLayout;
	std::vector<VkCommandBuffer> commandBuffers;
	std::vector<GameObject> gameObjects;
};