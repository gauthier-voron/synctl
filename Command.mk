define cmd-make
  $(call cmd-info,  MAKE    $(strip $(1)))
  +$(Q)$(MAKE) $(1)
endef

define cmd-mkdir
  $(call cmd-info,  MKDIR   $(strip $(call cmd-format, $(1))))
  $(Q)mkdir $(1)
endef


define cmd-ccxx
  $(call cmd-echo,  CCXX    $(strip $(call cmd-format, $(1))))
  $(Q)g++ -Wall -Wextra -g -std=c++17 -c $(2) -o $(1) $(3)
endef

define cmd-ldcxx
  $(call cmd-echo,  LD      $(strip $(call cmd-format, $(1))))
  $(Q)g++ $(2) -o $(1) $(3)
endef


define cmd-depcxx
  $(call cmd-info,  DEPCXX  $(strip $(call cmd-format, $(1))))
  $(Q)g++ -std=c++17 $(2) -MM -o $(1) -MT $(3) $(4)
endef

define cmd-dep
  $(call cmd-info,  DEP     $(strip $(call cmd-format, $(1))))
  $(Q)cat $(2) > $(1)
endef


define cmd-clean
  $(call cmd-echo,  CLEAN)
  $(Q)rm -rf $(1) 2> /dev/null || true
endef
