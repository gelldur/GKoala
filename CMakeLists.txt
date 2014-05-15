cmake_minimum_required(VERSION 2.8)
project(GKoala)

if ( ${PLATFORM} STREQUAL "LINUX")
	set (PLATFORM_FOLDER linux)
else()
	message( FATAL_ERROR "Sorry we don't support this" )
endif()

set(COCOS_ROOT $ENV{COCOS_ROOT})
set(KOALA_GAMES_ROOT $ENV{KOALA_GAMES_ROOT})

if (NOT COCOS_ROOT)
     message( FATAL_ERROR "Please set COCOS_ROOT" )
endif()
if (NOT KOALA_GAMES_ROOT)
     message( FATAL_ERROR "Please set KOALA_GAMES_ROOT" )
endif()

message("CXX compiler version:" ${CMAKE_CXX_COMPILER_VERSION})
message("C compiler version:" ${CMAKE_C_COMPILER_VERSION})

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	# using Clang
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	# using GCC
	set (LINKER_FLAGS "-fuse-ld=gold")
	set (ERROR_FLAGS_COMMON "-Wall -Werror -Wno-unused-result -Wno-unused-function -Wno-multichar -Wformat -Wpointer-arith -Winline -Wunreachable-code -Wstrict-overflow=4" )
	set(ERROR_CPP_FLAGS "${ERROR_FLAGS_COMMON} -Wcast-align -Wctor-dtor-privacy -Wdelete-non-virtual-dtor -Wnoexcept -Wcast-qual -Wfatal-errors -Wwrite-strings" )
	set(ERROR_C_FLAGS "${ERROR_FLAGS_COMMON}" )

	set(FLAGS_1 "-g3 -O0")
	set(FLAGS_2 "-fPIC -MMD")
	set(BACKTRACE_FLAGS "-rdynamic -g")


	if( ${CMAKE_CXX_COMPILER_VERSION} STREQUAL "4.8.1")
		#Some problems do not use please now
		#set(RUNTIME_CHECKER "-fno-strict-aliasing -pipe -lmudflapth -funwind-tables")
	else()
		set(RUNTIME_CHECKER "-fsanitize=undefined -fsanitize=shift -fsanitize=integer-divide-by-zero -fsanitize=unreachable -fsanitize=vla-bound -fsanitize=null -fsanitize=return -fsanitize=signed-integer-overflow -ftrapv")
	endif()

elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	# using Visual Studio C++
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}	${ERROR_CPP_FLAGS} ${FLAGS_1} ${FLAGS_2} ${LINKER_FLAGS} ${BACKTRACE_FLAGS} ${RUNTIME_CHECKER} -fabi-version=6 -std=c++11")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${ERROR_C_FLAGS} ${FLAGS_1} ${FLAGS_2} ${LINKER_FLAGS} ${BACKTRACE_FLAGS} ${RUNTIME_CHECKER} -fabi-version=6  -std=c99")

add_definitions(-D${PLATFORM})

option(DEBUG "Is this debug version" ON) # Enabled by default
if (DEBUG)
    message("It is DEBUG build")
    add_definitions(-DDEBUG=1)
    add_definitions(-D_DEBUG -DDEBUG -DCOCOS2D_DEBUG=1)
    set(BUILD_FOLDER debug)
else ()
    message("It is RELEASE build")
    set(BUILD_FOLDER release)
endif ()


set(CMAKE_BINARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/proj.${PLATFORM_FOLDER}/bin/${BUILD_FOLDER})
set(CMAKE_CURRENT_BINARY_DIR ${CMAKE_BINARY_DIR})

set(SOURCES_DIR ${PROJECT_SOURCE_DIR}/src)

file(GLOB_RECURSE SOURCES_GKOALA ${SOURCES_DIR}/*.cpp)

add_library( GKoala STATIC ${SOURCES_GKOALA})

target_include_directories( GKoala
PUBLIC ${SOURCES_DIR}
PUBLIC ${KOALA_GAMES_ROOT}/KoalaComponent/src 
PUBLIC ${COCOS_ROOT}/cocos2dx
PUBLIC ${COCOS_ROOT}/extensions
PUBLIC ${COCOS_ROOT}/cocos2dx/cocoa
PUBLIC ${COCOS_ROOT}/cocos2dx/include
PUBLIC ${COCOS_ROOT}/cocos2dx/kazmath/include
PUBLIC ${COCOS_ROOT}/cocos2dx/platform/${PLATFORM_FOLDER}
PUBLIC ${COCOS_ROOT}/cocos2dx/platform/third_party/${PLATFORM_FOLDER}/libpng
PUBLIC ${COCOS_ROOT}/cocos2dx/platform/third_party/${PLATFORM_FOLDER}/libjpeg
PUBLIC ${COCOS_ROOT}/cocos2dx/platform/third_party/${PLATFORM_FOLDER}/libtiff/include
PUBLIC ${COCOS_ROOT}/cocos2dx/platform/third_party/${PLATFORM_FOLDER}/libwebp
)

########################################################################################################################
########################################################################################################################
########################################################################################################################
# Test #################################################################################################################
add_executable( GKoalaTest 
${PROJECT_SOURCE_DIR}/visual_test/simple_test/proj.linux/main.cpp
${PROJECT_SOURCE_DIR}/visual_test/simple_test/Classes/HelloWorldScene.cpp
${PROJECT_SOURCE_DIR}/visual_test/simple_test/Classes/AppDelegate.cpp
)

target_include_directories( GKoalaTest
PRIVATE ${PROJECT_SOURCE_DIR}/visual_test/simple_test/Classes/
)

#pthread
find_package(Threads REQUIRED)
#glfw
find_package(PkgConfig REQUIRED)
#pkg_search_module(GLFW REQUIRED libglfw)
find_package(GLEW REQUIRED)
message("GLEW found: " ${GLEW_FOUND})
#include_directories(${GLFW_INCLUDE_DIRS})
#OpenGL
find_package(OpenGL REQUIRED)
message("Open gl found: " ${OPENGL_FOUND})
#cUrl
find_package(CURL REQUIRED)
message("cUrl found: " ${CURL_FOUND})
#dl
find_library( DL_LIB NAMES ${CMAKE_DL_LIBS} )
message("dl found: " ${DL_LIB})

target_link_libraries(GKoalaTest
GKoala
${COCOS_ROOT}/lib/${PLATFORM_FOLDER}/${BUILD_FOLDER}/libcocos2d.so
${GLFW_LIBRARIES}
${OPENGL_LIBRARIES}
${CURL_LIBRARIES}
${CMAKE_THREAD_LIBS_INIT}
)


