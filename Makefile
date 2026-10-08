#===============================================================================
# Configuration
#===============================================================================

CPPFLAGS := -I. -I$(shell root-config --incdir)
CXXFLAGS := -O2 -g -fPIC $(shell root-config --auxcflags)
ROOT_LIBS := $(shell root-config --libs)

CXX ?= c++

# The compiler writes the headers each object and executable includes into a
# .d file next to it, read back in at the end of this file, so changing any
# header rebuilds what uses it.
DEPFLAGS := -MMD -MP
#===============================================================================
# Default
#===============================================================================

all: evd_run


#===============================================================================
# VSD DICTIONARY
#===============================================================================

# --cxxmodule builds VsdDict.pcm, a C++ module described in module.modulemap.
# ROOT loads it with the library, so cling never parses VsdBase.h at runtime.
VsdDict.cc: VsdBase.h Vsd_Linkdef.h module.modulemap
	@rm -f VsdDict.cc VsdDict.pcm libVsdDict_rdict.pcm
	rootcling -I. -f VsdDict.cc -s libVsdDict.so \
	    --cxxmodule --moduleMapFile=module.modulemap \
	    VsdBase.h \
	    Vsd_Linkdef.h

VsdDict.o: VsdDict.cc
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(DEPFLAGS) -c $< -o $@

libVsdDict.so: VsdDict.o
	$(CXX) -shared -o $@ \
	    VsdDict.o \
	    $(ROOT_LIBS)


#===============================================================================
# GRAPHICAL DICTIONARY
#===============================================================================

# Needs VsdDict.pcm: VsdProxies.h includes VsdBase.h, which is imported
# from the VsdDict module instead of being compiled into this one.
# rootcling does not write dependency files: list every local header the
# dictionary headers include, directly or not.
FWDict.cc: FWEventManager.h FWDataCollection.h VsdProxies.h VsdBase.h lego_bins.h \
           FW_Linkdef.h module.modulemap VsdDict.cc
	@rm -f FWDict.cc FWDict.pcm libFWDict_rdict.pcm
	rootcling -I. -f FWDict.cc -s libFWDict.so \
	    --cxxmodule --moduleMapFile=module.modulemap \
	    FWEventManager.h \
	    FWDataCollection.h \
	    VsdProxies.h \
	    FW_Linkdef.h

FWDict.o: FWDict.cc
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(DEPFLAGS) -c $< -o $@

FWEventManager.o: FWEventManager.cc
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(DEPFLAGS) -c $< -o $@

VsdProxies.o: VsdProxies.cc
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(DEPFLAGS) -c $< -o $@

libFWDict.so: FWDict.o FWEventManager.o VsdProxies.o
	$(CXX) -shared -o $@ \
	    FWDict.o \
	    FWEventManager.o \
	    VsdProxies.o \
	    $(ROOT_LIBS)


#===============================================================================
# SAMPLE
#===============================================================================

UserVsd.root: UserVsd.py
	python UserVsd.py


#===============================================================================
# EXECUTABLE
#===============================================================================

evd_run: evd_run.cc libVsdDict.so libFWDict.so
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(DEPFLAGS) -o $@ \
	    evd_run.cc \
	    -L. \
	    -Wl,-rpath,'$$ORIGIN' \
	    -Wl,--no-as-needed \
	    -lVsdDict \
	    -lFWDict \
	    -lEG \
	    -lGeom \
	    -lROOTWebDisplay \
	    -lROOTEve \
	    $(ROOT_LIBS)

serviceVSDNano: service.cc libVsdDict.so libFWDict.so
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(DEPFLAGS) -o $@ \
	    service.cc \
	    -L. \
	    -Wl,-rpath,'$$ORIGIN' \
	    -Wl,--no-as-needed \
	    -lVsdDict \
	    -lFWDict \
	    -lEG \
	    -lGeom \
	    -lROOTWebDisplay \
	    -lROOTEve \
	    $(ROOT_LIBS)


#===============================================================================
# CLEAN
#===============================================================================

clean:
	rm -f evd_run
	rm -f libVsdDict.so libFWDict.so
	rm -f VsdDict.cc VsdDict.o VsdDict.pcm libVsdDict_rdict.pcm VsdDict_rdict.pcm
	rm -f FWDict.cc FWDict.o FWDict.pcm libFWDict_rdict.pcm FWDict_rdict.pcm
	rm -f FWEventManager.o VsdProxies.o
	rm -f *_dictContent.h *_dictUmbrella.h
	rm -f serviceVSDNano
	rm -f *.d


-include $(wildcard *.d)

