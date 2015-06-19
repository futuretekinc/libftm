CC=arm-openwrt-linux-uclibcgnueabi-gcc
AR=arm-openwrt-linux-uclibcgnueabi-ar
CFLAGS=-c -fPIC -Wall -DDEBUG \
		-I. \
		-I../include \
		-I../ftdm \
		-I../ftnm \
		-I../build_package/libconfig-1.4.9/include 
LDFLAGS= -L. -L../build_package/libconfig-1.4.9/lib

LIB_FTM=libftm
LIB_FTM_INCS= ftm_types.h ftm_object.h simclist.h ftm_debug.h ftm_list.h ftm_console.h 
LIB_FTM_SRCS= simclist.c  ftm_debug.c ftm_object.c ftm_list.c ftm_console.c ftm_mem.c ftm_value.c
LIB_FTM_OBJS=$(LIB_FTM_SRCS:.c=.o)

all: $(LIB_FTM)

$(LIB_FTM): $(LIB_FTM_OBJS)
	$(AR) rcs $(LIB_FTM).a $(LIB_FTM_OBJS)
	$(CC) -shared -o $(LIB_FTM).so.1.0.1 $(LIB_FTM_OBJS) $(LDFLAGS)

.c.o:  *.c *.h
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(LIB_FTM).so.1.0.1 *.a *.o
