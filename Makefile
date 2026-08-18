CPPFLAGS := -I$(shell root-config --incdir)
CXXFLAGS := -O0 -g -fPIC $(shell root-config --auxcflags)

ROOT_LIBS := $(shell root-config --libs)

#===============================================================================
#=================== DEFAULT ===================================================
#===============================================================================

all: evd_run

#===============================================================================
#=================== VSD DICTIONARY =============================================
#===============================================================================

VsdDict.cc: VsdBase.h Vsd_Linkdef.h
	@rm -f VsdDict.cc VsdDict_rdict.pcm
	rootcling -f VsdDict.cc VsdBase.h Vsd_Linkdef.h

VsdDict.o: VsdDict.cc
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -fPIC -c $<


FWEventManager.o: FWEventManager.cc
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -fPIC -c $<

libVsdDict.so: VsdDict.o FWEventManager.o
	$(CXX) -shared -o $@ \
	    VsdDict.o FWEventManager.o \
	    $(ROOT_LIBS)

#===============================================================================
#=================== GRAPHICAL DICTIONARY ======================================
#===============================================================================

FWDict.cc: FWEventManager.h FWDataCollection.h FW_Linkdef.h
	@rm -f FWDict.cc FWDict_rdict.pcm
	rootcling -I. -f FWDict.cc \
	    FWEventManager.h \
	    FWDataCollection.h \
	    FW_Linkdef.h

FWDict.o: FWDict.cc
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -fPIC -c $<

libFWDict.so: FWDict.o
	$(CXX) -shared -o $@ \
	    FWDict.o \
	    $(ROOT_LIBS)

#===============================================================================
#=================== SAMPLE ====================================================
#===============================================================================

UserVsd.root: UserVsd.py
	python UserVsd.py

#===============================================================================
#=================== EXECUTABLE ================================================
#===============================================================================

evd_run: evd_run.cc libVsdDict.so libFWDict.so
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $@ \
	    evd_run.cc FWCollectionManager.h \
	    -L. -Wl,-rpath,'$$ORIGIN' \
	    -Wl,--no-as-needed \
	    -lVsdDict \
	    -lFWDict \
	    -Wl,--as-needed \
	    -lEG \
	    -lGeom \
	    -lROOTWebDisplay \
	    -lROOTEve \
	    $(ROOT_LIBS)

#===============================================================================
#=================== CLEAN =====================================================
#===============================================================================

clean:
	rm -f VsdDict.cc VsdDict.o VsdDict_rdict.pcm
	rm -f FWDict.cc FWDict.o FWDict_rdict.pcm
	rm -f libVsdDict.so libFWDict.so FWEventManager.o
	rm -f evd_run
	rm -f FW*_dictContent.h
