ODIR=build

ifeq ($(OS), Windows_NT)
CC=g++-4
CFLAGS = -DWINDOWS
LKFLAGS = -enable-auto-import -Wl,-Map -Wl,example.map
else
CC=g++
CFLAGS = -arch i386 -DMAC_OSX
LKFLAGS = 
endif

INCDIR = -Iinclude

MEMORY_LIB_NAME := memory.a
MEMORY_LIB = $(ODIR)/$(MEMORY_LIB_NAME)

TEST_EXE_NAME := test.exe
TEST_EXE = $(ODIR)/$(TEST_EXE_NAME)

_MEMORY_OBJ = chunk.o fixedallocator.o smallobjectallocator.o
MEMORY_OBJ = $(patsubst %,$(ODIR)/%,$(_MEMORY_OBJ))

_TEST1_OBJ = memory1.o 
TEST1_OBJ = $(patsubst %,$(ODIR)/%,$(_TEST1_OBJ))

default: $(TEST_EXE)

$(ODIR)/%.o:  source/%.cpp
	$(CC) -c -o $@ $< $(CFLAGS) $(INCDIR)

$(MEMORY_LIB): $(MEMORY_OBJ)
	ar rcs $(MEMORY_LIB) $(MEMORY_OBJ)

$(ODIR)/%.o: tests/%.cpp
	$(CC) -c -o $@ $< $(CFLAGS) $(INCDIR)

$(TEST_EXE): $(MEMORY_OBJ) $(TEST1_OBJ)
	$(CC) $(LKFLAGS) -o $@ $^ $(CFLAGS)

.PHONY: clean
clean:
	rm -f $(ODIR)/*.o $(ODIR)/.exe $(ODIR)/*.stackdump $(ODIR)/*.map $(ODIR)/*.a

.PHONY: run
run:
	$(TEST_EXE)

