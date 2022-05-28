project(Core CXX)

add_library(Core STATIC)

set(PRIVATE_SOURCES
	"${CMAKE_CURRENT_SOURCE_DIR}/Private/Misc/Enviroment.cpp"
)

set(PUBLIC_HEADERS
	"${CMAKE_CURRENT_SOURCE_DIR}/Public/Core/Misc/Assert.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/Public/Core/Misc/Enviroment.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/Public/Core/Platform/PlatformDefine.hpp"
)

set_target_properties(Core 
	PROPERTIES 
		ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
		LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
		RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
		FOLDER Engine
)

target_compile_definitions(Core
	PUBLIC
		$<$<CONFIG:DEBUG>:ZEN_DEBUG>
)

target_sources(Core 
	PRIVATE 
		${PRIVATE_SOURCES}
	PUBLIC
		${PUBLIC_HEADERS}
	)

target_compile_options(Core
	PUBLIC
		$<$<CXX_COMPILER_ID:MSVC>:/wd4251>
	PRIVATE 
		$<$<CXX_COMPILER_ID:MSVC>:/W4 /utf-8>
		$<$<CXX_COMPILER_ID:Clang>:-Wall -pedantic -Werror -Wextra -Wno-unused-parameter -fsigned-char>
	)

target_include_directories(Core
	PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}/Public
	)