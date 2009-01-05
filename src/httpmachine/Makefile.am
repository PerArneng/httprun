lib_LIBRARIES = libhttpmachine.a
libhttpmachine_a_SOURCES = http_client_connection.c http_client_connection.h http_machine.c http_machine.h http_request.c http_request.h

AM_CFLAGS = -ansi -g -Wall -Wextra -pedantic-errors -std=gnu99
INCLUDES = `pkg-config --cflags gthread-2.0`
AM_LDFLAGS = `pkg-config --libs gthread-2.0`
