r36 := reading36
r37 := reading37
r38 := reading38
r39 := reading39

readings := $(r36) $(r37) $(r38) $(r39)


ifneq (,$(findstring clean, $(MAKECMDGOALS)))
clean := clean
endif

.PHONY: all $(readings)
all: $(readings)

$(readings):
	$(MAKE) --directory=$@ $(clean)

.CLEAN: clean 
clean: $(readings)



