#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
// エラー報告用
#include <stdexcept>
#include <cstdlib>

#include <vector>

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

class HelloTriangleApplication {
public:
	void run() {
		initWindow();
		initVulkan();
		mainLoop();
		cleanup(); // リソース割り当て解放
	}

private:
	GLFWwindow* window;
	
	VkInstance instance;
	
	void initWindow() {
		glfwInit(); // OpenGLコンテキスト作成含む

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // OpenGLコンテキスト作成抑止
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); // ウインドウリサイズ抑止

		// Window作成
		window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
	}
		
	void initVulkan() {
		createInstance();
	}

	void mainLoop() {
		// 単一のフレームをレンダリングするループ
		while (!glfwWindowShouldClose(window)) {
			glfwPollEvents(); // イベントループ
		}
	}

	void cleanup() {
		vkDestroyInstance(instance, nullptr);
		// ウインドウ破棄
		glfwDestroyWindow(window);
		// リソース破棄
		glfwTerminate();
	}
	
	void createInstance(){
		std::vector<const char*> requiredExtensions;

		VkApplicationInfo appInfo{};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "Hello Triangle";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "No Engine";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_0;

		VkInstanceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;

		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
		
		createInfo.enabledExtensionCount = glfwExtensionCount;
		createInfo.ppEnabledExtensionNames = glfwExtensions;
		
		createInfo.enabledLayerCount = 0;
		
		
		// Encountered VK_ERROR_INCOMPATIBLE_DRIVER:
		for(uint32_t i = 0; i < glfwExtensionCount; i++) {
			requiredExtensions.emplace_back(glfwExtensions[i]);
		}

		requiredExtensions.emplace_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);

		createInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;

		createInfo.enabledExtensionCount = (uint32_t) requiredExtensions.size();
		createInfo.ppEnabledExtensionNames = requiredExtensions.data();
		
		
		auto result = vkCreateInstance(&createInfo, nullptr, &instance);
		if(result != VK_SUCCESS){ // カスタムメモリアロケータはnullptr指定
			throw std::runtime_error("failed to create instance!");
		}
	}
	

};

int main() {
	HelloTriangleApplication app;

	try {
		app.run();
	} catch (const std::exception& e) { // なんらかのエラー発生時のレポーティング
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

