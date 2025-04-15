HEADERS = json.h buffer.h
OBJECTS = main.o json.o buffer.o
CFLAGS = -I include -I /usr/include/x86_64-linux-gnu -g
LIBS = -L lib -l:appwrapper.so -lreadline -lcurl

%.o: %.c $(HEADERS)
	gcc $(CFLAGS) -c $< -o $@

main: $(OBJECTS)
	gcc $(OBJECTS) $(LIBS) -o $@

clean:
	-rm -f $(OBJECTS)
	-rm -f main
