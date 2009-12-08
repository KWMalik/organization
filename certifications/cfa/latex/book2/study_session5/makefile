r18 := reading18
r19 := reading19
r20 := reading20
#r21 := reading21
#r22 := reading22

readings := $(r18) $(r19) $(r20) $(r21) $(r22)

ifneq (,$(findstring clean, $(MAKECMDGOALS)))
clean := clean
$(warning "Here 2")
endif

.PHONY: all $(readings)
all: $(readings)

$(readings):
	$(MAKE) --directory=$@ $(clean)

.CLEAN: clean 
clean: $(readings)



