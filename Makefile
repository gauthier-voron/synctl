# Copyright 2019 Gauthier Voron
#
# This file is part of Synctl
#
# Synctl is free software: you can redistribute it and/or modify it under the
# terms of the GNU General Public License as published by the Free Software
# Fundation, either version 3 of the License, or (at your option) any later
# version.
#
# Synctl is distributed in the hope that it will be useful, but WITHOUT ANY
# WARRANTY; without even the implied warranty of MERCHANDABILITY or FITNESS FOR
# A PARTICULAR PURPOSE. See the GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along with
# Synctl. If not, see <https://www.gnu.org/licenses/>.
#


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

ifneq ($(C),)
  cases := $(wildcard test/validation/$(strip $(C))*)
endif

manpages := $(foreach suffix, 1 2 3 4 5 6 7 8, \
              $(filter %.$(suffix), $(call FIND, man)))


bin-install-path := $(PREFIX)/usr/bin/

manpages-install-path := $(PREFIX)/usr/share/


all: $(BIN)synctl


test: validation-test

full-test: validation-test memcheck coverage


validation-test: test/execute.sh $(BIN)synctl $(BIN)intercept.so
	$(call cmd-call, $< --mode validation, \
          $(if $(filter $(V), 0), --silent,    \
          $(if $(filter $(V), 1), --quiet))    \
          --executable $(BIN)synctl $(cases))

memcheck: test/execute.sh $(BIN)synctl $(BIN)intercept.so
	$(call cmd-call, $< --mode validation, \
          $(if $(filter $(V), 0), --silent,    \
          $(if $(filter $(V), 1), --quiet))    \
          --valgrind --executable $(BIN)synctl $(cases))

.validation-test-gcov: test/execute.sh $(BIN)synctl-gcov
	-$(call cmd-call, $< --mode validation,    \
          $(if $(filter $(V), 0 1 2), --silent)    \
          --executable $(BIN)synctl-gcov $(cases))


benchmark: test/execute.sh $(BIN)synctl
	$(call cmd-call, $< --mode benchmark, \
          --executable $(BIN)synctl $(cases))


coverage: $(COV)coverage.csv


install: installbin installdoc

installbin: $(bin-install-path)synctl

installdoc: $(patsubst %, $(manpages-install-path)%.gz, $(manpages))


clean:
	$(call cmd-clean, $(DEP) $(COV) $(OBJ) $(BIN) .depends)


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


$(call REQUIRE-DIR, $(bin-install-path)synctl)
$(call REQUIRE-DIR, $(patsubst %, $(manpages-install-path)%.gz, $(manpages)))

$(bin-install-path)synctl: $(BIN)synctl
	$(call cmd-cp, $@, $<)

$(manpages-install-path)%.gz: %
	$(call cmd-gzip, $@, $<)


-include .config/Makefile


.depends:
	$(call cmd-dep, $@, $(filter %.d, $^))

ifeq ($(mode),build)
  -include .depends
endif


endif
