run:
	g++ *.cpp *.c NBodySimulation/*.cpp NBodySimulation/*.cc -I./NBodySimulation -Wfatal-errors -lglfw -ldl && ./a.out  &&  python3 file.py
debug:
	g++ *.cpp *.c NBodySimulation/*.cpp NBodySimulation/*.cc -I./NBodySimulation -Wfatal-errors -lglfw -ldl -g && gdb ./a.out
