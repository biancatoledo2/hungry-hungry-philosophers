#
# CPSC 351-02 Summer 2018 Session B
# Assignment 3 Dining Philosophers
# Michael Shafae
# mshafae at fullerton edu
#
# GNU Makefile for the Dining Philosophers assingment.
#

TARGET = philo
CFILES = dining_philisophers.c philosopher.c utilities.c
HEADERS = philosopher.h utilities.h
CFLAGS += -g -Wall -pedantic -pipe -D_GNU_SOURCE
LDFLAGS += -g -Wall -pipe
LLDLIBS = -lpthread
OBJECTS = $(CFILES:.c=.o)
DEP = $(CFILES:.c=.d)

default all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(LDFLAGS) -o $(TARGET) $(OBJECTS) $(LLDLIBS)

-include $(DEP)

%.d: %.c
	set -e; $(CC) -MM $(CFLAGS) $< \
	| sed 's/($*).o[ :]*/.o $@ : /g' > $@; \
	[ -s $@ ] || rm -f $@

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	-rm -f $(OBJECTS) core $(TARGET).core *~

spotless: clean
	-rm -f $(TARGET) $(DEP)

