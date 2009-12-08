r13 := reading13
r14 := reading14
r15 := reading15
r16 := reading16
r17 := reading17

readings := $(r13) $(r14) $(r15) $(r16) $(r17)

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



