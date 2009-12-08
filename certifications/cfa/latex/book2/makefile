s4 := study_session4
s5 := study_session5
#s6 := study_session6

study_sessions := $(s4) $(s5) # $(s6)

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



