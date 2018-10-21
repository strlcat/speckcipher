PROGS = specktest speckcrypt speckblktst
SUPPORT = mhexdump.c xstrlcpy.c
PROGSRCS = $(PROGS:=.c)
PROGOBJS = $(PROGSRCS:.c=.o)
SRCS = $(filter-out $(PROGSRCS) $(SUPPORT), $(wildcard *.c))
HDRS = $(wildcard *.h)
SUPPOBJS = $(SUPPORT:.c=.o)
OBJS = $(SRCS:.c=.o)

ifneq (,$(DEBUG))
override CFLAGS+=-Wall -O0 -g
else
override CFLAGS+=-O3
endif

default: $(OBJS) libspeck.a libsupp.a speckcrypt
all: $(OBJS) libspeck.a libsupp.a $(PROGS)

%.o: %.c $(HDRS)
	$(CC) $(CFLAGS) -c -o $@ $<

libspeck.a: $(OBJS)
	$(AR) cru $@ $^

libsupp.a: $(SUPPOBJS)
	$(AR) cru $@ $^

$(PROGS): %: %.o libspeck.a libsupp.a
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f libspeck.a libsupp.a $(OBJS) $(PROGOBJS) $(SUPPOBJS) $(PROGS)
