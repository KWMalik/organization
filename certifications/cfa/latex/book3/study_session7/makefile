r29 := reading29
r30 := reading30
#r31 := reading31

readings := $(r29) $(r30) #$(r31)

ifneq (,$(findstring clean, $(MAKECMDGOALS)))
clean := clean
$(warning "Here")
endif

.PHONY: all $(readings)
all: $(readings)

$(readings):
	$(MAKE) --directory=$@ $(clean)

.CLEAN: clean 
clean: $(readings)



