project(Launch CXX)

add_executable(Launch WIN32)

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	set(PRIVATE_SOURCES
		"${CMAKE_CURRENT_SOURCE_DIR}/Private/Windows/Launch_Windows.cpp"
	)

    target_link_options(Launch
        PRIVATE
            $<$<CXX_COMPILER_ID:GNU>:-municode>
    )
    target_compile_options(Launch
        PRIVATE
            $<$<CXX_COMPILER_ID:GNU>:-municode>
    )
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	set(PRIVATE_SOURCES
		"${CMAKE_CURRENT_SOURCE_DIR}/Private/Linux/Launch_Linux.cpp"
	)
endif()


target_sources(Launch
	PRIVATE 
		${PRIVATE_SOURCES}
		"${CMAKE_CURRENT_SOURCE_DIR}/Private/Main.cpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/Private/Main.hpp"
	)

target_compile_options(Launch
	PUBLIC
		$<$<CXX_COMPILER_ID:MSVC>:/wd4251>
	PRIVATE 
		$<$<CXX_COMPILER_ID:MSVC>:/W4 /utf-8>
		$<$<CXX_COMPILER_ID:Clang>:-Wall -pedantic -Werror -Wextra -Wno-unused-parameter -fsigned-char>
	)

set_target_properties(Launch
	PROPERTIES 
		ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
		LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
		RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
		FOLDER Engine
	)

target_compile_features(Launch PRIVATE cxx_std_17)

include(ZenBuildUtility)
create_filters(${CMAKE_CURRENT_SOURCE_DIR} ${PUBLIC_HEADERS} ${PRIVATE_SOURCES})