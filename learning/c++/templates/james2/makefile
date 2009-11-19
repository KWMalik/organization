CC=g++-4
ODIR=obj

CFLAGS =  -g
LKFLAGS = -g -enable-auto-import -Wl,-Map -Wl,example.map

LIBS=-lm

DEPS = 

_OBJ = main.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

example.exe: $(OBJ)
	$(CC) $(LKFLAGS) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o example.exe *.stackdump *.map

