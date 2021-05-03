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

#ifndef _MBED_HTTPS_REQUEST_H_
#define _MBED_HTTPS_REQUEST_H_

#include <string>
#include <vector>
#include <map>
#include "http_parser.h"
#include "http_response.h"
#include "http_request_builder.h"
#include "http_response_parser.h"
#include "http_parsed_url.h"
#include "tls_socket.h"

/**
 * \brief HttpsRequest implements the logic for interacting with HTTPS servers.
 */
class HttpsRequest {
public:
    /**
     * HttpsRequest Constructor
     * Initializes the TCP socket, sets up event handlers and flags.
     *
     * @param[in] net_iface The network interface
     * @param[in] ssl_ca_pem String containing the trusted CAs
     * @param[in] method HTTP method to use
     * @param[in] url URL to the resource
     * @param[in] body_callback Callback on which to retrieve chunks of the response body.
                                If not set, the complete body will be allocated on the HttpResponse object,
                                which might use lots of memory.
     */
    HttpsRequest(NetworkInterface* net_iface,
                 const char* ssl_ca_pem,
                 http_method method,
                 const char* url,
                 Callback<void(const char *at, size_t length)> body_callback = 0)
    {
        _parsed_url = new ParsedUrl(url);
        _body_callback = body_callback;
        _request_builder = new HttpRequestBuilder(method, _parsed_url);
        _response = NULL;
        _debug = false;

        _tlssocket = new TLSSocket(net_iface, _parsed_url->host(), _parsed_url->port(), ssl_ca_pem);
        _we_created_the_socket = true;
    }

    /**
     * HttpsRequest Constructor
     * Sets up event handlers and flags.
     *
     * @param[in] socket A connected TLSSocket
     * @param[in] method HTTP method to use
     * @param[in] url URL to the resource
     * @param[in] body_callback Callback on which to retrieve chunks of the response body.
                                If not set, the complete body will be allocated on the HttpResponse object,
                                which might use lots of memory.
     */
    HttpsRequest(TLSSocket* socket,
                 http_method method,
                 const char* url,
                 Callback<void(const char *at, size_t length)> body_callback = 0)
    {
        _parsed_url = new ParsedUrl(url);
        _body_callback = body_callback;
        _request_builder = new HttpRequestBuilder(method, _parsed_url);
        _response = NULL;
        _debug = false;

        _tlssocket = socket;
        _we_created_the_socket = false;
    }

    /**
     * HttpsRequest Destructor
     */
    ~HttpsRequest() {
        if (_request_builder) {
            delete _request_builder;
        }

        if (_tlssocket && _we_created_the_socket) {
            delete _tlssocket;
        }

        if (_parsed_url) {
            delete _parsed_url;
        }

        if (_response) {
            delete _response;
        }
    }

    /**
     * Execute the HTTPS request.
     *
     * @param[in] body Pointer to the request body
     * @param[in] body_size Size of the request body
     * @return An HttpResponse pointer on success, or NULL on failure.
     *         See get_error() for the error code.
     */
    HttpResponse* send(const void* body = NULL, nsapi_size_t body_size = 0) {
        // not tried to connect before?
        if (_tlssocket->error() != 0) {
            _error = _tlssocket->error();
            return NULL;
        }

        bool socket_was_open = _tlssocket->connected();

        if (!socket_was_open) {
            nsapi_error_t r = _tlssocket->connect();
            if (r != 0) {
                _error = r;
                return NULL;
            }
        }

        int ret;

        size_t request_size = 0;
        char* request = _request_builder->build(body, body_size, request_size);

        ret = mbedtls_ssl_write(_tlssocket->get_ssl_context(), (const unsigned char *) request, request_size);

        free(request);

        if (ret < 0) {
            if (ret != MBEDTLS_ERR_SSL_WANT_READ &&
                ret != MBEDTLS_ERR_SSL_WANT_WRITE) {
                print_mbedtls_error("mbedtls_ssl_write", ret);
                onError(_tlssocket->get_tcp_socket(), -1 );
            }
            else {
                _error = ret;
            }
            return NULL;
        }

        // Create a response object
        _response = new HttpResponse();
        // And a response parser
        HttpResponseParser parser(_response, _body_callback);

        // Set up a receive buffer (on the heap)
        uint8_t* recv_buffer = (uint8_t*)malloc(HTTP_RECEIVE_BUFFER_SIZE);

        /* Read data out of the socket */
        while ((ret = mbedtls_ssl_read(_tlssocket->get_ssl_context(), (unsigned char *) recv_buffer, HTTP_RECEIVE_BUFFER_SIZE)) > 0) {
            // Don't know if this is actually needed, but OK
            size_t _bpos = static_cast<size_t>(ret);
            recv_buffer[_bpos] = 0;

            size_t nparsed = parser.execute((const char*)recv_buffer, _bpos);
            if (nparsed != _bpos) {
                print_mbedtls_error("parser_error", nparsed);
                // parser error...
                _error = -2101;
                free(recv_buffer);
                return NULL;
            }

            if (_response->is_message_complete()) {
                break;
            }
        }
        if (ret < 0) {
            if (ret != MBEDTLS_ERR_SSL_WANT_READ && ret != MBEDTLS_ERR_SSL_WANT_WRITE) {
                print_mbedtls_error("mbedtls_ssl_read", ret);
                onError(_tlssocket->get_tcp_socket(), -1 );
            }
            else {
                _error = ret;
            }
            free(recv_buffer);
            return NULL;
        }

        parser.finish();

        if (!socket_was_open) {
            _tlssocket->get_tcp_socket()->close();
        }

        free(recv_buffer);

        return _response;
    }

    /**
     * Closes the underlying TCP socket
     */
    void close() {
        _tlssocket->get_tcp_socket()->close();
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
        _request_builder->set_header(key, value);
    }

    /**
     * Get the error code.
     *
     * When send() fails, this error is set.
     */
    nsapi_error_t get_error() {
        return _error;
    }

    /**
     * Set the debug flag.
     *
     * If this flag is set, debug information from mbed TLS will be logged to stdout.
     */
    void set_debug(bool debug) {
        _debug = debug;

        _tlssocket->set_debug(debug);
    }


protected:
    /**
     * Helper for pretty-printing mbed TLS error codes
     */
    static void print_mbedtls_error(const char *name, int err) {
        char buf[128];
        mbedtls_strerror(err, buf, sizeof (buf));
        mbedtls_printf("%s() failed: -0x%04x (%d): %s\r\n", name, -err, err, buf);
    }

    void onError(TCPSocket *s, int error) {
        s->close();
        _error = error;
    }

protected:
    TLSSocket* _tlssocket;
    bool _we_created_the_socket;

    Callback<void(const char *at, size_t length)> _body_callback;
    ParsedUrl* _parsed_url;
    HttpRequestBuilder* _request_builder;
    HttpResponse* _response;

    nsapi_error_t _error;
    bool _debug;

};

#endif // _MBED_HTTPS_REQUEST_H_
