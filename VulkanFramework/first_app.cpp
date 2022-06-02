#pragma once

#include "KeyboardMovementController.h"
#include "first_app.h"
#include <stdexcept>
#include <array>
#include <chrono>

static float constexpr MAX_FRAME_TIME = 0.167f;

FirstApp::FirstApp()
{
	loadGameObjects();
}

FirstApp::~FirstApp()
{}

void FirstApp::run()
{
	SimpleRenderSystem simpleRenderSystem{device, renderer.getSwapChainRenderPass()};
    Camera camera{};
    camera.setViewTarget(glm::vec3(-3.0f, -4.0f, 5.0f), glm::vec3(0.0f, 0.0f, 2.5f));

    auto viewerObject = GameObject::createGameObject();;
    KeyboardMovementController cameraController{};

    auto currentTime = std::chrono::high_resolution_clock::now();

	while (!window.shouldClose())
	{
		glfwPollEvents();

        auto newTime = std::chrono::high_resolution_clock::now();
        float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
        currentTime = newTime;

        frameTime = glm::min(frameTime, MAX_FRAME_TIME);

        cameraController.moveInPlaneXZ(window.getGLFWwindow(), frameTime, viewerObject);
        camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);

        float aspect = renderer.getAspectRatio();
        camera.setPerspectiveProjection(glm::pi<float>() / 4.0f, aspect, 0.1f, 100.0f);
		
		if (auto commandBuffer = renderer.beginFrame())
		{
			renderer.beginSwapChainRenderPass(commandBuffer);
			simpleRenderSystem.renderGameObjects(commandBuffer, gameObjects, camera);
			renderer.endSwapChainRenderPass(commandBuffer);
			renderer.endFrame();
		}
	}

	vkDeviceWaitIdle(device.device());
}

void FirstApp::loadGameObjects()
{
    std::shared_ptr<Model> model = Model::createModelFromFile(device, "models/smooth_vase.obj");

    auto gameObj = GameObject::createGameObject();
    gameObj.model = model;
    gameObj.transform.translation = { 0.0f, 0.0f, 2.5f };
    gameObj.transform.scale= glm::vec3{3.0f};
    gameObjects.push_back(std::move(gameObj));
}
