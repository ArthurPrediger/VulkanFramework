#pragma once

#include "first_app.h"
#include <stdexcept>

FirstApp::FirstApp()
{
	createPipelineLayout();
	createPipeline();
	createCommandBuffers();
}

FirstApp::~FirstApp()
{
	vkDestroyPipelineLayout(engDevice.device(), pipelineLayout, nullptr);
}

void FirstApp::run()
{
	while (!window.shouldClose())
	{
		glfwPollEvents();
	}
}

void FirstApp::createPipelineLayout()
{
	VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 0;
	pipelineLayoutInfo.pSetLayouts = nullptr;
	pipelineLayoutInfo.pushConstantRangeCount = 0;
	pipelineLayoutInfo.pPushConstantRanges = nullptr;

	if (vkCreatePipelineLayout(engDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
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
		engDevice,
		"Shaders/simple_shader.vert.spv",
		"Shaders/simple_shader.frag.spv",
		pipelineConfig
		);
}

void FirstApp::createCommandBuffers()
{
}

void FirstApp::drawFrame()
{
}
