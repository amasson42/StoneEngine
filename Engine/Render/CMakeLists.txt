
if (GRAPHICS_API STREQUAL "VULKAN")
	setup_module(
		NAME render
		TARGET_DEPS logging widgets utils gfx_api
		VARIABLE_DEPS Vulkan_LIBRARIES Vulkan_INCLUDE_DIRS
		SPECIAL_HEADER_PATHS ${Vulkan_INCLUDE_DIRS}
		SPECIAL_LIBS ${Vulkan_LIBRARIES}
		FATAL_ERROR
	)
elseif (GRAPHICS_API STREQUAL "OPENGL")
	setup_module(
		NAME render
		TARGET_DEPS logging widgets utils gfx_api
		VARIABLE_DEPS OpenGL::GL GLEW::GLEW
		SPECIAL_LIBS OpenGL::GL GLEW::GLEW
		FATAL_ERROR
	)
else ()
	message(FATAL_ERROR "Unexpected value for GRAPHICS_API, unknown API ${GRAPHICS_API}")
endif()
