#pragma once

#include "Window.h"
#include "EngineDevice.h"
#include "EngineSwapChain.h"
#include <memory>
#include <cassert>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

class Renderer
{
public:
	Renderer(Window& window, EngineDevice& device);
	~Renderer();
	Renderer(const Renderer&) = delete;
	Renderer& operator=(const Renderer&) = delete;

	VkRenderPass getSwapChainRenderPass() const
	{
		return engSwapChain->getRenderPass();
	}
	bool isFrameInProgress() const
	{
		return isFrameStarted;
	}
	VkCommandBuffer getCurrentCommandBuffer() const
	{
		assert(isFrameStarted && "Cannot get command buffer when the frame is not in progress!");
		return commandBuffers[currentFrameIndex];
	}
	int getFrameIndex() const
	{
		assert(isFrameStarted && "Cannot get frame index when the frame is not in progress!");
		return currentFrameIndex;
	}

	VkCommandBuffer beginFrame();
	void endFrame();
	void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
	void endSwapChainRenderPass(VkCommandBuffer commandBuffer);
private:
	void createCommandBuffers();
	void freeCommandBuffers();
	void recreateSwapChain();
private:
	Window& window;
	EngineDevice& device;
	std::unique_ptr<EngineSwapChain> engSwapChain;
	std::vector<VkCommandBuffer> commandBuffers;
	uint32_t currentImageIndex;
	int currentFrameIndex{0};
	bool isFrameStarted = false;
};