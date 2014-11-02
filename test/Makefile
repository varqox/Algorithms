export

# Extra options to compile project with
EXTRA_OPTIONS = -O3

# Extra options to link project with
EXTRA_LD_OPTIONS = -s -O3

# Warnings options to compile project with
WARNING_OPTIONS = -Wall -Wextra -Wabi -Weffc++ -Wshadow -Wfloat-equal -Wno-unused-result

CFLAGS = $(WARNING_OPTIONS) $(EXTRA_OPTIONS) -c
CXXFLAGS = $(CFLAGS)
LFLAGS = $(WARNING_OPTIONS) $(EXTRA_LD_OPTIONS)

ifeq ($(VERBOSE),1)
	Q :=
	P =
else
	Q := @
	P = printf "   $$(1)\t $$(subst $(ROOT),,$$(abspath $$(2)))\n";
	override MFLAGS += --no-print-directory -s
endif

PHONY := all
all:
	@printf "CC -> $(CC)\nCXX -> $(CXX)\n"
	$(Q)$(MAKE) $(MFLAGS) -f Makefile2
	@printf "\033[;32mBuild finished\033[0m\n"

PHONY += release
release: override RELEASE=1
release: override EXTRA_LD_OPTIONS += -static
release: clean
	@printf "CC -> $(CC)\nCXX -> $(CXX)\n"
	$(Q)$(MAKE) $(MFLAGS) -f Makefile2
	@printf "\033[;32mBuild finished\033[0m\n"


PHONY += debug
debug: override CC += -DDEBUG
debug: override CXX += -DDEBUG
debug: override CFLAGS = $(WARNING_OPTIONS) -g -c
debug: override CXXFLAGS = $(WARNING_OPTIONS) -g -c
debug: override LFLAGS = $(WARNING_OPTIONS) -g
debug:
	@printf "CC -> $(CC)\nCXX -> $(CXX)\n"
	$(Q)$(MAKE) $(MFLAGS) -f Makefile2
	@printf "\033[;32mBuild finished\033[0m\n"

PHONY += clean
clean:
	@$(MAKE) $(MFLAGS) clean -f Makefile2

PHONY += mrproper
mrproper:
	@$(MAKE) $(MFLAGS) mrproper -f Makefile2

.PHONY: $(PHONY)
