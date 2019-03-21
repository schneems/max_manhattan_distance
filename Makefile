CC = gcc
CFLAGS = -O3 -g -fopenmp -std=gnu99
LDFLAGS = -lm

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

# ifeq ($(strip $(IMPL)), )
#  IMPL=hj
# endif

manhattan: manhattan.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# metrics: metrics.o listrank_$(IMPL).o listutils.o timer.o
# 	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f *~ manhattan *.o
