r44 := reading44
r45 := reading45
r46 := reading46
r47 := reading47
r48 := reading48

readings := $(r44) $(r45) $(r46) $(r47) $(r48) 


ifneq (,$(findstring clean, $(MAKECMDGOALS)))
clean := clean
endif

.PHONY: all $(readings)
all: $(readings)

$(readings):
	$(MAKE) --directory=$@ $(clean)

.CLEAN: clean 
clean: $(readings)



