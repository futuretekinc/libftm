include ./config.mk

.PHONY : really clean install

MOSQ_OBJS= 	simclist.o\
			ftm_debug.o\
			ftm_object.o\
			ftm_list.o\
			ftm_console.o\
			ftm_mem.o\
			ftm_value.o


all : libftm.so.${SOVERSION} libftm.a

install : all
	$(INSTALL) -d ${DESTDIR}$(prefix)/lib${LIB_SUFFIX}/
	$(INSTALL) -s --strip-program=${CROSS_COMPILE}${STRIP} libftm.so.${SOVERSION} ${DESTDIR}${prefix}/lib${LIB_SUFFIX}/libftm.so.${SOVERSION}
	ln -sf libftm.so.${SOVERSION} ${DESTDIR}${prefix}/lib${LIB_SUFFIX}/libftm.so
	$(INSTALL) -d ${DESTDIR}${prefix}/include/
	$(INSTALL) ftm.h ${DESTDIR}${prefix}/include/ftm.h

uninstall :
	-rm -f ${DESTDIR}${prefix}/lib${LIB_SUFFIX}/libftm.so.${SOVERSION}
	-rm -f ${DESTDIR}${prefix}/lib${LIB_SUFFIX}/libftm.so
	-rm -f ${DESTDIR}${prefix}/include/ftm.h

reallyclean : clean

clean :
	-rm -f *.o libftm.so.${SOVERSION} libftm.so libftm.a

libftm.so.${SOVERSION} : ${MOSQ_OBJS}
	${CROSS_COMPILE}$(CC) -shared $(LIB_LDFLAGS) $^ -o $@ ${LIB_LIBS}

libftm.a : ${MOSQ_OBJS}
	${CROSS_COMPILE}$(AR) cr $@ $^
simclist.o: simclist.c simclist.h
	${CROSS_COMPILE}$(CC) $(LIB_CFLAGS) -c $< -o $@

ftm_debug.o : ftm_debug.c ftm_debug.h
	${CROSS_COMPILE}$(CC) $(LIB_CFLAGS) -c $< -o $@

ftm_object.o : ftm_object.c ftm_object.h
	${CROSS_COMPILE}$(CC) $(LIB_CFLAGS) -c $< -o $@

ftm_list.o : ftm_list.c ftm_list.h
	${CROSS_COMPILE}$(CC) $(LIB_CFLAGS) -c $< -o $@

ftm_console.o : ftm_console.c ftm_console.h
	${CROSS_COMPILE}$(CC) $(LIB_CFLAGS) -c $< -o $@

ftm_mem.o : ftm_mem.c ftm_mem.h
	${CROSS_COMPILE}$(CC) $(LIB_CFLAGS) -c $< -o $@

ftm_value.o : ftm_value.c ftm_value.h
	${CROSS_COMPILE}$(CC) $(LIB_CFLAGS) -c $< -o $@

