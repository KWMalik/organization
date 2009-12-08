r32 := reading32
r33 := reading33
r34 := reading34
r35 := reading35

readings := $(r32) $(r33) $(r34) $(r35)


ifneq (,$(findstring clean, $(MAKECMDGOALS)))
clean := clean
endif

.PHONY: all $(readings)
all: $(readings)

$(readings):
	$(MAKE) --directory=$@ $(clean)

.CLEAN: clean 
clean: $(readings)



