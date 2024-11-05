set(STONE_ENGINE_ENABLE_TESTS OFF)
if ( NOT SKIP_TESTS AND TARGET GTest::gtest_main )
	set(STONE_ENGINE_ENABLE_TESTS ON)
	enable_testing()
	include(GoogleTest)

	add_custom_target(build_tests)
endif ()

set(SRCS_FOR_DOCS "" CACHE INTERNAL "")
set(INCLUDE_DIRS_FOR_DOCS "" CACHE INTERNAL "")

# Layer 0
add_subdirectory(Utils)
# Layer 1
add_subdirectory(Logging)
# Layer 2
add_subdirectory(Core)
# Layer 3
add_subdirectory(Network)
add_subdirectory(Scene)
# Layer 4
add_subdirectory(Physics)
# Layer 5
add_subdirectory(Sound)
add_subdirectory(Widgets)
add_subdirectory(Render)
# Layer 6
add_subdirectory(Window)

get_all_targets(all_targets)
set(test_targets)
foreach ( target IN ITEMS ${all_targets} )
	if ( NOT target MATCHES "^test_" )
		message(DEBUG "library target `${target}` will be selected for docs")
		get_target_property(LOCAL_INC_DIRS ${target} INCLUDE_DIRECTORIES)
		list(APPEND INCLUDE_DIRS_FOR_DOCS ${LOCAL_INC_DIRS})
	endif ()
endforeach ()

list(REMOVE_DUPLICATES INCLUDE_DIRS_FOR_DOCS)
set(INCLUDE_DIRS_FOR_DOCS "${INCLUDE_DIRS_FOR_DOCS}" CACHE INTERNAL "")
