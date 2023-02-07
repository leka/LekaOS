# ? Note the following code has been heavily inspired from:
# ? https://github.com/xioTechnologies/Fusion/blob/main/Examples/Advanced
# ? For more information, see https://github.com/xioTechnologies/Fusion

file(GLOB_RECURSE files "*.c")

add_library(Fusion ${files})

if(UNIX AND NOT APPLE)
	target_link_libraries(Fusion m) # link math library for Linux
endif()
