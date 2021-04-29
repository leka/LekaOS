/*******************************************************************************
 * Copyright 2016, 2017 ARM Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *******************************************************************************/

#include "unity.h"
#include "unity_fixture.h"
#include "pal.h"
#define PAL_RUN_ALL_TESTS 1

// pal Socket API tests
TEST_GROUP_RUNNER(pal_socket)
{
#ifndef PAL_SKIP_TEST_MODULE_NETWORK
    RUN_TEST_CASE(pal_socket, socketUDPCreationOptionsTest);
    RUN_TEST_CASE(pal_socket, basicTCPclientSendRecieve);
    RUN_TEST_CASE(pal_socket, basicUDPclientSendRecieve);
    RUN_TEST_CASE(pal_socket, basicSocketScenario3);
    RUN_TEST_CASE(pal_socket, tProvUDPTest);
    RUN_TEST_CASE(pal_socket, nonBlockingAsyncTest);
    RUN_TEST_CASE(pal_socket, ServerSocketScenario);
    RUN_TEST_CASE(pal_socket, socketTCPBufferedSmall);
    RUN_TEST_CASE(pal_socket, socketTCPBufferedLarge);
    RUN_TEST_CASE(pal_socket, socketUDPBufferedSmall);
    RUN_TEST_CASE(pal_socket, socketUDPBufferedLarge);
    RUN_TEST_CASE(pal_socket, getAddressInfoAsync);
    RUN_TEST_CASE(pal_socket, socketApiInputParamValidation);
    RUN_TEST_CASE(pal_socket, keepaliveOn);
    RUN_TEST_CASE(pal_socket, keepaliveOff);
    RUN_TEST_CASE(pal_socket, tcp_echo);
    RUN_TEST_CASE(pal_socket, udp_echo);
    RUN_TEST_CASE(pal_socket, interfaceStatusListener);
#endif
}
