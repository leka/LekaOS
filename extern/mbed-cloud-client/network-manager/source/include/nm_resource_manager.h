/*
 * Copyright (c) 2020 ARM Limited. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef NM_RESOURCE_MANAGER_H_
#define NM_RESOURCE_MANAGER_H_

nm_status_t nm_res_manager_create(void *cloud_client_info);
nm_status_t nm_res_manager_get(void *resource_object);
nm_status_t nm_res_manager_set(void *resource_data);
void nm_manager_res_refresh(void);

#endif /* NM_RESOURCE_MANAGER_H_ */
