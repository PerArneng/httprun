bin_PROGRAMS = httprun
httprun_SOURCES = main.c
httprun_LDADD = -L../httpmachine -lhttpmachine

AM_CFLAGS = -ansi -g -Wall -Wextra -pedantic-errors -pedantic -std=gnu99 
INCLUDES = -I../httpmachine `pkg-config --cflags gthread-2.0` 
AM_LDFLAGS = `pkg-config --libs gthread-2.0`
 
