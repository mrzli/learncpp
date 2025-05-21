#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>
#include <stdexcept>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <set>

// Validation layer settings
static const std::vector<const char *> validationLayers = {"VK_LAYER_KHRONOS_validation"};

// Debug messenger callback
static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT severity,
    [[maybe_unused]] VkDebugUtilsMessageTypeFlagsEXT type,
    const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
    [[maybe_unused]] void *pUserData)
{
  if (severity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
  {
    std::cerr << "Validation layer: " << pCallbackData->pMessage << std::endl;
  }
  return VK_FALSE;
}

vk::ApplicationInfo createVulkanApplicationInfo()
{
  vk::ApplicationInfo appInfo{};
  appInfo.sType = vk::StructureType::eApplicationInfo;
  appInfo.setPApplicationName("Vulkan App");
  appInfo.setApplicationVersion(VK_MAKE_API_VERSION(0, 1, 0, 0));
  appInfo.setPEngineName("My Engine");
  appInfo.setEngineVersion(VK_MAKE_API_VERSION(0, 1, 0, 0));
  appInfo.setApiVersion(VK_API_VERSION_1_0);

  return appInfo;
}

vk::Instance createVulkanInstance(const vk::ApplicationInfo &appInfo, bool enableValidationLayers)
{
  // Get GLFW extensions
  uint32_t glfwExtensionCount = 0;
  const char **glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
  std::vector<const char *> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

  // Add debug utils extension if validation layers are enabled
  if (enableValidationLayers)
  {
    extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
  }

  // Verify extension support
  auto availableExtensions = vk::enumerateInstanceExtensionProperties();
  std::set<std::string> requiredExtensions(extensions.begin(), extensions.end());
  for (const auto &ext : availableExtensions)
  {
    requiredExtensions.erase(ext.extensionName);
  }
  if (!requiredExtensions.empty())
  {
    throw std::runtime_error("Required extensions not supported");
  }

  // Instance create info
  vk::InstanceCreateInfo createInfo{};
  // createInfo.sType = vk::StructureType::eInstanceCreateInfo;
  createInfo.setPApplicationInfo(&appInfo);
  createInfo.setEnabledExtensionCount(static_cast<uint32_t>(extensions.size()));
  createInfo.setPpEnabledExtensionNames(extensions.data());

  // Validation layers
  std::vector<const char *> layers;
  if (enableValidationLayers)
  {
    auto availableLayers = vk::enumerateInstanceLayerProperties();

    // Print available layers
    std::cout << "Available validation layers: " << availableLayers.size() << std::endl;
    for (const auto &layer : availableLayers)
    {
      std::cout << "  " << layer.layerName << " (version "
                << VK_VERSION_MAJOR(layer.specVersion) << "."
                << VK_VERSION_MINOR(layer.specVersion) << "."
                << VK_VERSION_PATCH(layer.specVersion) << ")" << std::endl;
    }

    std::set<std::string> requiredLayers(validationLayers.begin(), validationLayers.end());
    for (const auto &layer : availableLayers)
    {
      requiredLayers.erase(layer.layerName);
    }
    if (!requiredLayers.empty())
    {
      throw std::runtime_error("Required validation layers not supported");
    }
    layers = validationLayers;
    createInfo.setEnabledLayerCount(static_cast<uint32_t>(layers.size()));
    createInfo.setPpEnabledLayerNames(layers.data());
  }

  vk::Instance instance = vk::createInstance(createInfo);
  return instance;
}

vk::DebugUtilsMessengerEXT createDebugMessenger(vk::Instance instance, bool enableValidationLayers, vk::DispatchLoaderDynamic &dldy)
{
  if (!enableValidationLayers)
  {
    return vk::DebugUtilsMessengerEXT();
  }

  vk::DebugUtilsMessengerCreateInfoEXT createInfo{};
  createInfo.setMessageSeverity(
      vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose |
      vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
      vk::DebugUtilsMessageSeverityFlagBitsEXT::eError);
  createInfo.setMessageType(
      vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
      vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation |
      vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance);
  createInfo.setPfnUserCallback(debugCallback);
  createInfo.setPUserData(nullptr);

  return instance.createDebugUtilsMessengerEXT(createInfo, nullptr, dldy);
}

void destroyDebugMessenger(vk::Instance instance, vk::DebugUtilsMessengerEXT debugMessenger, vk::DispatchLoaderDynamic &dldy)
{
  if (debugMessenger)
  {
    instance.destroyDebugUtilsMessengerEXT(debugMessenger, nullptr, dldy);
  }
}

int main()
{
  try
  {
    // Initialize GLFW
    if (!glfwInit())
    {
      throw std::runtime_error("Failed to initialize GLFW");
    }

    // Step 1: Create Vulkan instance
    bool enableValidationLayers = true; // Toggle for debugging
    auto appInfo = createVulkanApplicationInfo();
    auto instance = createVulkanInstance(appInfo, enableValidationLayers);

    // Initialize dispatch loader for debug utils
    vk::DispatchLoaderDynamic dldy(instance, vkGetInstanceProcAddr);
    auto debugMessenger = createDebugMessenger(instance, enableValidationLayers, dldy);

    // Print available Vulkan extensions
    std::vector<vk::ExtensionProperties> extensions = vk::enumerateInstanceExtensionProperties();
    std::cout << "Available Vulkan extensions: " << extensions.size() << std::endl;
    for (const auto &extension : extensions)
    {
      std::cout << "  " << extension.extensionName << " (version "
                << extension.specVersion << ")" << std::endl;
    }

    // Print physical devices
    std::vector<vk::PhysicalDevice> physicalDevices = instance.enumeratePhysicalDevices();
    std::cout << "\nAvailable physical devices: " << physicalDevices.size() << std::endl;
    for (const auto &device : physicalDevices)
    {
      vk::PhysicalDeviceProperties properties = device.getProperties();
      std::cout << "  Device name: " << properties.deviceName << std::endl;
      std::cout << "    Type: " << vk::to_string(properties.deviceType) << std::endl;
      std::cout << "    API version: "
                << VK_VERSION_MAJOR(properties.apiVersion) << "."
                << VK_VERSION_MINOR(properties.apiVersion) << "."
                << VK_VERSION_PATCH(properties.apiVersion) << std::endl;
    }

    // Manual cleanup
    destroyDebugMessenger(instance, debugMessenger, dldy);
    instance.destroy();
    glfwTerminate();

    return EXIT_SUCCESS;
  }
  catch (const std::exception &e)
  {
    std::cerr << "Error: " << e.what() << std::endl;
    glfwTerminate();
    return EXIT_FAILURE;
  }
}
