# Leka - LekaOS
# Copyright 2021 APF France handicap
# SPDX-License-Identifier: Apache-2.0

#
# MARK: - General
#

RUNNER_HOME=$(eval echo "~")
echo "RUNNER_HOME=$RUNNER_HOME" >> $GITHUB_ENV

CACHE_NOW_TIME=$(date +%s)
echo "CACHE_NOW_TIME=$CACHE_NOW_TIME" >> $GITHUB_ENV

CLICOLOR_FORCE="true" # ninja
echo "CLICOLOR_FORCE=$CLICOLOR_FORCE" >> $GITHUB_ENV

#
# MARK: - Base/Head data
#

echo "BASE_REF=$BASE_REF" >> $GITHUB_ENV
echo "HEAD_REF=$HEAD_REF" >> $GITHUB_ENV

git checkout $BASE_REF

BASE_SHA=$(git rev-parse --short HEAD)
BASE_MBED_VERSION=$(cat config/mbed_version)
BASE_MCUBOOT_VERSION=$(cat config/mcuboot_version)
BASE_CXX_STANDARD="-std=c++$(cat CMakeLists.txt | grep -Po '(?<=CMAKE_CXX_STANDARD\s)[0-9]*')"

echo "BASE_SHA=$BASE_SHA" >> $GITHUB_ENV
echo "BASE_MBED_VERSION=$BASE_MBED_VERSION" >> $GITHUB_ENV
echo "BASE_MCUBOOT_VERSION=$BASE_MCUBOOT_VERSION" >> $GITHUB_ENV
echo "BASE_CXX_STANDARD=$BASE_CXX_STANDARD" >> $GITHUB_ENV

git checkout $HEAD_REF

HEAD_SHA=$(git rev-parse --short HEAD)
HEAD_MBED_VERSION=$(cat config/mbed_version)
HEAD_MCUBOOT_VERSION=$(cat config/mcuboot_version)
HEAD_CXX_STANDARD="-std=c++$(cat CMakeLists.txt | grep -Po '(?<=CMAKE_CXX_STANDARD\s)[0-9]*')"

echo "HEAD_SHA=$HEAD_SHA" >> $GITHUB_ENV
echo "HEAD_MBED_VERSION=$HEAD_MBED_VERSION" >> $GITHUB_ENV
echo "HEAD_MCUBOOT_VERSION=$HEAD_MCUBOOT_VERSION" >> $GITHUB_ENV
echo "HEAD_CXX_STANDARD=$HEAD_CXX_STANDARD" >> $GITHUB_ENV

#
# MARK: - ccache
#

CCACHE_DIR="/home/runner/work/ccache"
CCACHE_COMPRESS="true"
CCACHE_COMPRESSLEVEL=6
CCACHE_COMPILERCHECK="content"
CCACHE_LOGFILE="/home/runner/work/ccache/my_ccache.log"

echo "CCACHE_DIR=$CCACHE_DIR" >> $GITHUB_ENV
echo "CCACHE_COMPRESS=$CCACHE_COMPRESS" >> $GITHUB_ENV
echo "CCACHE_COMPRESSLEVEL=$CCACHE_COMPRESSLEVEL" >> $GITHUB_ENV
echo "CCACHE_COMPILERCHECK=$CCACHE_COMPILERCHECK" >> $GITHUB_ENV
echo "CCACHE_LOGFILE=$CCACHE_LOGFILE" >> $GITHUB_ENV

#
# MARK: - ARM GCC Toolchain
#

ARM_TOOLCHAIN_FILENAME="gcc-arm-none-eabi-*-x86_64-linux.tar.bz2"
ARM_TOOLCHAIN_EXTRACT_DIRECTORY="gcc-arm-none-eabi-*"
ARM_TOOLCHAIN_URL=$(cat config/toolchain_gcc_arm_none_eabi_url)

echo "ARM_TOOLCHAIN_FILENAME=$ARM_TOOLCHAIN_FILENAME" >> $GITHUB_ENV
echo "ARM_TOOLCHAIN_EXTRACT_DIRECTORY=$ARM_TOOLCHAIN_EXTRACT_DIRECTORY" >> $GITHUB_ENV
echo "ARM_TOOLCHAIN_URL=$ARM_TOOLCHAIN_URL" >> $GITHUB_ENV

#
# MARK: - SonarCloud
#

SONARCLOUD_CLI_URL="https://binaries.sonarsource.com/Distribution/sonar-scanner-cli/sonar-scanner-cli-4.7.0.2747.zip"
SONARCLOUD_CLI_FILENAME="sonar-scanner-cli-*.zip"
SONARCLOUD_CLI_EXTRACT_DIRECTORY="sonar-scanner-*"

echo "SONARCLOUD_CLI_URL=$SONARCLOUD_CLI_URL" >> $GITHUB_ENV
echo "SONARCLOUD_CLI_FILENAME=$SONARCLOUD_CLI_FILENAME" >> $GITHUB_ENV
echo "SONARCLOUD_CLI_EXTRACT_DIRECTORY=$SONARCLOUD_CLI_EXTRACT_DIRECTORY" >> $GITHUB_ENV

SONARCLOUD_BUILD_WRAPPER_URL="https://sonarcloud.io/static/cpp/build-wrapper-linux-x86.zip"
SONARCLOUD_BUILD_WRAPPER_FILENAME="build-wrapper-linux-x86.zip"
SONARCLOUD_BUILD_WRAPPER_EXTRACT_DIRECTORY="build-wrapper-linux-x86"

echo "SONARCLOUD_BUILD_WRAPPER_URL=$SONARCLOUD_BUILD_WRAPPER_URL" >> $GITHUB_ENV
echo "SONARCLOUD_BUILD_WRAPPER_FILENAME=$SONARCLOUD_BUILD_WRAPPER_FILENAME" >> $GITHUB_ENV
echo "SONARCLOUD_BUILD_WRAPPER_EXTRACT_DIRECTORY=$SONARCLOUD_BUILD_WRAPPER_EXTRACT_DIRECTORY" >> $GITHUB_ENV

SONARCLOUD_CACHE_DIR="/home/runner/work/sonarcloud"

echo "SONARCLOUD_CACHE_DIR=$SONARCLOUD_CACHE_DIR" >> $GITHUB_ENV
