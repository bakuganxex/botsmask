cmake_minimum_required(VERSION 3.1)
project(rr_statusmask CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -g0 -O2 -fvisibility=hidden -fvisibility-inlines-hidden -fno-rtti -ffunction-sections -fdata-sections")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-variable -Wno-unused-function -Wno-sign-compare -Wno-unused-local-typedefs")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -ldl -static-libgcc -static-libstdc++ -Wl,-gc-sections -Wl,--version-script=${PROJECT_SOURCE_DIR}/version_script.lds")

set(PROJECT_SRC_DIR
	"${PROJECT_SOURCE_DIR}/"
	"${PROJECT_SOURCE_DIR}/src"
)

set(PROJECT_VENDOR_DIR
	"${PROJECT_SOURCE_DIR}/vendor/public"
	"${PROJECT_SOURCE_DIR}/vendor/dlls"
	"${PROJECT_SOURCE_DIR}/vendor/engine"
	"${PROJECT_SOURCE_DIR}/vendor/common"
	"${PROJECT_SOURCE_DIR}/vendor/pm_shared"
	"${PROJECT_SOURCE_DIR}/vendor/metamod"
)

set(RR_STATUSMASK_SRCS
	"src/main.cpp"
	"src/meta_api.cpp"
	"src/h_export.cpp"
	"src/engine_rehlds.cpp"
	"src/sdk_util.cpp"
	"vendor/public/interface.cpp"
)

include_directories(
	${PROJECT_SRC_DIR}
	${PROJECT_VENDOR_DIR}
)

add_definitions(
	-DNDEBUG
	-Dlinux
	-D__linux__
	-D_vsnprintf=vsnprintf
	-D_stricmp=strcasecmp
	-D_strnicmp=strncasecmp
)

add_library(rr_statusmask_mm_i386 SHARED ${RR_STATUSMASK_SRCS})
set_target_properties(rr_statusmask_mm_i386 PROPERTIES PREFIX "" COMPILE_FLAGS "-m32" LINK_FLAGS "-m32" POSITION_INDEPENDENT_CODE ON)
