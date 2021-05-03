/*
 * PackageLicenseDeclared: Apache-2.0
 * Copyright (c) 2017 ARM Limited
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
 */

#ifndef _HTTP_REQUEST_
#define _HTTP_REQUEST_

#include <string>
#include <vector>
#include <map>
#include "http_parser.h"
#include "http_response.h"
#include "http_request_builder.h"
#include "http_response_parser.h"
#include "http_parsed_url.h"

/**
 * @todo:
 *      - Userinfo parameter is not handled
 */


/**
 * \brief HttpRequest implements the logic for interacting with HTTPS servers.
 */
class HttpRequest {
public:
    /**
     * HttpRequest Constructor
     *
     * @param[in] aNetwork The network interface
     * @param[in] aMethod HTTP method to use
     * @param[in] url URL to the resource
     * @param[in] aBodyCallback Callback on which to retrieve chunks of the response body.
                                If not set, the complete body will be allocated on the HttpResponse object,
                                which might use lots of memory.
    */
    HttpRequest(NetworkInterface* aNetwork, http_method aMethod, const char* url, Callback<void(const char *at, size_t length)> aBodyCallback = 0)
        : network(aNetwork), method(aMethod), body_callback(aBodyCallback)
    {
        error = 0;
        response = NULL;

        parsed_url = new ParsedUrl(url);
        request_builder = new HttpRequestBuilder(method, parsed_url);

        socket = new TCPSocket();
        we_created_socket = true;
    }

    /**
     * HttpRequest Constructor
     *
     * @param[in] aSocket An open TCPSocket
     * @param[in] aMethod HTTP method to use
     * @param[in] url URL to the resource
     * @param[in] aBodyCallback Callback on which to retrieve chunks of the response body.
                                If not set, the complete body will be allocated on the HttpResponse object,
                                which might use lots of memory.
    */
    HttpRequest(TCPSocket* aSocket, http_method aMethod, const char* url, Callback<void(const char *at, size_t length)> aBodyCallback = 0)
        : socket(aSocket), method(aMethod), body_callback(aBodyCallback)
    {
        error = 0;
        response = NULL;
        network = NULL;

        parsed_url = new ParsedUrl(url);
        request_builder = new HttpRequestBuilder(method, parsed_url);

        we_created_socket = false;
    }

    /**
     * HttpRequest Constructor
     */
    ~HttpRequest() {
        // should response be owned by us? Or should user free it?
        // maybe implement copy constructor on response...
        if (response) {
            delete response;
        }

        if (parsed_url) {
            delete parsed_url;
        }

        if (request_builder) {
            delete request_builder;
        }

        if (socket && we_created_socket) {
            delete socket;
        }
    }

    /**
     * Execute the request and receive the response.
     */
    HttpResponse* send(const void* body = NULL, nsapi_size_t body_size = 0) {
        if (response != NULL) {
            // already executed this response
            error = -2100; // @todo, make a lookup table with errors
            return NULL;
        }

        error = 0;

        if (we_created_socket) {
            nsapi_error_t open_result = socket->open(network);
            if (open_result != 0) {
                error = open_result;
                return NULL;
            }

            nsapi_error_t connection_result = socket->connect(parsed_url->host(), parsed_url->port());
            if (connection_result != 0) {
                error = connection_result;
                return NULL;
            }
        }

        size_t request_size = 0;
        char* request = request_builder->build(body, body_size, request_size);

        nsapi_size_or_error_t send_result = socket->send(request, request_size);

        free(request);

        if (send_result != request_size) {
            error = send_result;
            return NULL;
        }

        // Create a response object
        response = new HttpResponse();
        // And a response parser
        HttpResponseParser parser(response, body_callback);

        // Set up a receive buffer (on the heap)
        uint8_t* recv_buffer = (uint8_t*)malloc(HTTP_RECEIVE_BUFFER_SIZE);

        // TCPSocket::recv is called until we don't have any data anymore
        nsapi_size_or_error_t recv_ret;
        while ((recv_ret = socket->recv(recv_buffer, HTTP_RECEIVE_BUFFER_SIZE)) > 0) {

            // Pass the chunk into the http_parser
            size_t nparsed = parser.execute((const char*)recv_buffer, recv_ret);
            if (nparsed != recv_ret) {
                // printf("Parsing failed... parsed %d bytes, received %d bytes\n", nparsed, recv_ret);
                error = -2101;
                free(recv_buffer);
                return NULL;
            }

            if (response->is_message_complete()) {
                break;
            }
        }
        // error?
        if (recv_ret < 0) {
            error = recv_ret;
            free(recv_buffer);
            return NULL;
        }

        // When done, call parser.finish()
        parser.finish();

        // Free the receive buffer
        free(recv_buffer);

        if (we_created_socket) {
            // Close the socket
            socket->close();
        }

        return response;
    }

    /**
     * Set a header for the request.
     *
     * The 'Host' and 'Content-Length' headers are set automatically.
     * Setting the same header twice will overwrite the previous entry.
     *
     * @param[in] key Header key
     * @param[in] value Header value
     */
    void set_header(string key, string value) {
        request_builder->set_header(key, value);
    }

    /**
     * Get the error code.
     *
     * When send() fails, this error is set.
     */
    nsapi_error_t get_error() {
        return error;
    }

private:
    NetworkInterface* network;
    TCPSocket* socket;
    http_method method;
    Callback<void(const char *at, size_t length)> body_callback;

    ParsedUrl* parsed_url;

    HttpRequestBuilder* request_builder;
    HttpResponse* response;

    bool we_created_socket;

    nsapi_error_t error;
};

#endif // _HTTP_REQUEST_
