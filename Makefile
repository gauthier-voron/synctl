DEP := .depends.d/
COV := gcov/
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

objects-gcov := $(patsubst %.o, %.gcov.o,    $(objects))
objects-gcda := $(patsubst %.o, %.gcov.gcda, $(objects))

intercept-sources := $(filter %.c, $(call FIND, test/intercept))
intercept-objects := $(patsubst %.c, $(OBJ)%.so, $(intercept-sources))

check: $(BIN)synctl
	./$< init --force 'sandbox'
	./$< push --root='include' --server 'file://sandbox' \
             --exclude=/synctl/* --include=/synctl/io/* --include=**/ui/* --skip=*repo*
	# ./$< pull -rrecvbox -s 'file://sandbox' -R `cat sandbox/branches/Orme/*/ref`

all: $(BIN)synctl


test: validation-test

validation-test: test/execute.sh $(BIN)synctl $(BIN)intercept.so
	$(call cmd-call, $< --mode validation, \
          $(if $(filter $(V), 0), --silent,    \
          $(if $(filter $(V), 1), --quiet)))

.validation-test-gcov: test/execute.sh $(BIN)synctl-gcov
	-$(call cmd-call, $< --mode validation,  \
          --executable $(BIN)synctl-gcov         \
          $(if $(filter $(V), 0 1 2), --silent))

benchmark: test/execute.sh $(BIN)synctl
	$(call cmd-call, $< --mode benchmark)

coverage: $(COV)coverage.csv

clean:
	$(call cmd-clean, $(DEP) $(COV) $(OBJ) $(BIN) .depends \
               sandbox recvbox recbox)


$(call REQUIRE-DIR, $(BIN)synctl)
$(call REQUIRE-DIR, $(BIN)synctl-gcov)

$(BIN)synctl: $(objects)
	$(call cmd-ldcxx, $@, $^, -lssl -lcrypto -lpcre2-8)

$(BIN)synctl-gcov: $(objects-gcov)
	$(call cmd-ldcov, $@, $^, -lssl -lcrypto -lpcre2-8)


$(call REQUIRE-DIR, $(objects))
$(call REQUIRE-DIR, $(objects-gcov))
$(call REQUIRE-DEP, $(sources), $(DEP)%.d)
$(call REQUIRE-DEP, $(sources), $(DEP)%.gcov.d)

$(OBJ)%.o: %.cxx
	$(call cmd-ccxx, $@, $<, -Iinclude/ -include iostream)

$(OBJ)%.gcov.o: %.cxx
	$(call cmd-ccov, $@, $<, -Iinclude/ -include iostream)

$(DEP)%.cxx.d: %.cxx
	$(call cmd-depcxx, $@, $<, $(patsubst %.cxx, $(OBJ)%.o, $<), \
               -Iinclude/)

$(DEP)%.cxx.gcov.d: %.cxx
	$(call cmd-depcxx, $@, $<, $(patsubst %.cxx, $(OBJ)%.gcov.o, $<), \
               -Iinclude/)


$(call REQUIRE-DIR, $(COV)coverage.csv)

$(COV)coverage.csv: $(objects-gcda)
	$(call cmd-call, test/tools/coverage, -o $@ -d $(COV) $(objects-gcov))

$(objects-gcda): .validation-test-gcov ;

.INTERMEDIATE: .validation-test-gcov


$(call REQUIRE-DIR, $(BIN)intercept.so)

$(BIN)intercept.so: $(intercept-objects)
	$(call cmd-ldso, $@, $^, -ldl)

$(call REQUIRE-DIR, $(intercept-objects))
$(call REQUIRE-DEP, $(intercept-sources), $(DEP)%.d)

$(OBJ)test/intercept/%.so: test/intercept/%.c
	$(call cmd-ccso, $@, $<, -Itest/intercept/)

$(DEP)test/intercept/%.c.d: test/intercept/%.c
	$(call cmd-depc, $@, $<, $(patsubst %.c, $(OBJ)%.so, $<), \
               -Itest/intercept/)


.depends:
	$(call cmd-dep, $@, $(filter %.d, $^))

ifeq ($(mode),build)
  -include .depends
endif


endif
