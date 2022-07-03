project(CoreTest CXX)

enable_testing()

add_executable(CoreTest)

set(PRIVATE_SOURCES
	"${CMAKE_CURRENT_SOURCE_DIR}/Containers/ArrayTest.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/Containers/StringTest.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/Containers/StringViewTest.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/Memory/SharedPtrTest.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/Memory/UniquePtrTest.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/Span/SpanTest.cpp"
)

set(PUBLIC_HEADERS
)

target_sources(CoreTest 
	PRIVATE 
		${PRIVATE_SOURCES}
	)

find_package(GTest CONFIG REQUIRED)

target_compile_features(CoreTest PRIVATE cxx_std_20)

target_compile_options(CoreTest
	PUBLIC
		$<$<CXX_COMPILER_ID:MSVC>:/wd4251>
	PRIVATE 
		$<$<CXX_COMPILER_ID:MSVC>:/W4 /utf-8>
		$<$<CXX_COMPILER_ID:Clang>:-Wall -pedantic -Werror -Wextra -Wno-unused-parameter -fsigned-char>
		$<$<CXX_COMPILER_ID:GNU>:-Wall -pedantic -Wextra>
	)

set_target_properties(CoreTest
	PROPERTIES 
		ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
		LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
		RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
		FOLDER Tests
	)

#-------------------------------------------------
# 追加のライブラリディレクトリ
#-------------------------------------------------
target_link_libraries(CoreTest
   PRIVATE
       Core
	   GTest::gtest
	   GTest::gtest_main
)

add_test(AllTestsInMain main)

include(ZenBuildUtility)
create_filters(${CMAKE_CURRENT_SOURCE_DIR} ${PUBLIC_HEADERS} ${PRIVATE_SOURCES})
