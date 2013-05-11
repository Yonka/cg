file(GLOB_RECURSE HEADERS "*.h*")
add_custom_target(visualization_headers SOURCES ${HEADERS})
add_custom_target(find_path_headers SOURCES ${HEADERS})

