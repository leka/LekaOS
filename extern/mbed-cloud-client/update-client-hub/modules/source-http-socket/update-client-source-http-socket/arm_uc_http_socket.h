// ----------------------------------------------------------------------------
// Copyright 2016-2017 ARM Ltd.
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

#ifndef UPDATE_CLIENT_SOURCE_HTTP_SOCKET_H
#define UPDATE_CLIENT_SOURCE_HTTP_SOCKET_H

#include "update-client-common/arm_uc_common.h"

#include <pal.h>

/**
 * @brief Events passed to event handler.
 * @details EVENT_HASH Get hash complete.
 *          EVENT_DATE Get date complete.
 *          EVENT_DOWNLOAD_PENDING Download complete with data pending.
 *          EVENT_DOWNLOAD_DONE Download complete, all done.
 */
typedef enum {
    UCS_HTTP_EVENT_HASH,
    UCS_HTTP_EVENT_DATE,
    UCS_HTTP_EVENT_DOWNLOAD,
    UCS_HTTP_EVENT_ERROR,
    UCS_HTTP_EVENT_ERROR_BUFFER_SIZE
} arm_ucs_http_event_t;

typedef enum {
    RQST_TYPE_NONE, // to indicate idle
    RQST_TYPE_HASH_ETAG,
    RQST_TYPE_HASH_DATE,
    RQST_TYPE_GET_FILE,
    RQST_TYPE_GET_FRAG
} arm_uc_http_rqst_t;

typedef enum {
    STATE_DISCONNECTED,
    STATE_CONNECTING,
    STATE_PROCESS_HEADER,
    STATE_PROCESS_BODY,
    STATE_CONNECTED_IDLE
} arm_uc_http_socket_state_t;

typedef enum {
    SOCKET_EVENT_UNDEFINED,
    SOCKET_EVENT_INITIATE,
    SOCKET_EVENT_LOOKUP_START,
    SOCKET_EVENT_LOOKUP_WAITING,
    SOCKET_EVENT_LOOKUP_BLOCKED,
    SOCKET_EVENT_LOOKUP_FAILED,
    SOCKET_EVENT_LOOKUP_DONE,
    SOCKET_EVENT_CONNECT_START,
    SOCKET_EVENT_CONNECT_BLOCKED,
    SOCKET_EVENT_CONNECT_DONE,
    SOCKET_EVENT_SEND_START,
    SOCKET_EVENT_SEND_BLOCKED,
    SOCKET_EVENT_SEND_DONE,
    SOCKET_EVENT_HEADER_START,
    SOCKET_EVENT_HEADER_MORE,
    SOCKET_EVENT_HEADER_BLOCKED,
    SOCKET_EVENT_HEADER_DONE,
    SOCKET_EVENT_FRAG_START,
    SOCKET_EVENT_FRAG_MORE,
    SOCKET_EVENT_FRAG_BLOCKED,
    SOCKET_EVENT_FRAG_DONE,
    SOCKET_EVENT_RECEIVE_DONE,
    SOCKET_EVENT_RESTART,
    SOCKET_EVENT_TIMER_FIRED,
    SOCKET_EVENT_RESUME_WAITING,
    SOCKET_EVENT_RESUME_IDLE,
    SOCKET_EVENT_RESUME_INTERVAL,
    SOCKET_EVENT_RESUME_ATTEMPT,
    SOCKET_EVENT_RESUME_TERMINATED,
    SOCKET_EVENT_RESUME_ERROR
} arm_uc_http_socket_event_t;

/**
 * @brief Prototype for event handler.
 */
typedef void (*ARM_UCS_HttpEvent_t)(uintptr_t event);


// Number of fragments in a burst, it is not required to use one of these values.
#define ARM_UC_MULTI_FRAGS_PER_HTTP_BURST__DISABLED         0   // do not use http fragments. Do only single request
#define ARM_UC_MULTI_FRAGS_PER_HTTP_BURST__ONE              1
#define ARM_UC_MULTI_FRAGS_PER_HTTP_BURST__LIGHT            4
#define ARM_UC_MULTI_FRAGS_PER_HTTP_BURST__MILD             16
#define ARM_UC_MULTI_FRAGS_PER_HTTP_BURST__MODERATE         64
#define ARM_UC_MULTI_FRAGS_PER_HTTP_BURST__HEAVY            256
#define ARM_UC_MULTI_FRAGS_PER_HTTP_BURST__EXTREME          1024

#if !defined(ARM_UC_MULTI_FRAGS_PER_HTTP_BURST)
#if defined(TARGET_IS_PC_LINUX)
#define ARM_UC_MULTI_FRAGS_PER_HTTP_BURST           ARM_UC_MULTI_FRAGS_PER_HTTP_BURST__DISABLED
#else
#define ARM_UC_MULTI_FRAGS_PER_HTTP_BURST           ARM_UC_MULTI_FRAGS_PER_HTTP_BURST__MODERATE
#endif
#endif

// Developer-facing #defines allow easier testing of parameterised resume.
// If not available, it becomes extremely difficult to detect exactly when the resume
//   functionality is taking place, or to set values outside of the assumed 'reasonable'
//   range (which can't predict all use cases), which hampers assessment of the settings.

// Print very high priority messages about resume activity for debugging.
// Also, disable checks on resume initialization values.
// Normally compiler errors out if checks enabled and out of permissible range.
#define ARM_UC_HTTP_RESUME_DEFAULT_ATTEMPT_TEST_MESSAGES_ENABLE 0

// !do not modify or delete these definitions!
// default configuration values for HTTP resume functionality.
// to modify from default values, declare as below but without _DEFAULT
//   eg. ARM_UC_HTTP_RESUME_EXPONENTIATION_FACTOR   3

#define ARM_UC_HTTP_RESUME_DEFAULT_EXPONENTIATION_FACTOR        2
#define ARM_UC_HTTP_RESUME_DEFAULT_INITIAL_DELAY_SECS           5
#define ARM_UC_HTTP_RESUME_DEFAULT_MAXIMUM_DELAY_SECS           (60*60)
#define ARM_UC_HTTP_RESUME_DEFAULT_MAXIMUM_DOWNLOAD_TIME_SECS   (7*24*60*60)

typedef struct {
    /* external callback handler */
    ARM_UCS_HttpEvent_t callback_handler;

    /* location */
    arm_uc_uri_t *request_uri;
    arm_uc_uri_t *open_request_uri;

    /* buffer to store downloaded data */
    arm_uc_buffer_t *request_buffer;

    /* fragment offset in a multi-fragment download */
    uint32_t request_offset;
    uint32_t open_request_offset;

    /* request type */
    arm_uc_http_rqst_t request_type;
    arm_uc_http_rqst_t open_request_type;

    /* internal state */

    /* remaining bytes in request */
    uint32_t open_burst_requested;
    uint32_t open_burst_expected;
    uint32_t open_burst_received;

    uint32_t header_end_index;
    uint32_t number_of_pieces;

    /* socket and socket timer management */
    arm_uc_http_socket_state_t socket_state;
    palTimerID_t socket_timeout_timer_id;
    bool socket_timeout_timer_is_running;

    /* socket event future and history */
    arm_uc_http_socket_event_t resume_socket_phase;
    arm_uc_http_socket_event_t expected_socket_event;

    /* pointer to socket */
    palSocket_t socket;

    /* cache for storing DNS lookup */
    palSocketAddress_t cache_address;
    palSocketLength_t cache_address_length;
} arm_uc_http_socket_context_t;

/**
 * @brief Initialize Http module.
 * @details A memory struct is passed as well as a function pointer for event
 *          handling.
 *
 * @param context Struct holding all global variables.
 * @param handler Event handler for signaling when each operation is complete.
 * @return Error code.
 */
arm_uc_error_t ARM_UCS_HttpSocket_Initialize(arm_uc_http_socket_context_t *context,
                                             ARM_UCS_HttpEvent_t handler);

/**
 * @brief Resets HTTP socket to uninitialized state and clears memory struct.
 * @details HTTP sockets must be initialized again before use.
 * @return Error code.
 */
arm_uc_error_t ARM_UCS_HttpSocket_Terminate(void);

/**
 * @brief Get hash for resource at URI.
 * @details Store hash in provided buffer. Enclosing "" and '\0' are removed.
 *
 *          Event generated: EVENT_HASH
 *
 * @param uri Pointer to struct with resource location.
 * @param buffer Pointer to structure with buffer location, maxSize, and size.
 * @return Error code.
 */
arm_uc_error_t ARM_UCS_HttpSocket_GetHash(arm_uc_uri_t *uri, arm_uc_buffer_t *buffer);

/**
 * @brief Get date for resource at URI.
 * @details Store Last-Modified data in provided buffer. Enclosing "" and '\0' are removed.
 *
 *          Event generated: EVENT_DATE
 *
 * @param uri Pointer to struct with resource location.
 * @param buffer Pointer to structure with buffer location, maxSize, and size.
 * @return Error code.
 */
arm_uc_error_t ARM_UCS_HttpSocket_GetDate(arm_uc_uri_t *uri, arm_uc_buffer_t *buffer);

/**
 * @brief Get full resource at URI.
 * @details Download resource at URI and store in provided buffer.
 *          If the provided buffer is not big enough to contain the whole resource
 *          what can fit in the buffer will be downloaded.
 *          The user can then use GetFragment to download the rest.

 *          Events generated: EVENT_DOWNLOAD_PENDING if there is still data to
 *          download and EVENT_DOWNLOAD_DONE if the file is completely downloaded.
 *
 * @param uri Pointer to structure with resource location.
 * @param buffer Pointer to structure with buffer location, maxSize, and size.
 * @return Error code.
 */
arm_uc_error_t ARM_UCS_HttpSocket_GetFile(arm_uc_uri_t *uri, arm_uc_buffer_t *buffer);

/**
 * @brief Get partial resource at URI.
 * @details Download resource at URI from given offset and store in buffer.
 *
 *          The buffer maxSize determines how big a fragment to download. If the
 *          buffer is larger than the requested fragment (offset to end-of-file)
 *          the buffer size is set to indicate the number of available bytes.
 *
 *          Events generated: EVENT_DOWNLOAD_PENDING if there is still data to
 *          download and EVENT_DOWNLOAD_DONE if the file is completely downloaded.
 *
 * @param uri Pointer to structure with resource location.
 * @param buffer Pointer to structure with buffer location, maxSize, and size.
 * @param offset Offset in resource to begin download from.
 * @return Error code.
 */
arm_uc_error_t ARM_UCS_HttpSocket_GetFragment(arm_uc_uri_t *uri, arm_uc_buffer_t *buffer, uint32_t offset);

/**
 * @brief Cancel resume engine externally
 * @param unused Unused.
 */
void ARM_UC_HttpSocket_EndResume(void);

#endif /* UPDATE_CLIENT_SOURCE_HTTP_SOCKET_H */
