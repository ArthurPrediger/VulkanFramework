#pragma once

#include "first_app.h"
#include <stdexcept>
#include <array>

FirstApp::FirstApp()
{
	loadModels();
	createPipelineLayout();
	createPipeline();
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

void FirstApp::loadModels()
{
	std::vector<Model::Vertex> vertices {
		{{0.0f, -0.5f}},
		{{0.5f, 0.5f}},
		{{-0.5f, 0.5f}}
	};

	model = std::make_unique<Model>(device, vertices);
}

void FirstApp::createPipelineLayout()
{
	VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 0;
	pipelineLayoutInfo.pSetLayouts = nullptr;
	pipelineLayoutInfo.pushConstantRangeCount = 0;
	pipelineLayoutInfo.pPushConstantRanges = nullptr;

	if (vkCreatePipelineLayout(device.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create pipelineLayout");
	}
}

void FirstApp::createPipeline()
{
	auto pipelineConfig = Pipeline::defaultPipelineConfigInfo(engSwapChain.width(), engSwapChain.height());
	pipelineConfig.renderPass = engSwapChain.getRenderPass();
	pipelineConfig.pipelineLayout = pipelineLayout;
	pipeline = std::make_unique<Pipeline>(
		device,
		"Shaders/simple_shader.vert.spv",
		"Shaders/simple_shader.frag.spv",
		pipelineConfig
		);
}

void FirstApp::createCommandBuffers()
{
	commandBuffers.resize(engSwapChain.imageCount());

	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = device.getCommandPool();
	allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

	if (vkAllocateCommandBuffers(device.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to allocate command buffers!");
	}

	for (int i = 0; i < commandBuffers.size(); i++)
	{
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to begin recording command buffer!");
		}

		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = engSwapChain.getRenderPass();
		renderPassInfo.framebuffer = engSwapChain.getFrameBuffer(i);

		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = engSwapChain.getSwapChainExtent();

		std::array<VkClearValue, 2> clearValues{};
		clearValues[0].color = { 0.1f, 0.1f, 0.1f, 1.0f };
		clearValues[1].depthStencil = { 1.0f, 0 };
		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();

		vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		pipeline->bind(commandBuffers[i]);
		model->bind(commandBuffers[i]);
		model->draw(commandBuffers[i]);

		vkCmdEndRenderPass(commandBuffers[i]);
		if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to record command buffer!");
		}
	}
}

void FirstApp::drawFrame()
{
	uint32_t imageIndex;
	auto result = engSwapChain.acquireNextImage(&imageIndex);

	if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
	{
		throw std::runtime_error("Failed to acquire swap chain image!");
	}

	result = engSwapChain.submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);

	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to present swap chain image!");
	}
}
