CFLAGS=-Wall -std=c11
SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)
RT_SRCS=$(wildcard ./rt/*.c)
RT_OBJS=$(RT_SRCS:.c=.o)

9cc: $(OBJS) $(RT_OBJS)
	gcc -o 9cc $(CFLAGS) $(OBJS)

$(OBJS): 9cc.h
$(RT_OBJS): ./rt/lib.h

test: 9cc
	./9cc -test
	./test.sh
clean:
	rm -f 9cc *.o *~ tmp* rt/*.o
