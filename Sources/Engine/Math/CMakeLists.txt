project(Math CXX)

add_library(Math STATIC)

set(PRIVATE_SOURCES
	"${CMAKE_CURRENT_SOURCE_DIR}/Private/Math.cpp"
)

set(PUBLIC_HEADERS
	"${CMAKE_CURRENT_SOURCE_DIR}/Public/Math/Vector3.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/Public/Math/Vector4.hpp"
)

set_target_properties(Math 
	PROPERTIES 
		ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
		LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
		RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
		FOLDER Engine
	)

target_compile_features(Math PRIVATE cxx_std_17)

target_sources(Math 
	PRIVATE 
		${PRIVATE_SOURCES}
	PUBLIC
		${PUBLIC_HEADERS}
	)

target_include_directories(Math
	PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}/Public
	)