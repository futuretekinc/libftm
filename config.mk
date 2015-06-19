VERSION=0.1.0
TIMESTAMP:=$(shell date "+%F %T%z")

# Client library SO version. Bump if incompatible API/ABI changes are made.
SOVERSION=1.0.1

UNAME:=$(shell uname -s)

CROSS_COMPILE=arm-openwrt-linux-uclibcgnueabi-
CC=gcc

CFLAGS?= -c -Wall -O2 -DDEBUG -I../build_package/libconfig-1.4.9/include 
LDFLAGS= -L. -L../build_package/libconfig-1.4.9/lib

LIB_CFLAGS:=${CFLAGS} ${CPPFLAGS} 
LIB_CXXFLAGS:=$(LIB_CFLAGS) ${CPPFLAGS}
LIB_LDFLAGS:=${LDFLAGS}

LIB_LIBS:=
PASSWD_LIBS:=

ifeq ($(UNAME),Linux)
	LIB_LIBS:=$(LIB_LIBS) -lrt
endif

LIB_CFLAGS:=$(LIB_CFLAGS) -fPIC 
LIB_CXXFLAGS:=$(LIB_CXXFLAGS) -fPIC

MAKE_ALL:=libftm
ifeq ($(WITH_DOCS),yes)
	MAKE_ALL:=$(MAKE_ALL) docs
endif

INSTALL?=install
prefix=/home/xtra/work/cortina/build_package/libftm
STRIP?=strip

