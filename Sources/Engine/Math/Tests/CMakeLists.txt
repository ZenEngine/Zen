project(MathTest CXX)

enable_testing()

add_executable(MathTest)

set(PRIVATE_SOURCES
	"${CMAKE_CURRENT_SOURCE_DIR}/MathTestHelper.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/Vector3Test.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/Vector4Test.cpp"
)

target_sources(MathTest
	PRIVATE 
		${PRIVATE_SOURCES}
	)

find_package(GTest CONFIG REQUIRED)

target_compile_features(MathTest PRIVATE cxx_std_20)

target_compile_options(MathTest
	PUBLIC
		$<$<CXX_COMPILER_ID:MSVC>:/wd4251>
	PRIVATE 
		$<$<CXX_COMPILER_ID:MSVC>:/W4 /utf-8>
		$<$<CXX_COMPILER_ID:Clang>:-Wall -pedantic -Werror -Wextra -Wno-unused-parameter -fsigned-char>
		$<$<CXX_COMPILER_ID:GNU>:-Wall -pedantic -Wextra>
	)

set_target_properties(MathTest
	PROPERTIES 
		ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
		LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
		RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
		FOLDER Tests
	)

#-------------------------------------------------
# 追加のライブラリディレクトリ
#-------------------------------------------------
target_link_libraries(MathTest
   PRIVATE
       Math
	   GTest::gtest
	   GTest::gtest_main
)

add_test(AllTestsInMain main)

include(ZenBuildUtility)
create_filters(${CMAKE_CURRENT_SOURCE_DIR} ${PUBLIC_HEADERS} ${PRIVATE_SOURCES})
