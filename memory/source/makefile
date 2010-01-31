ODIR=build

ifeq ($(OS), Windows_NT)
EXE1=test.exe
EXE2=fixed.exe
CC=g++-4
CFLAGS = -DWINDOWS
LKFLAGS = -enable-auto-import -Wl,-Map -Wl,build/example.map
else
EXE1=test
EXE2=fixed
CC=g++
#CFLAGS = -arch x86_64 -DMAC_OSX -g  -x c -fmessage-length=0 -pipe -Wno-trigraphs -fpascal-strings -fasm-blocks -O0 -Wreturn-type -Wunused-variable -isysroot /Developer/SDKs/MacOSX10.6.sdk -mfix-and-continue -mmacosx-version-min=10.6 -gdwarf-2 -iquote 
#CFLAGS = -arch x86_64 -DMAC_OSX -g  -x c -fmessage-length=0 -pipe -Wno-trigraphs -fpascal-strings -fasm-blocks -O0 -Wreturn-type -Wunused-variable -mfix-and-continue -gdwarf-2 
CFLAGS = -arch x86_64 -DMAC_OSX -g  -O0 -mfix-and-continue -gdwarf-2 
LKFLAGS = -arch x86_64 -isysroot /Developer/SDKs/MacOSX10.6.sdk -mmacosx-version-min=10.6 
endif

INCDIR = -Iinclude

MEMORY_LIB_NAME := memory.a
MEMORY_LIB = $(ODIR)/$(MEMORY_LIB_NAME)

TEST_EXE_NAME := $(EXE1)
TEST_EXE = $(ODIR)/$(TEST_EXE_NAME)

FIXED_EXE_NAME := $(EXE2)
FIXED_EXE = $(ODIR)/$(FIXED_EXE_NAME)


#_MEMORY_OBJ = chunk.o growingfixedallocator.o smallobjectallocator.o
#_MEMORY_OBJ = chunk.o stagnantfixedallocator.o smallobjectallocator.o
_MEMORY_OBJ = stagnantfixedallocator.o smallobjectallocator.o
MEMORY_OBJ = $(patsubst %,$(ODIR)/%,$(_MEMORY_OBJ))

_TEST1_OBJ = memory1.o 
TEST1_OBJ = $(patsubst %,$(ODIR)/%,$(_TEST1_OBJ))

_FIXED1_HELPER_OBJS =	test_chunk.o \
						test_fixed_allocator.o \
						test_fixed_sized_chunk_allocator.o \
					   	test_new_delete_malloc_free.o \
					   	test_object_allocator.o \
					   	test_small_object_allocator.o \
					   	test_type_allocator.o \
						test_growing_pool.o

_FIXED1_OBJ = fixed1.o $(_FIXED1_HELPER_OBJS)
FIXED1_OBJ = $(patsubst %,$(ODIR)/%,$(_FIXED1_OBJ))

default: $(FIXED_EXE)

$(ODIR)/%.o:  source/%.cpp
	$(CC) $(CFLAGS) $(INCDIR) -c -o $@ $< 

$(MEMORY_LIB): $(MEMORY_OBJ)
	ar rcs $(MEMORY_LIB) $(MEMORY_OBJ)

$(ODIR)/%.o: tests/%.cpp
	$(CC) -c -o $@ $< $(CFLAGS) $(INCDIR)

$(TEST_EXE): $(MEMORY_LIB) $(TEST1_OBJ)
	$(CC) $(LKFLAGS) -o $@ $^ $(CFLAGS)

$(FIXED_EXE): $(MEMORY_LIB) $(FIXED1_OBJ)
	$(CC) $(LKFLAGS) -o $@ $^ $(CFLAGS)


.PHONY: clean
clean:
	rm -f $(ODIR)/*.o $(ODIR)/*.exe $(ODIR)/*.stackdump $(ODIR)/*.map $(ODIR)/*.a

.PHONY: run
run:
	$(FIXED_EXE)

.PHONY: debug
debug:
	gdb $(FIXED_EXE)

.PHONY: test
test:
	$(TEST_EXE)

.PHONY: debug-test
debug-test:
	gdb $(TEST_EXE)

.PHONY: fixed 
fixed:
	$(FIXED_EXE)

.PHONY: fixed-test
fixed-test:
	gdb $(FIXED_EXE)

