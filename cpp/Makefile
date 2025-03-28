HEADERS = json.h buffer.h
OBJECTS = main.o json.o buffer.o
CFLAGS = -I ../java/NewPipeExtractor/appwrapper/build/native/nativeCompile/ -g
LIBS = -L ../java/NewPipeExtractor/appwrapper/build/native/nativeCompile/ -l:appwrapper.so -Wl,-rpath ../java/NewPipeExtractor/appwrapper/build/native/nativeCompile/ -lreadline

default: main

%.o: %.c $(HEADERS)
	gcc $(CFLAGS) -c $< -o $@

main: $(OBJECTS)
	gcc $(OBJECTS) $(LIBS) -o $@

clean:
	-rm -f $(OBJECTS)
	-rm -f main
