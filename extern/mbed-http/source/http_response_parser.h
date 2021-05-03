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

#ifndef _HTTP_RESPONSE_PARSER_H_
#define _HTTP_RESPONSE_PARSER_H_

#include "http_parser.h"
#include "http_response.h"

class HttpResponseParser {
public:
    HttpResponseParser(HttpResponse* a_response, Callback<void(const char *at, size_t length)> a_body_callback = 0)
        : response(a_response), body_callback(a_body_callback)
    {
        settings = new http_parser_settings();

        settings->on_message_begin = &HttpResponseParser::on_message_begin_callback;
        settings->on_url = &HttpResponseParser::on_url_callback;
        settings->on_status = &HttpResponseParser::on_status_callback;
        settings->on_header_field = &HttpResponseParser::on_header_field_callback;
        settings->on_header_value = &HttpResponseParser::on_header_value_callback;
        settings->on_headers_complete = &HttpResponseParser::on_headers_complete_callback;
        settings->on_chunk_header = &HttpResponseParser::on_chunk_header_callback;
        settings->on_chunk_complete = &HttpResponseParser::on_chunk_complete_callback;
        settings->on_body = &HttpResponseParser::on_body_callback;
        settings->on_message_complete = &HttpResponseParser::on_message_complete_callback;

        // Construct the http_parser object
        parser = new http_parser();
        http_parser_init(parser, HTTP_RESPONSE);
        parser->data = (void*)this;
    }

    ~HttpResponseParser() {
        if (parser) {
            delete parser;
        }
        if (settings) {
            delete settings;
        }
    }

    size_t execute(const char* buffer, size_t buffer_size) {
        return http_parser_execute(parser, settings, buffer, buffer_size);
    }

    void finish() {
        http_parser_execute(parser, settings, NULL, 0);
    }

private:
    // Member functions
    int on_message_begin(http_parser* parser) {
        return 0;
    }

    int on_url(http_parser* parser, const char *at, size_t length) {
        return 0;
    }

    int on_status(http_parser* parser, const char *at, size_t length) {
        string s(at, length);
        response->set_status(parser->status_code, s);
        return 0;
    }

    int on_header_field(http_parser* parser, const char *at, size_t length) {
        string s(at, length);
        response->set_header_field(s);
        return 0;
    }

    int on_header_value(http_parser* parser, const char *at, size_t length) {
        string s(at, length);
        response->set_header_value(s);
        return 0;
    }

    int on_headers_complete(http_parser* parser) {
        response->set_headers_complete();
        return 0;
    }

    int on_body(http_parser* parser, const char *at, size_t length) {
        response->increase_body_length(length);

        if (body_callback) {
            body_callback(at, length);
            return 0;
        }

        response->set_body(at, length);
        return 0;
    }

    int on_message_complete(http_parser* parser) {
        response->set_message_complete();

        return 0;
    }

    int on_chunk_header(http_parser* parser) {
        response->set_chunked();

        return 0;
    }

    int on_chunk_complete(http_parser* parser) {
        return 0;
    }

    // Static http_parser callback functions
    static int on_message_begin_callback(http_parser* parser) {
        return ((HttpResponseParser*)parser->data)->on_message_begin(parser);
    }

    static int on_url_callback(http_parser* parser, const char *at, size_t length) {
        return ((HttpResponseParser*)parser->data)->on_url(parser, at, length);
    }

    static int on_status_callback(http_parser* parser, const char *at, size_t length) {
        return ((HttpResponseParser*)parser->data)->on_status(parser, at, length);
    }

    static int on_header_field_callback(http_parser* parser, const char *at, size_t length) {
        return ((HttpResponseParser*)parser->data)->on_header_field(parser, at, length);
    }

    static int on_header_value_callback(http_parser* parser, const char *at, size_t length) {
        return ((HttpResponseParser*)parser->data)->on_header_value(parser, at, length);
    }

    static int on_headers_complete_callback(http_parser* parser) {
        return ((HttpResponseParser*)parser->data)->on_headers_complete(parser);
    }

    static int on_body_callback(http_parser* parser, const char *at, size_t length) {
        return ((HttpResponseParser*)parser->data)->on_body(parser, at, length);
    }

    static int on_message_complete_callback(http_parser* parser) {
        return ((HttpResponseParser*)parser->data)->on_message_complete(parser);
    }

    static int on_chunk_header_callback(http_parser* parser) {
        return ((HttpResponseParser*)parser->data)->on_chunk_header(parser);
    }

    static int on_chunk_complete_callback(http_parser* parser) {
        return ((HttpResponseParser*)parser->data)->on_chunk_complete(parser);
    }

    HttpResponse* response;
    Callback<void(const char *at, size_t length)> body_callback;
    http_parser* parser;
    http_parser_settings* settings;
};

#endif // _HTTP_RESPONSE_PARSER_H_
