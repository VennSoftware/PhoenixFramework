VULKAN_PATH = os.getenv("VULKAN_SDK");
VENDOR = "vendor"
workspace "Phoenix"
    configurations {"Debug", "Release", "Dist"}


project "PhoenixFramework"
    language "c"
    kind "StaticLib"
    architecture ("amd64")
    local BASE_DIR = "PhoenixFramework"


    files {BASE_DIR .. "/src/**.c", BASE_DIR .. "/include/**.h", VENDOR .. "/include/**.h"}
    includedirs {VENDOR .. "/include", BASE_DIR .. "/include", VULKAN_PATH .. "/include"}
    libdirs {VENDOR .. "/lib", VULKAN_PATH .. "/Lib"}
    links {"vulkan-1", "glfw3"}

    filter {"configurations:Debug"}
        defines {"DEBUG"}

    filter {"configurations:Release"}
        defines {"NDEBUG"}

project "PhoenixApp"
    language "c"
    architecture ("amd64")
    kind "ConsoleApp"
    local BASE_DIR = "PhoenixApp"


    files {BASE_DIR .. "/src/**.c", BASE_DIR .. "/include/**.h", VENDOR .. "/include/**.h"}
    includedirs {VENDOR .. "/include", BASE_DIR .. "/include", "PhoenixFramework/include", VULKAN_PATH .. "/include"}
    libdirs {VENDOR .. "/lib", VULKAN_PATH .. "/Lib"}
    links {"PhoenixFramework"}