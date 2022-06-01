#-------------------------------------------------
# CMake設定
#-------------------------------------------------
# バージョン情報
cmake_minimum_required(VERSION 3.19)

# プロジェクト名を設定
project(Zen CXX)

# エンジンのバージョン設定
set(ZEN_ENGINE_MAJOR_VERSION 0)
set(ZEN_ENGINE_MINOR_VERSION 1)
set(ZEN_ENGINE_PATCH_VERSION 0)
set(ZEN_ENGINE_VERSION ${ZEN_ENGINE_MAJOR_VERSION}.${ZEN_ENGINE_MINOR_VERSION}.${ZEN_ENGINE_PATCH_VERSION})


# プロジェクトのルートパスを設定
set(ZEN_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR})

# プロジェクト内用モジュールを利用可能にするためのパスを設定する。
set(CMAKE_MODULE_PATH ${ZEN_ROOT_PATH}/BuildTools/Modules/)

add_subdirectory(Sources)