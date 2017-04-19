SRCDIR   := src
BINDIR   := bin
LIBDIR   := lib
CXXFLAGS := -g -O2 -Wall -Wextra -pipe -std=c++14 -pedantic -I$(SRCDIR)
LDFLAGS  :=
AR       := ar crs
MKDIR    := mkdir -p
RM       := rm -f
UNAME    := $(shell uname -s)

ifeq ($(UNAME), Darwin)
	CXXFLAGS += -stdlib=libc++
endif

EXE    := $(BINDIR)/eeZGmumu
EXESRC := $(patsubst $(BINDIR)/%,$(SRCDIR)/%.cc,$(EXE))
EXEOBJ := $(EXESRC:.cc=.o)
LIB    := $(LIBDIR)/libPartonMC.a
LIBSRC := $(filter-out $(EXESRC),$(wildcard $(SRCDIR)/*.cc))
LIBOBJ := $(LIBSRC:.cc=.o)
HEAD   := $(filter-out $(EXESRC:.cc=.h),$(wildcard $(SRCDIR)/*.h))

.PHONY: all build clean

all: $(BINDIR)/eeZGmumu

$(BINDIR)/eeZGmumu: $(SRCDIR)/eeZGmumu.o build $(LIB)
	$(CXX) $(LDFLAGS) -o $@ $< $(LIB)

$(LIBDIR)/libPartonMC.a: CXXFLAGS += -fPIC
$(LIBDIR)/libPartonMC.a: $(LIBOBJ)
	$(AR) $@ $^
	ranlib $@

build:
	$(MKDIR) $(LIBDIR)
	$(MKDIR) $(BINDIR)

clean::
	$(RM) $(LIBOBJ) $(EXEOBJ)
	$(RM) $(EXE) $(LIB)
	$(RM) -r $(BINDIR) $(LIBDIR)
