cmake_minimum_required(VERSION 3.13)

project(mitsos)

pico_sdk_init()

add_executable(mitsos)

target_sources(mitsos PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/mitsos.c
	${CMAKE_CURRENT_LIST_DIR}/mitsos_stdio.c
	${CMAKE_CURRENT_LIST_DIR}/mitsos_usb.c)

target_link_libraries(mitsos PUBLIC
	mitsoxmlib
	pico_stdlib
	tinyusb_device
	tinyusb_board)

target_include_directories(mitsos PUBLIC
	${CMAKE_CURRENT_LIST_DIR})

pico_enable_stdio_usb(mitsos 1)

pico_add_extra_outputs(mitsos)
