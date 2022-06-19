project(Core CXX)

add_library(Core STATIC)

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	target_compile_definitions(Core
		PUBLIC
			ZEN_PLATFORM_WINDOWS
			NOMINMAX
	)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	target_compile_definitions(Core
		PUBLIC
			ZEN_PLATFORM_MAC
	)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	target_compile_definitions(Core
		PUBLIC
			ZEN_PLATFORM_LINUX
	)
endif()

set(PRIVATE_SOURCES
	"${CMAKE_CURRENT_SOURCE_DIR}/Private/Hash/XxHash.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/Private/Misc/Enviroment.cpp"
)

set(PUBLIC_HEADERS
	"${CMAKE_CURRENT_SOURCE_DIR}/Public/Core/Misc/Assert.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/Public/Core/Misc/EnumUtility.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/Public/Core/Misc/Enviroment.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/Public/Core/Hash/XxHash.hpp"
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
		$<$<CONFIG:RELEASE>:ZEN_RELEASE>
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
		$<$<CXX_COMPILER_ID:GNU>:-Wall -pedantic -Wextra>
	)

find_package(xxHash CONFIG REQUIRED)

target_link_libraries(Core
  PUBLIC
	xxHash::xxhash
)

target_include_directories(Core
	PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}/Public
	)


target_compile_features(Core PRIVATE cxx_std_20)
