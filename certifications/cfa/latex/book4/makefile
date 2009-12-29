s11 := study_session11
s12 := study_session12

study_sessions := $(s11) $(s12)

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

