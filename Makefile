CFLAGS= -I/usr/include/GL/freeglut/2.6.0/gcc-4.4.6/include/GL -I. -lc -g -Wall -lm \
 -L/usr/include/GL/freeglut/2.6.0/gcc-4.4.7/lib -lglut -lGLU -lGL

CCFLAGS= -L/usr/lib64/nvidia/cuda -lcudart -lcublas -lcufft -lcurand -lcusparse -lnpp \
 -I/usr/include/linux/cuda/include -I. -g -lm -lc
  

SimulationMain:  Source.o
	g++ $(CFLAGS) $(CCFLAGS)  Source.o -o $@

Source.o: Source.cu
	nvcc $(CCFLAGS) Source.cu -c $@



