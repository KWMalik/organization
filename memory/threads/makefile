ODIR=build

ifeq ($(OS), Windows_NT)
EXE=main.exe
CC=g++-4
CFLAGS = -DWINDOWS
LKFLAGS = -enable-auto-import -Wl,-Map -Wl,build/main.map
else
EXE=main
CC=g++
CFLAGS = -arch x86_64 -DMAC_OSX -g  -O0 -mfix-and-continue -gdwarf-2 
LKFLAGS = -arch x86_64 -isysroot /Developer/SDKs/MacOSX10.6.sdk -mmacosx-version-min=10.6 
endif

INCDIR = -Iinclude


FIXED_EXE_NAME := $(EXE)
FIXED_EXE = $(ODIR)/$(FIXED_EXE_NAME)

_FIXED1_HELPER_OBJS =	#test_new_delete_malloc_free.o

_FIXED1_OBJ = main.o $(_FIXED1_HELPER_OBJS)
FIXED1_OBJ = $(patsubst %,$(ODIR)/%,$(_FIXED1_OBJ))


default: $(FIXED_EXE)

$(ODIR)/%.o: tests/%.cpp include/*.h
	$(CC) -c -o $@ $< $(CFLAGS) $(INCDIR)

$(FIXED_EXE): $(FIXED1_OBJ)
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

.PHONY: rar
rar:
	rm -f $(ODIR)/*.o $(ODIR)/*.exe $(ODIR)/*.stackdump $(ODIR)/*.map $(ODIR)/*.a
	rm -f *.rar
	rm -f projects/smallobjectallocator_vcproj/*.ncb projects/smallobjectallocator_vcproj/*.suo 
	rm -fdr projects/smallobjectallocator_vcproj/debug 
	rm -fdr projects/smallobjectallocator_vcproj/smallobjectallocator_vcproj/Debug
	rm -fdr projects/smallobjectallocator_vcproj/smallobjectallocator_vcproj/*.vcproj.*user

	rar a -r object_allocator



