#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>
#include <stdexcept>
#include <cstdlib>
#include <iostream>
#include <vector>



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

vk::UniqueInstance createVulkanInstance(const vk::ApplicationInfo &appInfo)
{
  vk::InstanceCreateInfo createInfo{};
  createInfo.sType = vk::StructureType::eInstanceCreateInfo;
  createInfo.pApplicationInfo = &appInfo;

  vk::UniqueInstance instance = vk::createInstanceUnique(createInfo);
  return instance;
}

int main()
{
  try
  {
    auto appInfo = createVulkanApplicationInfo();
    auto instance = createVulkanInstance(appInfo);

    // Print available Vulkan extensions
    std::vector<vk::ExtensionProperties> extensions = vk::enumerateInstanceExtensionProperties();
    std::cout << "Available Vulkan extensions: " << extensions.size() << std::endl;
    for (const auto &extension : extensions)
    {
      std::cout << "  " << extension.extensionName << " (version "
                << extension.specVersion << ")" << std::endl;
    }

    // Print physical devices
    std::vector<vk::PhysicalDevice> physicalDevices = instance->enumeratePhysicalDevices();
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

    return EXIT_SUCCESS;
  }
  catch (const std::exception &e)
  {
    std::cerr << "Error: " << e.what() << std::endl;
    return EXIT_FAILURE;
  }
}
