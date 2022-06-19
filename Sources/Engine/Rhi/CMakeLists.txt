project(Rhi CXX)

add_library(Rhi STATIC)

set(PRIVATE_SOURCES
	"${CMAKE_CURRENT_SOURCE_DIR}/Private/Dx12/BufferDx12.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/Private/Dx12/BufferDx12.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/Private/Dx12/DeviceDx12.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/Private/Dx12/DeviceDx12.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/Private/Dx12/RenderPassDx12.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/Private/Dx12/RenderPassDx12.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/Private/Dx12/SwapChainDx12.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/Private/Dx12/TextureDx12.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/Private/Dx12/TextureDx12.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/Private/DxgiUtility.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/Private/DxgiUtility.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/Private/HandlePool.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/Private/RefCountPtr.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/Private/slot_map.h"
)

set(PUBLIC_HEADERS
	"${CMAKE_CURRENT_SOURCE_DIR}/Public/Rhi/Handle.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/Public/Rhi/Rhi.hpp"
)

set_target_properties(Rhi 
	PROPERTIES 
		ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
		LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
		RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
		FOLDER Engine
)

target_sources(Rhi 
	PRIVATE 
		${PRIVATE_SOURCES}
	PUBLIC
		${PUBLIC_HEADERS}
	)

target_compile_options(Rhi
	PUBLIC
		$<$<CXX_COMPILER_ID:MSVC>:/wd4251>
	PRIVATE 
		$<$<CXX_COMPILER_ID:MSVC>:/W4 /utf-8>
		$<$<CXX_COMPILER_ID:Clang>:-Wall -pedantic -Werror -Wextra -Wno-unused-parameter -fsigned-char>
		$<$<CXX_COMPILER_ID:GNU>:-Wall -pedantic -Wextra>
	)

target_include_directories(Rhi
	PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}/Public
	)

find_package(d3d12-memory-allocator CONFIG REQUIRED)

target_link_libraries(Rhi
    PUBLIC
        Core
    PRIVATE
	    unofficial::d3d12-memory-allocator
)

target_compile_features(Rhi PRIVATE cxx_std_20)
