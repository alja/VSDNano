ROOT_CFLAGS := `root-config --cflags`
ROOT_LDFLAGS := -L`root-config --libdir`

all: evd

clean:
	rm -f VsdDict.cc VsdDict_rdict.pcm libVsdDict.so
	rm -f FWDict.cc FWDict_rdict.pcm libFWDict.so
	rm -f service

### Vsd Tree and dicts

VsdDict.cc VsdDict_rdict.pcm &: VsdBase.h Vsd_Linkdef.h
	rootcling -I. -f VsdDict.cc VsdBase.h Vsd_Linkdef.h

libVsdDict.so: VsdDict.cc
	c++ -shared -fPIC -o libVsdDict.so ${ROOT_CFLAGS} VsdDict.cc

### Graphical Dict

FWDict.cc FWDict_rdict.pcm &: FWClasses.h FWDataCollection.h FW_Linkdef.h
	rootcling -I. -f FWDict.cc FWClasses.h FWDataCollection.h FW_Linkdef.h

libFWDict.so: FWDict.cc
	c++ -shared -fPIC -o libFWDict.so ${ROOT_CFLAGS} FWDict.cc


### Sample

UserVsd.root: UserVsd.py
	python UserVsd.py


## run event display
evd: UserVsd.root libVsdDict.so libFWDict.so
	root.exe 'evd.h("UserVsd.root")'

service: service.cc evd.h libVsdDict.so libFWDict.so
	c++ ${ROOT_CFLAGS} `root-config --libs`  -lROOTEve -lROOTWebDisplay -lGeom -o $@ service.cc lego_bins.h
