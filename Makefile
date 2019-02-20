DEP := .depends.d/
OBJ := obj/
BIN := bin/

VALIDATION-GCOV := y


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
objects-gcov := $(patsubst %.o, %.gcov.o, $(objects))


check: $(BIN)synctl
	./$< init --force 'sandbox'
	./$< push --root='include' --server 'file://sandbox' \
             --exclude=/synctl/* --include=/synctl/io/* --include=**/ui/* --skip=*repo*
	# ./$< pull -rrecvbox -s 'file://sandbox' -R `cat sandbox/branches/Orme/*/ref`

all: $(BIN)synctl

test: validation-test


ifeq ($(VALIDATION-GCOV),y)

  validation-test: test/validation.sh $(BIN)synctl-gcov
	$(call cmd-call, $<, \
          --executable $(BIN)synctl-gcov \
          $(if $(filter $(V), 0), --silent, $(if $(filter $(V), 1), --quiet)))

else

  validation-test: test/validation.sh $(BIN)synctl
	$(call cmd-call, $<, \
          $(if $(filter $(V), 0), -s, $(if $(filter $(V), 1), -q)))

endif


clean:
	$(call cmd-clean, $(DEP) $(OBJ) $(BIN) .depends sandbox recvbox recbox)


$(call REQUIRE-DIR, $(BIN)synctl)
$(call REQUIRE-DIR, $(BIN)synctl-gcov)

$(BIN)synctl: $(objects)
	$(call cmd-ldcxx, $@, $^, -lssl -lcrypto)

$(BIN)synctl-gcov: $(objects-gcov)
	$(call cmd-ldcov, $@, $^, -lssl -lcrypto)


$(call REQUIRE-DIR, $(objects))
$(call REQUIRE-DIR, $(objects-gcov))
$(call REQUIRE-DEP, $(sources))

$(OBJ)%.o: %.cxx
	$(call cmd-ccxx, $@, $<, -Iinclude/ -include iostream)

$(OBJ)%.gcov.o: %.cxx
	$(call cmd-ccov, $@, $<, -Iinclude/ -include iostream)

$(DEP)%.cxx.d: %.cxx
	$(call cmd-depcxx, $@, $<, $(patsubst %.cxx, $(OBJ)%.o, $<), \
               -Iinclude/)

$(DEP)%.cxx.d: %.cxx
	$(call cmd-depcxx, $@, $<, $(patsubst %.cxx, $(OBJ)%.gcov.o, $<), \
               -Iinclude/)


.depends:
	$(call cmd-dep, $@, $(filter %.d, $^))

ifeq ($(mode),build)
  -include .depends
endif

endif
