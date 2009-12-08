r40 := reading40
r41 := reading41
r42 := reading42
r43 := reading43

readings := $(r40) #$(r41) $(r42) $(r43)


ifneq (,$(findstring clean, $(MAKECMDGOALS)))
clean := clean
endif

.PHONY: all $(readings)
all: $(readings)

$(readings):
	$(MAKE) --directory=$@ $(clean)

.CLEAN: clean 
clean: $(readings)



