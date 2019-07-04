UTHREAD = .
TARGETS = pc_sem traffic_sem SearchFileContents.class

JUNKF = *.o *~ *.dSYM
WARNINGS = -Wall -Wno-unused-variable -Wno-unused-const-variable

override CFLAGS  += $(WARNINGS) -g -std=gnu11 -I$(UTHREAD)
override LDFLAGS += -pthread

JC = javac
override JFLAGS += -g

all: $(TARGETS)

pc_sem:      pc_sem.o      uthread.o uthread_sem.o
traffic_sem: traffic_sem.o uthread.o uthread_sem.o

pc_sem.o:      pc_sem.c      uthread.h spinlock.h uthread_sem.h
traffic_sem.o: traffic_sem.c uthread.h spinlock.h uthread_sem.h
uthread.o:     uthread.c     uthread.h uthread_util.h spinlock.h
uthread_sem.o: uthread_sem.c uthread_sem.h uthread.h uthread_util.h spinlock.h

SearchFileContents.class: SearchFileContents.java
	$(JC) $(JFLAGS) $^

tidy:
	-rm -rf $(JUNKF)
clean: tidy
	-rm -rf $(TARGETS)


