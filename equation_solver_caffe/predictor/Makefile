all: predictor

predictor: predictor.cpp
	g++ --std=c++11 -L/usr/local/lib -I/usr/local/include $< -o $@ -lopencv_core -lopencv_dnn -lopencv_imgproc -lopencv_highgui -lopencv_imgcodecs -lopencv_ml

clean:
	rm ./predictor
