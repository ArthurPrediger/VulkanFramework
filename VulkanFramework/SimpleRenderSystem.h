#pragma once

#include "Camera.h"
#include "Pipeline.h"
#include "EngineDevice.h"
#include "GameObject.h"
#include <memory>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

class SimpleRenderSystem
{
	struct SimplePushConstantData
	{
		glm::mat4 transform{ 1.0f };
		alignas(16) glm::vec3 color;
	};
public:
	SimpleRenderSystem(EngineDevice& device, VkRenderPass renderPass);
	~SimpleRenderSystem();
	SimpleRenderSystem(const SimpleRenderSystem&) = delete;
	SimpleRenderSystem& operator=(const SimpleRenderSystem&) = delete;

	void renderGameObjects(VkCommandBuffer commandBuffer, std::vector<GameObject>& gameObjects, const Camera& camera);
private:
	void createPipelineLayout();
	void createPipeline(VkRenderPass renderPass);
private:
	EngineDevice& device;
	std::unique_ptr<Pipeline> pipeline;
	VkPipelineLayout pipelineLayout;
};