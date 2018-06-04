CC 		:= gcc
LD		:= ld


INCDIR	:= $(shell pwd)
# C预处理器的flag，flag就是编译器可选的选项
#CPPFLAGS	:= -nostdlib -nostdinc -I$(INCDIR)/include
CPPFLAGS	:= -I$(INCDIR)/include
# C编译器的flag
CFLAGS		:= -Wall -O2 -fno-builtin

LIBS		:= -ljpeg


export CC LD CPPFLAGS CFLAGS LDFLAGS LIBS


objs := main.o image2jpeg.o

all:$(objs)
	$(CC) -o app.elf $^ $(LIBS)

%.o : %.c
	$(CC) $(CPPFLAGS) -o $@ $< -c


clean:
	rm *.o
	rm *.elf






