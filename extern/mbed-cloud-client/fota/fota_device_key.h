// ----------------------------------------------------------------------------
// Copyright 2021 Pelion Ltd.
//
// SPDX-License-Identifier: Apache-2.0
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
// ----------------------------------------------------------------------------

#ifndef FOTA_DEVICE_KEY
#define FOTA_DEVICE_KEY

#ifdef __MBED__

#ifdef __cplusplus
extern "C" {
#endif

#if ((MBED_CLOUD_CLIENT_FOTA_FW_HEADER_VERSION == 2) && (MBED_CLOUD_CLIENT_FOTA_FW_HEADER_EXTERNAL == 1)) || (MBED_CLOUD_CLIENT_FOTA_KEY_ENCRYPTION == FOTA_USE_DEVICE_KEY)

int8_t fota_get_device_key_128bit(uint8_t *key, uint32_t keyLenBytes);

#endif // #if (MBED_CLOUD_CLIENT_FOTA_FW_HEADER_VERSION == 2) || (MBED_CLOUD_CLIENT_FOTA_KEY_ENCRYPTION == FOTA_USE_DEVICE_KEY)

#ifdef __cplusplus
}
#endif

#endif

#endif // FOTA_DEVICE_KEY
