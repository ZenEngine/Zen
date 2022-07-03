# CC BY-SA 3.0
# https://stackoverflow.com/questions/32645217/visual-studio-as-an-editor-for-cmake-friendly-project
function(create_filters source_root_directory)
	foreach(source IN LISTS ARGN)
		get_filename_component(source_directory "${source}" DIRECTORY)
		file(RELATIVE_PATH source_relative_directory "${CMAKE_CURRENT_SOURCE_DIR}" "${source_directory}")
		string(REPLACE "/" "\\" source_directory_msvc "${source_relative_directory}")
		source_group("${source_directory_msvc}" FILES "${source}")
	endforeach()
endfunction(create_filters)

# CC BY-SA 3.0
# CMake: How do I change properties on subdirectory project targets?
# https://stackoverflow.com/questions/45092198/cmake-how-do-i-change-properties-on-subdirectory-project-targets

function(get_all_targets _result _dir)
    get_property(_subdirs DIRECTORY "${_dir}" PROPERTY SUBDIRECTORIES)
    foreach(_subdir IN LISTS _subdirs)
        get_all_targets(${_result} "${_subdir}")
    endforeach()
    get_property(_sub_targets DIRECTORY "${_dir}" PROPERTY BUILDSYSTEM_TARGETS)
    set(${_result} ${${_result}} ${_sub_targets} PARENT_SCOPE)
endfunction()

function(add_subdirectory_with_folder _folder_name _folder)
    add_subdirectory(${_folder} ${ARGN})
    get_all_targets(_targets "${_folder}")
    foreach(_target IN LISTS _targets)
        set_target_properties(
            ${_target}
            PROPERTIES FOLDER "${_folder_name}"
        )
    endforeach()
endfunction()