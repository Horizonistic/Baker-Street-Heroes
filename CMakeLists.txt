cmake_minimum_required (VERSION 2.6)
project (Game)

add_subdirectory(oxygine-framework/ oxygine-framework)
add_definitions(${OXYGINE_DEFINITIONS})
include_directories(${OXYGINE_INCLUDE_DIRS} inc)
link_directories(${OXYGINE_LIBRARY_DIRS})

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OXYGINE_CXX_FLAGS}")

#todo: clean up release vs debug flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -std=c++11")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG} -g")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE} -O2")

set(CMAKE_BUILD_TYPE "Debug")

# Find includes and add src dir
file(GLOB hFiles
		"inc/*.h"
		)
add_subdirectory(src)
add_subdirectory(Box2D)

add_executable(Game ${SOURCE_FILES} ${hFiles})
target_link_libraries(Game ${OXYGINE_CORE_LIBS} Box2D)



if (WIN32) #disable console mode for VC++
	set_target_properties(Game PROPERTIES WIN32_EXECUTABLE TRUE)
endif(WIN32)



if (EMSCRIPTEN)
	SET(CMAKE_EXECUTABLE_SUFFIX ".html")	
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s NO_EXIT_RUNTIME=1 -s WARN_ON_UNDEFINED_SYMBOLS=1 --memory-init-file 0 -s TOTAL_MEMORY=50331648")
	em_link_pre_js(Game  ${OXYGINE_JS_LIBRARIES}  ${CMAKE_CURRENT_SOURCE_DIR}/data.js)
endif(EMSCRIPTEN)