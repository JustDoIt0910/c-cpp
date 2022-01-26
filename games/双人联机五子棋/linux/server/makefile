
CC = g++
OBJS = main.o server.o
TARGET = gobang_s     		
		

$(TARGET): $(OBJS)
	$(CC) -o gobang_s $(OBJS)

main.o: main.cpp
server.o: server.cpp server.h

.PHONY: clean
clean:
	rm *.o $(TARGET)