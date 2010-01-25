#User defined variables
core_test_memory1_user_dir			:= $(core_dir)tests/memory
core_test_memory1_user_src			:= memory1.cpp
core_test_memory1_user_frm			:= 
#core_test_memory1_user_lib			:= memory1
core_test_memory1_user_prg			:= memory1
core_test_memory1_user_dp_lb		:= $(LIB_DIR)libcore.a
core_test_memory1_user_cc_incdir	= -I$(core_dir)include

core_test_memory1_user_lk_flags := -Fpackages/colladadom/1.04.00/build/mac-1.4
core_test_memory1_user_lk_dirs := -L$(LIB_DIR) $(core_test_memory1_user_dp_lb)


# Generate the names of local variables
$(eval $(call create_locals,core_test_memory1,$(core_test_memory1_user_dir),$(core_test_memory1_user_src),$(core_test_memory1_user_frm),$(core_test_memory1_user_prg)))

# HACK HACK HACK HACK -- THIS GETS AROUND A BUG WITH NESTING VARIABLES TO DEEP IN MAKE
$(eval $(call create_locals2,core_test_memory1,$(core_test_memory1_local_src)))

# These are useful for debugging so I'm going to leave them in.
#$(warning $(core_test_memory1_local_dir))
#$(warning $(core_test_memory1_local_src))
#$(warning $(core_test_memory1_local_objs))
#$(warning $(core_test_memory1_local_frm))
#$(warning $(core_test_memory1_local_lib))
#$(warning $(core_test_memory1_local_prg))
#$(warning $(core_test_memory1_local_filenames))

# Include necessary dependancies
dependy = $(subst .o,.d,$(core_test_memory1_local_objs))
ifneq "$(MAKECMDGOALS)" "clean"
-include $(dependy)
endif

# Providing a shortcut to build the core lib.
.PHONY: core-test-memory1
core-test-memory1: $(core_test_memory1_local_prg)

# Providing a shortcut to build the core lib.
.PHONY: core-test-memory1-run
core-test-memory1-run:
	$(core_test_memory1_local_prg)

# Providing a shortcut to build the core lib.
.PHONY: core-test-memory1-debug
core-test-memory1-debug:
	gdb $(core_test_memory1_local_prg)


# Create all of the dependancies
$(foreach source,$(core_test_memory1_local_filenames),$(eval $(call compile-depend,$(source), CC_INCDIR = $(core_test_memory1_user_cc_incdir))))

# Compile all of the source files
$(foreach source,$(core_test_memory1_local_filenames),$(eval $(call compile-c,$(source),CC_INCDIR = $(core_test_memory1_user_cc_incdir))))

# Convert the memory1 program
$(eval $(call make-program,$(core_test_memory1_local_prg),$(core_test_memory1_local_objs),$(core_test_memory1_local_dp_lb),$(core_test_memory1_local_link_libs),$(core_test_memory1_local_frm), LK_FLAGS += $(core_test_memory1_user_lk_flags), LK_INCDIRS += $(core_test_memory1_user_lk_dirs)))

