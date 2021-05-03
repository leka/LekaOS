# HTTP and HTTPS library for mbed OS 5

This library is used to make HTTP and HTTPS calls from mbed OS 5 applications.

## HTTP Request API

```cpp
NetworkInterface* network = /* obtain a NetworkInterface object */

const char body[] = "{\"hello\":\"world\"}";

HttpRequest* request = new HttpRequest(network, HTTP_POST, "http://httpbin.org/post");
request->set_header("Content-Type", "application/json");
HttpResponse* response = request->send(body, strlen(body));
// if response is NULL, check response->get_error()

printf("status is %d - %s\n", response->get_status_code(), response->get_status_message());
printf("body is:\n%s\n", response->get_body_as_string().c_str());

delete request; // also clears out the response
```

## HTTPS Request API

```cpp
// pass in the root certificates that you trust, there is no central CA registry in mbed OS
const char SSL_CA_PEM[] = "-----BEGIN CERTIFICATE-----\n"
    /* rest of the CA root certificates */;

NetworkInterface* network = /* obtain a NetworkInterface object */

const char body[] = "{\"hello\":\"world\"}";

HttpsRequest* request = new HttpsRequest(network, SSL_CA_PEM, HTTP_GET "https://httpbin.org/status/418");
HttpResponse* response = request->send();
// if response is NULL, check response->get_error()

printf("status is %d - %s\n", response->get_status_code(), response->get_status_message());
printf("body is:\n%s\n", response->get_body().c_str());

delete request;
```

## Dealing with large body

By default the library will store the full request body on the heap. This works well for small responses, but you'll run out of memory when receiving a large response body. To mitigate this you can pass in a callback as the last argument to the request constructor. This callback will be called whenever a chunk of the body is received. You can set the request chunk size in the `HTTP_RECEIVE_BUFFER_SIZE` macro (see `mbed_lib.json` for the definition) although it also depends on the buffer size of the underlying network connection.

```cpp
void body_callback(const char* data, size_t data_len) {
    // do something with the data
}

HttpRequest* req = new HttpRequest(network, HTTP_GET, "http://pathtolargefile.com");
req->send(NULL, 0, body_callback);
```

## Socket re-use

By default the library opens a new socket per request. This is wasteful, especially when dealing with TLS requests. You can re-use sockets like this:

### HTTP

```cpp
TCPSocket* socket = new TCPSocket();

nsapi_error_t open_result = socket->open(network);
// check open_result

nsapi_error_t connect_result = socket->connect("httpbin.org", 80);
// check connect_result

// Pass in `socket`, instead of `network` as first argument
HttpRequest* req = new HttpRequest(socket, HTTP_GET, "http://httpbin.org/status/418");
```

### HTTPS

```cpp
TLSSocket* socket = new TLSSocket(network, "httpbin.org", 443, SSL_CA_PEM);
socket->set_debug(true);
if (socket->connect() != 0) {
    printf("TLS Connect failed %d\n", socket->error());
    return 1;
}

// Pass in `socket`, instead of `network` as first argument, and omit the `SSL_CA_PEM` argument
HttpsRequest* get_req = new HttpsRequest(socket, HTTP_GET, "https://httpbin.org/status/418");
```

## Tested on

* K64F with Ethernet.
* NUCLEO_F411RE with ESP8266.
