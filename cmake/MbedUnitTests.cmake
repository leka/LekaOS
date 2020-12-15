# This file is called by mbed-src/CMakeLists.txt when unit tests are active
# and we only want to build a stub that contains all the headers.

project(mbed-os)

# Unit test stub include dirs (list adapted from mbed-src/UNITTESTS/CMakeLists.txt)
set(unittest_includes_base
	"${ROOT_DIR}/extern/mbed-os/UNITTESTS/target_h"
	"${ROOT_DIR}/extern/mbed-os/events/tests/UNITTESTS/target_h"
	"${ROOT_DIR}/extern/mbed-os/events/tests/UNITTESTS/target_h/equeue"
	"${ROOT_DIR}/extern/mbed-os/UNITTESTS/target_h/platform"
	"${ROOT_DIR}/extern/mbed-os/UNITTESTS/target_h/platform/cxxsupport"
	"${ROOT_DIR}/extern/mbed-os/UNITTESTS/target_h/drivers"
	"${ROOT_DIR}/extern/mbed-os/UNITTESTS/target_h/rtos/include"
	"${ROOT_DIR}/extern/mbed-os/UNITTESTS/stubs"
	"${ROOT_DIR}/extern/mbed-os"
	"${ROOT_DIR}/extern/mbed-os/features"
	"${ROOT_DIR}/extern/mbed-os/platform/include"
	"${ROOT_DIR}/extern/mbed-os/platform/include/platform"
	"${ROOT_DIR}/extern/mbed-os/platform/mbed-trace/include"
	"${ROOT_DIR}/extern/mbed-os/storage/filesystem/littlefs/include"
	"${ROOT_DIR}/extern/mbed-os/storage/filesystem/fat/include"
	"${ROOT_DIR}/extern/mbed-os/storage/blockdevice/include"
	"${ROOT_DIR}/extern/mbed-os/storage/filesystem/include"
	"${ROOT_DIR}/extern/mbed-os/storage/kvstore/include"
	"${ROOT_DIR}/extern/mbed-os/storage/kvstore/kv_config"
	"${ROOT_DIR}/extern/mbed-os/storage/kvstore/kv_config/include"
	"${ROOT_DIR}/extern/mbed-os/drivers"
	"${ROOT_DIR}/extern/mbed-os/drivers/include"
	"${ROOT_DIR}/extern/mbed-os/drivers/include/drivers"
	"${ROOT_DIR}/extern/mbed-os/drivers/include/drivers/internal"
	"${ROOT_DIR}/extern/mbed-os/hal"
	"${ROOT_DIR}/extern/mbed-os/hal/include"
	"${ROOT_DIR}/extern/mbed-os/events/include"
	"${ROOT_DIR}/extern/mbed-os/events/include/events/internal"
	"${ROOT_DIR}/extern/mbed-os/events/source"
	"${ROOT_DIR}/extern/mbed-os/rtos/include"
	"${ROOT_DIR}/extern/mbed-os/features/frameworks"
	"${ROOT_DIR}/extern/mbed-os/connectivity/libraries/nanostack-libservice"
	"${ROOT_DIR}/extern/mbed-os/connectivity/libraries/nanostack-libservice/mbed-client-libservice"
	"${ROOT_DIR}/extern/mbed-os/connectivity/netsocket/include"
	"${ROOT_DIR}/extern/mbed-os/features/filesystem/fat"
	"${ROOT_DIR}/extern/mbed-os/features/filesystem/fat/ChaN"
	"${ROOT_DIR}/extern/mbed-os/features/filesystem/bd"
	"${ROOT_DIR}/extern/mbed-os/features/filesystem/"
	"${ROOT_DIR}/extern/mbed-os/features/filesystem/littlefs"
	"${ROOT_DIR}/extern/mbed-os/features/filesystem/littlefs/littlefs"
	"${ROOT_DIR}/extern/mbed-os/connectivity/cellular/include/cellular/framework/API"
	"${ROOT_DIR}/extern/mbed-os/connectivity/cellular/include/cellular/framework/AT"
	"${ROOT_DIR}/extern/mbed-os/connectivity/cellular/include/cellular/framework/device"
	"${ROOT_DIR}/extern/mbed-os/connectivity/cellular/include/cellular/framework"
	"${ROOT_DIR}/extern/mbed-os/connectivity/cellular/include/cellular/framework/common"
	"${ROOT_DIR}/extern/mbed-os/connectivity"
	"${ROOT_DIR}/extern/mbed-os/connectivity/lorawan/include/lorawan"
	"${ROOT_DIR}/extern/mbed-os/connectivity/lorawan/lorastack"
	"${ROOT_DIR}/extern/mbed-os/connectivity/lorawan/lorastack/mac"
	"${ROOT_DIR}/extern/mbed-os/connectivity/lorawan/lorastack/phy"
	"${ROOT_DIR}/extern/mbed-os/connectivity/lorawan"
	"${ROOT_DIR}/extern/mbed-os/connectivity/mbedtls"
	"${ROOT_DIR}/extern/mbed-os/connectivity/mbedtls/include"
)

# create final library target which refers to MBed OS stubs
add_library(mbed-os INTERFACE)
target_compile_options(mbed-os INTERFACE -include${ROOT_DIR}/tests/unit/utest/mbed_config_utest.h)
target_include_directories(mbed-os INTERFACE ${unittest_includes_base})

# fix error with missing types when building for MinGW
set(CMAKE_EXTRA_INCLUDE_FILES "sys/types.h")
check_all_types(uid_t gid_t nlink_t)
if(NOT HAVE_UID_T)
	target_compile_definitions(mbed-os INTERFACE uid_t=int)
endif()
if(NOT HAVE_GID_T)
	target_compile_definitions(mbed-os INTERFACE gid_t=int)
endif()
if(NOT HAVE_NLINK_T)
	target_compile_definitions(mbed-os INTERFACE nlink_t=int)
endif()
