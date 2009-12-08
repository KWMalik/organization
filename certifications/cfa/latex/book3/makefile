s7 := study_session7
#s8 := study_session8
#s9 := study_session9
#s10 := study_session10

study_sessions := $(s7) # $(s8) $(s9) $(s10)

ifneq (,$(findstring clean, $(MAKECMDGOALS)))
clean := clean
$(warning "There")
endif

.PHONY: all $(study_sessions)
all: $(study_sessions)

$(study_sessions):
	$(MAKE) --directory=$@ $(clean)

.CLEAN: clean 
clean: $(study_sessions)

