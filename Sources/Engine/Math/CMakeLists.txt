project(Math CXX)

add_library(Math STATIC)

set(PRIVATE_SOURCES
	"${CMAKE_CURRENT_SOURCE_DIR}/Private/Math.cpp"
)

set(PUBLIC_HEADERS
	"${CMAKE_CURRENT_SOURCE_DIR}/Public/Math/Math.hpp"
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

target_compile_features(Math PRIVATE cxx_std_20)

target_sources(Math 
	PRIVATE 
		${PRIVATE_SOURCES}
	PUBLIC
		${PUBLIC_HEADERS}
	)

target_compile_options(Math
	PUBLIC
		$<$<CXX_COMPILER_ID:MSVC>:/wd4251>
		$<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-msse4.1>
	PRIVATE 
		$<$<CXX_COMPILER_ID:MSVC>:/W4 /utf-8>
		$<$<CXX_COMPILER_ID:Clang>:-Wall -pedantic -Werror -Wextra -Wno-unused-parameter -fsigned-char>
		$<$<CXX_COMPILER_ID:GNU>:-Wall -pedantic -Wextra>
	)

target_include_directories(Math
	PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}/Public
	)

target_link_libraries(Math
   PUBLIC
       Core
)

include(ZenBuildUtility)
create_filters(${CMAKE_CURRENT_SOURCE_DIR} ${PUBLIC_HEADERS} ${PRIVATE_SOURCES})

add_subdirectory(Tests)