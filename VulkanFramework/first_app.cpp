#pragma once

#include "first_app.h"
#include <stdexcept>
#include <array>

FirstApp::FirstApp()
{
	loadGameObjects();
	createPipelineLayout();
	recreateSwapChain();
	createCommandBuffers();
}

FirstApp::~FirstApp()
{
	vkDestroyPipelineLayout(device.device(), pipelineLayout, nullptr);
}

void FirstApp::run()
{
	while (!window.shouldClose())
	{
		glfwPollEvents();
		drawFrame();
	}

	vkDeviceWaitIdle(device.device());
}

void FirstApp::loadGameObjects()
{
	std::vector<Model::Vertex> vertices {
		{{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
		{{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
		{{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
	};

	auto model = std::make_shared<Model>(device, vertices);

	auto triangle = GameObject::createGameObject();
	triangle.model = model;
	triangle.color = { 0.1f, 0.8f, 0.1f };
	triangle.transform2d.translation.x = 0.2f;
	triangle.transform2d.scale = { 0.5f, 0.2f };
	triangle.transform2d.rotation = 0.5f * glm::pi<float>();

	gameObjects.push_back(std::move(triangle));
}

void FirstApp::createPipelineLayout()
{
	VkPushConstantRange pushConstantRange{};
	pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
	pushConstantRange.offset = 0;
	pushConstantRange.size = sizeof(SimplePushConstantData);

	VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 0;
	pipelineLayoutInfo.pSetLayouts = nullptr;
	pipelineLayoutInfo.pushConstantRangeCount = 1;
	pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;

	if (vkCreatePipelineLayout(device.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create pipelineLayout!");
	}
}

void FirstApp::createPipeline()
{
	assert(engSwapChain != nullptr && "Cannot create pipeline before swap chain!");
	assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout!");

	PipelineConfigInfo pipelineConfig{};
	Pipeline::defaultPipelineConfigInfo(pipelineConfig);
	pipelineConfig.renderPass = engSwapChain->getRenderPass();
	pipelineConfig.pipelineLayout = pipelineLayout;
	pipeline = std::make_unique<Pipeline>(
		device,
		"Shaders/simple_shader.vert.spv",
		"Shaders/simple_shader.frag.spv",
		pipelineConfig);
}

void FirstApp::createCommandBuffers()
{
	commandBuffers.resize(engSwapChain->imageCount());

	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = device.getCommandPool();
	allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

	if (vkAllocateCommandBuffers(device.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to allocate command buffers!");
	}
}

void FirstApp::freeCommandBuffers()
{
	vkFreeCommandBuffers(
		device.device(),
		device.getCommandPool(),
		static_cast<uint32_t>(commandBuffers.size()),
		commandBuffers.data());
	commandBuffers.clear();
}

void FirstApp::drawFrame()
{
	uint32_t imageIndex;
	auto result = engSwapChain->acquireNextImage(&imageIndex);

	if (result == VK_ERROR_OUT_OF_DATE_KHR)
	{
		recreateSwapChain();
		return;
	}

	if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
	{
		throw std::runtime_error("Failed to acquire swap chain image!");
	}

	recordCommandBuffer(imageIndex);
	result = engSwapChain->submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);

	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR ||
		window.wasWindowResized())
	{
		window.resetWindowResized();
		recreateSwapChain();
		return;
	}

	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to present swap chain image!");
	}
}

void FirstApp::recreateSwapChain()
{
	auto extent = window.getExtent();
	while (extent.width == 0 || extent.height == 0)
	{
		extent = window.getExtent();
		glfwWaitEvents();
	}

	vkDeviceWaitIdle(device.device());

	if (engSwapChain == nullptr)
	{
		engSwapChain = std::make_unique<EngineSwapChain>(device, extent);
	}
	else
	{
		engSwapChain = std::make_unique <EngineSwapChain>(device, extent, std::move(engSwapChain));
		if (engSwapChain->imageCount() != commandBuffers.size())
		{
			freeCommandBuffers();
			createCommandBuffers();
		}
	}
	createPipeline();
}

void FirstApp::recordCommandBuffer(int imageIndex)
{
	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

	if (vkBeginCommandBuffer(commandBuffers[imageIndex], &beginInfo) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to begin recording command buffer!");
	}

	VkRenderPassBeginInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = engSwapChain->getRenderPass();
	renderPassInfo.framebuffer = engSwapChain->getFrameBuffer(imageIndex);

	renderPassInfo.renderArea.offset = { 0, 0 };
	renderPassInfo.renderArea.extent = engSwapChain->getSwapChainExtent();

	std::array<VkClearValue, 2> clearValues{};
	clearValues[0].color = { 0.1f, 0.1f, 0.1f, 1.0f };
	clearValues[1].depthStencil = { 1.0f, 0 };
	renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
	renderPassInfo.pClearValues = clearValues.data();

	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = static_cast<float>(engSwapChain->getSwapChainExtent().width);
	viewport.height = static_cast<float>(engSwapChain->getSwapChainExtent().height);
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	VkRect2D scissor{ {0, 0}, engSwapChain->getSwapChainExtent() };
	vkCmdSetViewport(commandBuffers[imageIndex], 0, 1, &viewport);
	vkCmdSetScissor(commandBuffers[imageIndex], 0, 1, &scissor);

	vkCmdBeginRenderPass(commandBuffers[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

	renderGameObjects(commandBuffers[imageIndex]);

	vkCmdEndRenderPass(commandBuffers[imageIndex]);
	if (vkEndCommandBuffer(commandBuffers[imageIndex]) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to record command buffer!");
	}
}

void FirstApp::renderGameObjects(VkCommandBuffer commandBuffer)
{
	pipeline->bind(commandBuffer);

	for (auto& obj : gameObjects)
	{
		obj.transform2d.rotation = glm::mod(obj.transform2d.rotation + 0.01f, glm::two_pi<float>());

		SimplePushConstantData push{};
		push.offset = obj.transform2d.translation;
		push.color = obj.color;
		push.transform = obj.transform2d.mat2();

		vkCmdPushConstants(
			commandBuffer,
			pipelineLayout,
			VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
			0,
			sizeof(SimplePushConstantData),
			&push);
		obj.model->bind(commandBuffer);
		obj.model->draw(commandBuffer);
	}
}
