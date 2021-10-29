/*
 * Copyright (c) 2020 Embedded Planet
 * Copyright (c) 2020 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
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
 * limitations under the License
 */

#include "mbed_application.h"
#include "mbedtls/platform.h"

#include "drivers/BufferedSerial.h"

#include "LogKit.h"
#include "bootutil/bootutil.h"
#include "bootutil/image.h"
#include "hal/serial_api.h"

static auto serial = mbed::BufferedSerial(USBTX, USBRX, 115200);

auto main() -> int
{
	int rc;

	// log_info("Starting MCUboot");

	// // Initialize mbedtls crypto for use by MCUboot
	// mbedtls_platform_context unused_ctx;
	// rc = mbedtls_platform_setup(&unused_ctx);
	// if (rc != 0) {
	// 	log_error("Failed to setup Mbed TLS, error: %d", rc);
	// 	exit(rc);
	// }

	// struct boot_rsp rsp;
	// rc = boot_go(&rsp);
	// if (rc != 0) {
	// 	log_error("Failed to locate firmware image, error: %d", rc);
	// 	exit(rc);
	// }

	// uint32_t address = rsp.br_image_off + rsp.br_hdr->ih_hdr_size;

	// // Workaround: The extra \n ensures the last trace gets flushed
	// // before mbed_start_application() destroys the stack and jumps
	// // to the application
	// log_info("Booting firmware image at 0x%x\n", address);

	// // Run the application in the primary slot
	// // Add header size offset to calculate the actual start address of application
	// mbed_start_application(address);
}
