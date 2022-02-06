cmake_minimum_required(VERSION 3.13)

project(mitsoxmlib)

add_library(mitsoxmlib
	${CMAKE_CURRENT_LIST_DIR}/mxml-attr.c
	${CMAKE_CURRENT_LIST_DIR}/mxml-entity.c
	${CMAKE_CURRENT_LIST_DIR}/mxml-file.c
	${CMAKE_CURRENT_LIST_DIR}/mxml-get.c
	${CMAKE_CURRENT_LIST_DIR}/mxml-index.c
	${CMAKE_CURRENT_LIST_DIR}/mxml-node.c
	${CMAKE_CURRENT_LIST_DIR}/mxml-private.c
	${CMAKE_CURRENT_LIST_DIR}/mxml-search.c
	${CMAKE_CURRENT_LIST_DIR}/mxml-set.c
	${CMAKE_CURRENT_LIST_DIR}/mxml-string.c)
