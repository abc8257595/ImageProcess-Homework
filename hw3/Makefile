CC = gcc
SRC = $(wildcard *.cpp)
TAR = $(patsubst %.cpp,%.exe,$(SRC))

all:$(TAR)
	echo "compiling $(SRC)"
$(TAR):%.exe:%.cpp
	-gcc -g `pkg-config opencv --cflags --libs opencv` -o $@ $^ -I/usr/local/include/opencv2 -L/usr/local/lib -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_gpu -lopencv_ts -lopencv_video -lopencv_objdetect -lopencv_ml -lpthread

clean:
	rm $(TAR)

.PHONY:all clean
