LIBS 	= 
CFLAGS	= -Wall -I inc
LDFLAGS	= -rcs
CC	= gcc   # compiler name
LD	= ar

TARGET	= libuart.a

OBJ := $(patsubst %.c,%.o,$(wildcard src/*.c))

all: $(TARGET) 


$(TARGET): $(OBJ)
	$(LD) $(LDFLAGS) -o $@ $(OBJ) $(LIBS)
	
src/%.o : src/%.c inc/%.h
	$(CC) -c $< -o $@ $(CFLAGS)
clean:
	rm -f $(TARGET)  $(OBJ) #This way is cleaner than your clean

install:
	cp libuart.a /lib/
	cp inc/uart.h /usr/include/

uninstall:
	rm /lib/libuart.a
	cp /usr/include/uart.h
