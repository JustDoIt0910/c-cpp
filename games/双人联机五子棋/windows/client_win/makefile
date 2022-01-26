
CC = g++
OBJS = main.o Window.o GoBang.o Network.o Button.o Label.o Widget.o
TARGET = gobang_c      		
LIBS += -lSDL2 			\
		-lSDL2main 		\
		-lSDL2_image 	\
		-lSDL2_ttf 		\
		-lpthread		

$(TARGET): $(OBJS)
	$(CC) -o gobang_c $(OBJS) $(LIBS)

main.o: main.cpp
GoBang.o: GoBang.cpp GoBang.h
Window.o: Window.cpp Window.h Widget.h
Network.o: Network.cpp Network.h
Button.o: Button.cpp Button.h
Label.o: Label.cpp Label.h
Widget.o: Widget.h Widget.cpp

.PHONY: clean
clean:
	rm *.o $(TARGET)
