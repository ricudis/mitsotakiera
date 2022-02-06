cmake_minimum_required(VERSION 3.13)

project(takis)

pico_sdk_init()

add_executable(takis takis.c)

target_sources(takis PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/takis.c)

target_link_libraries(takis PUBLIC
	mitsoxmlib
	pico_stdlib
	tinyusb_host
	tinyusb_board)

target_include_directories(takis PUBLIC
	${CMAKE_CURRENT_LIST_DIR})

pico_enable_stdio_usb(takis 0)
pico_enable_stdio_uart(takis 1)

pico_add_extra_outputs(takis)
