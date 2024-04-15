ROOT_CFLAGS := `root-config --cflags`
ROOT_LDFLAGS := -L`root-config --libdir`

all: evd

nanovsd: libVsdNanoDict.so MyVsdNanoTree.class

clean:
	rm -f VsdDict.cc VsdDict_rdict.pcm libVsdDict.so
	rm -f service

### Vsd Tree and dicts

VsdDict.cc VsdDict_rdict.pcm &: VsdBase.h Vsd_Linkdef.h
	rootcling -I. -f VsdDict.cc VsdBase.h Vsd_Linkdef.h

libVsdDict.so: VsdDict.cc
	c++ -shared -fPIC -o libVsdDict.so ${ROOT_CFLAGS} VsdDict.cc

### Sample

UserVsd.root: UserVsd.py
	python UserVsd.py

### CMS

## run event display
evd: UserVsd.root libVsdDict.so UserVsd.root
	root.exe  -e 'gSystem->Load("libVsdDict.so")' 'evd.h("UserVsd.root")'

service: service.cc libVsdDict.so
	c++ ${ROOT_CFLAGS} `root-config --libs`  -lROOTEve -lROOTWebDisplay -lGeom -o $@ service.cc lego_bins.h # VsdTree.cc
	# c++ ${ROOT_CFLAGS} `root-config --libs`  -lROOTEve -lROOTWebDisplay -lGeom -L. -lVsdDict -o $@ service.cc lego_bins.h VsdTree.cc


