DEP := .depends.d/
OBJ := obj/
BIN := bin/


include Function.mk
include Command.mk


config-commands := tidy clean cleanall .depends

ifeq ($(filter $(config-commands), $(MAKECMDGOALS)),)
  # There is no goal or there are only build commands
  mode := build
else
  # There is at least one config command
  ifneq ($(filter-out $(config-commands), $(MAKECMDGOALS)),)
    # There is at least one build command
    mode := mixed
  else
    # There is no build command
    mode := config
  endif
endif


ifeq ($(mode),mixed)

  %:
        $(call cmd-make, $@)

  .NOTPARALLEL:

else


sources := $(filter %.cxx, $(call FIND,io plan repo tree ui)) $(wildcard *.cxx)
objects := $(patsubst %.cxx, $(OBJ)%.o, $(sources))


check: $(BIN)synctl
	./$< init --force 'sandbox'
	./$< push --root='include' --server 'file://sandbox' \
             --exclude=/synctl/* --include=/synctl/io/* --include=**/ui/* --skip=*repo*
	# ./$< pull -rrecvbox -s 'file://sandbox' -R `cat sandbox/branches/Orme/*/ref`

all: $(BIN)synctl

clean:
	$(call cmd-clean, $(DEP) $(OBJ) $(BIN) .depends sandbox recvbox recbox)


$(call REQUIRE-DIR, $(BIN)synctl)

$(BIN)synctl: $(objects)
	$(call cmd-ldcxx, $@, $^, -lssl -lcrypto)


$(call REQUIRE-DIR, $(objects))
$(call REQUIRE-DEP, $(sources))

$(OBJ)%.o: %.cxx
	$(call cmd-ccxx, $@, $<, -Iinclude/ -include iostream)

$(DEP)%.cxx.d: %.cxx
	$(call cmd-depcxx, $@, $<, $(patsubst %.cxx, $(OBJ)%.o, $<), \
               -Iinclude/)


.depends:
	$(call cmd-dep, $@, $(filter %.d, $^))

ifeq ($(mode),build)
  -include .depends
endif

endif
