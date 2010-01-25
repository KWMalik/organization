CC=g++-4
CC_INCDIRS := -I"C:\Qt\2009.05\Qt\include"
CFLAGS = 

LKFLAGS = -enable-auto-import -Wl,-Map -Wl,example.map
LIBS=-lm  "C:\Qt\2009.05\Qt\lib\libQtCore4.a" "C:\Qt\2009.05\Qt\lib\libQtXml4.a" "C:\Qt\2009.05\Qt\lib\libQtXmlPatterns4.a" "C:\Qt\2009.05\Qt\lib\libqtmain.a" "C:\Qt\2009.05\Qt\lib\libQAxContainer.a" "C:\Qt\2009.05\Qt\lib\libQtXmlPatterns4.a" 

ODIR=obj

DEPS = 

_OBJ = main.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.cpp $(DEPS)
	$(CC) $(CC_INCDIRS) -c -o $@ $< $(CFLAGS)

example.exe: $(OBJ)
	$(CC) $(LKFLAGS) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o example.exe *.stackdump *.map

