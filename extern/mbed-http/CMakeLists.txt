add_library(mbed-http STATIC)

target_include_directories(mbed-http
	PUBLIC
	http_parser
	source
)

target_sources(mbed-http
	PRIVATE
	http_parser/http_parser.c
)
