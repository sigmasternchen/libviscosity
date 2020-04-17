CC       = gcc
CFLAGS   = -std=c99 -Wall -D_POSIX_C_SOURCE=201112L -D_XOPEN_SOURCE=500 -D_GNU_SOURCE -static
LD       = gcc
LDFLAGS  = 
AR 		= ar
ARFLAGS  = rcs

OBJS     = obj/viscocity.o
OBJS_SHARED = obj/viscocity-shared.o
DEPS     = $(OBJS:%.o=%.d)

all: libviscocity.so libviscocity.a viscocity.h

libviscocity.so: $(OBJS_SHARED)
	$(LD) $(LDFLAGS) -shared -o $@ $^
	
libviscocity.a: $(OBJS)
	$(AR) $(ARFLAGS) $@ $^
	
viscocity.h: src/viscocity.h
	cp $< $@

-include $(DEPS)

obj/%.o: src/%.c obj
	$(CC) $(CFLAGS) -MMD -c -o $@ $<

obj/%-shared.o: src/%.c obj
	$(CC) $(CFLAGS) -fPIC -MMD -c -o $@ $<

obj:
	@mkdir -p obj

clean:
	@echo "Cleaning up..."
	@rm -f obj/*.o
	@rm -f obj/*.d
	@rm -f libviscocity.so
	@rm -f libviscocity.a
	@rm -f viscocity.h
