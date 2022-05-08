#pragma once

#include "EngineDevice.h"
#include <vector>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

class Model
{
public:
	struct Vertex
	{
		glm::vec2 position;

		static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
		static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
	};
public:
	Model(EngineDevice& device, const std::vector<Vertex>& vertices);
	~Model();
	Model(const Model&) = delete;
	Model& operator=(const Model&) = delete;
	void bind(VkCommandBuffer commandBuffer);
	void draw(VkCommandBuffer commandBuffer);
private:
	void createVertexBuffers(const std::vector<Vertex>& vertices);
private:
	EngineDevice& device;
	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory;
	uint32_t vertexCount;
};