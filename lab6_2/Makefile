all: tracer

tracer:
	g++ -g -std=c++17 -fopenmp main.cpp Vec.cpp Object.cpp FileManager.cpp RayTracer.cpp -o $@

test:
	./tracer test.rayTracing test.ppm &
	./tracer SceneII.rayTracing SceneII.ppm &
	./tracer diffuse.rayTracing diffuse.ppm &
	./tracer test2.rayTracing test2.ppm &

handin:
	tar -czf raytracer.tar *.cpp *.h diffuse.ppm test.ppm SceneII.ppm

clean:
	rm -f tracer