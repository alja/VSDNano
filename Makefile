
#===============================================================================
# Configuration
#===============================================================================

CPPFLAGS := -I. -I$(shell root-config --incdir)
CXXFLAGS := -O0 -g -fPIC $(shell root-config --auxcflags)
ROOT_LIBS := $(shell root-config --libs)

CXX ?= c++
#===============================================================================
# Default
#===============================================================================

all: evd_run


#===============================================================================
# VSD DICTIONARY
#===============================================================================

VsdDict.cc: VsdBase.h Vsd_Linkdef.h
	@rm -f VsdDict.cc VsdDict_rdict.pcm
	rootcling -I. -f VsdDict.cc \
	    VsdBase.h \
	    Vsd_Linkdef.h

VsdDict.o: VsdDict.cc
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

libVsdDict.so: VsdDict.o
	$(CXX) -shared -o $@ \
	    VsdDict.o \
	    $(ROOT_LIBS)


#===============================================================================
# GRAPHICAL DICTIONARY
#===============================================================================

FWDict.cc: FWEventManager.h FWDataCollection.h VsdProxies.h FW_Linkdef.h
	@rm -f FWDict.cc FWDict_rdict.pcm
	rootcling -I. -f FWDict.cc \
	    FWEventManager.h \
	    FWDataCollection.h \
	    VsdProxies.h \
	    FW_Linkdef.h

FWDict.o: FWDict.cc
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

FWEventManager.o: FWEventManager.cc FWEventManager.h
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

VsdProxies.o: VsdProxies.cc VsdProxies.h
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

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
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $@ \
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


#===============================================================================
# CLEAN
#===============================================================================

clean:
	rm -f evd_run
	rm -f libVsdDict.so libFWDict.so
	rm -f VsdDict.cc VsdDict.o VsdDict_rdict.pcm
	rm -f FWDict.cc FWDict.o FWDict_rdict.pcm
	rm -f FWEventManager.o VsdProxies.o
	rm -f *_dictContent.h *_dictUmbrella.h

