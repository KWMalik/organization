ODIR=build

ifeq ($(OS), Windows_NT)
CC=g++-4
CFLAGS = -DWINDOWS
LKFLAGS = -enable-auto-import -Wl,-Map -Wl,build/test.map
else
CC=g++
CFLAGS = -arch i386 -DMAC_OSX
LKFLAGS = 
endif

INCDIR = -Iinclude

TEST_EXE_NAME := test.exe
TEST_EXE = $(ODIR)/$(TEST_EXE_NAME)

_TEST1_OBJ = main.o 
TEST1_OBJ = $(patsubst %,$(ODIR)/%,$(_TEST1_OBJ))

default: $(TEST_EXE)

$(ODIR)/%.o:  source/%.cpp
	$(CC) -c -o $@ $< $(CFLAGS) $(INCDIR)

$(TEST_EXE): $(TEST1_OBJ)
	$(CC) $(LKFLAGS) -o $@ $^ $(CFLAGS)

.PHONY: clean
clean:
	rm -f $(ODIR)/*.o $(ODIR)/*.exe $(ODIR)/*.stackdump $(ODIR)/*.map $(ODIR)/*.a

.PHONY: run
run:
	$(TEST_EXE)

