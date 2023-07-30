#ifndef PHNX_VK_CONTEXT_H
#define PHNX_VK_CONTEXT_H
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <phoenix/common.h>

static VkInstance gInstance;

void phnx_create_instance(const cstr_t appName);


#endif // !PHNX_VK_CONTEXT_H
