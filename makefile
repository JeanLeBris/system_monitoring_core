.PHONY: compile examples clean

# compile:
# 	g++ .\\src\\*.cpp .\\src\\windows\\*.cpp -I include -o test.exe

compile:
	g++ ./src/*.cpp ./src/linux/*.cpp -I include -o test.exe