cmake_minimum_required (VERSION 2.6)
project (GKoala)

file(GLOB_RECURSE _SRC_ ${CMAKE_CURRENT_SOURCE_DIR} "*.h" "*.cpp")

set(PLATFORM linux)
set(COCOS_ROOT $ENV{COCOS_ROOT})

set(GCC_C_FLAGS "-MMD -Wall -Werror -fPIC -Wno-unused-function -Wno-unused-result")
set(GCC_CPP_FLAGS "-MMD -Wall -Werror -fPIC -std=c++0x -Wno-unused-function -Wno-unused-result")

set (CMAKE_CXX_FLAGS ${GCC_CPP_FLAGS})
set (CMAKE_C_FLAGS ${GCC_C_FLAGS})

ADD_DEFINITIONS(-DLINUX)

include_directories(
${CMAKE_CURRENT_SOURCE_DIR}
${COCOS_ROOT}/cocos2dx
${COCOS_ROOT}/cocos2dx/cocoa 
${COCOS_ROOT}/cocos2dx/include 
${COCOS_ROOT}/cocos2dx/kazmath/include 
${COCOS_ROOT}/cocos2dx/platform/linux 
${COCOS_ROOT}/cocos2dx/platform/third_party/linux/libpng 
${COCOS_ROOT}/cocos2dx/platform/third_party/linux/libjpeg 
${COCOS_ROOT}/cocos2dx/platform/third_party/linux/libtiff/include 
${COCOS_ROOT}/cocos2dx/platform/third_party/linux/libwebp
${COCOS_ROOT}/extensions/
${COCOS_ROOT}/external/chipmunk/include/chipmunk
${COCOS_ROOT}/external/jansson/
${COCOS_ROOT}/extensions/cocos2dx-store/Soomla
${COCOS_ROOT}/extensions/cocos2dx-store/
${COCOS_ROOT}/extensions/cocos2dx-store/Soomla/data
${COCOS_ROOT}/extensions/cocos2dx-store/Soomla/domain
${COCOS_ROOT}/extensions/cocos2dx-store/Soomla/domain/virtualCurrencies
${COCOS_ROOT}/extensions/cocos2dx-store/Soomla/domain/virtualGoods
${COCOS_ROOT}/extensions/cocos2dx-store/Soomla/PurchaseTypes
${COCOS_ROOT}/CocosDenshion/include
${COCOS_ROOT}/external/)

add_library( GKoala STATIC  ${_SRC_})
