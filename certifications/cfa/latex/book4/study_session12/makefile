r49 := reading49
r50 := reading50

readings := $(r49) $(r50) 


ifneq (,$(findstring clean, $(MAKECMDGOALS)))
clean := clean
endif

.PHONY: all $(readings)
all: $(readings)

$(readings):
	$(MAKE) --directory=$@ $(clean)

.CLEAN: clean 
clean: $(readings)



