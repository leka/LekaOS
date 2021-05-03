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

#ifndef _MBED_HTTP_REQUEST_BUILDER_H_
#define _MBED_HTTP_REQUEST_BUILDER_H_

#include <string>
#include <map>
#include "http_parser.h"
#include "http_parsed_url.h"

class HttpRequestBuilder {
public:
    HttpRequestBuilder(http_method a_method, ParsedUrl* a_parsed_url)
        : method(a_method), parsed_url(a_parsed_url)
    {
        set_header("Host", string(parsed_url->host()));
    }

    /**
     * Set a header for the request
     * If the key already exists, it will be overwritten...
     */
    void set_header(string key, string value) {
        map<string, string>::iterator it = headers.find(key);

        if (it != headers.end()) {
            it->second = value;
        }
        else {
            headers.insert(headers.end(), pair<string, string>(key, value));
        }
    }

    char* build(const void* body, size_t body_size, size_t &size) {
        const char* method_str = http_method_str(method);

        if (method == HTTP_POST || method == HTTP_PUT || method == HTTP_DELETE || body_size > 0) {
            char buffer[10];
            snprintf(buffer, 10, "%d", body_size);
            set_header("Content-Length", string(buffer));
        }

        size = 0;

        // first line is METHOD PATH+QUERY HTTP/1.1\r\n
        size += strlen(method_str) + 1 + strlen(parsed_url->path()) + (strlen(parsed_url->query()) ? strlen(parsed_url->query()) + 1 : 0) + 1 + 8 + 2;

        // after that we'll do the headers
        typedef map<string, string>::iterator it_type;
        for(it_type it = headers.begin(); it != headers.end(); it++) {
            // line is KEY: VALUE\r\n
            size += it->first.length() + 1 + 1 + it->second.length() + 2;
        }

        // then the body, first an extra newline
        size += 2;

        // body
        size += body_size;

        // extra newline
        size += 2;

        // Now let's print it
        char* req = (char*)calloc(size + 1, 1);
        char* originalReq = req;

        if (strlen(parsed_url->query())) {
            sprintf(req, "%s %s?%s HTTP/1.1\r\n", method_str, parsed_url->path(), parsed_url->query());
        } else {
            sprintf(req, "%s %s%s HTTP/1.1\r\n", method_str, parsed_url->path(), parsed_url->query());
        }
        req += strlen(method_str) + 1 + strlen(parsed_url->path()) + (strlen(parsed_url->query()) ? strlen(parsed_url->query()) + 1 : 0) + 1 + 8 + 2;

        typedef map<string, string>::iterator it_type;
        for(it_type it = headers.begin(); it != headers.end(); it++) {
            // line is KEY: VALUE\r\n
            sprintf(req, "%s: %s\r\n", it->first.c_str(), it->second.c_str());
            req += it->first.length() + 1 + 1 + it->second.length() + 2;
        }

        sprintf(req, "\r\n");
        req += 2;

        if (body_size > 0) {
            memcpy(req, body, body_size);
        }
        req += body_size;

        sprintf(req, "\r\n");
        req += 2;

        // Uncomment to debug...
        // printf("----- BEGIN REQUEST -----\n");
        // printf("%s", originalReq);
        // printf("----- END REQUEST -----\n");

        return originalReq;
    }

private:
    http_method method;
    ParsedUrl* parsed_url;
    map<string, string> headers;
};

#endif // _MBED_HTTP_REQUEST_BUILDER_H_
