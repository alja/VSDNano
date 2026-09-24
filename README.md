
# UNIVERSAL DATA VISUALIZATION

# Main Idea
Provide a simple web-based service to read data in a custom format and visualize it using a set of high level objects such as jets, charged and neutral tracks, muons etc using custom detector geometry and a C++ macro to convert custom event representation to a standard format. The custom format is called Visual Summary Data (VSD) format.


# High Level Physics Objects

At the moment only Jets, Candiates, Muon, MET, and Vertex are implemented.See [VSDBase.h](/VsdBase.h) for all implementation details.

* Jet - a cone with indication of jet energy
    * Momentum vector
    * Jet size - optional
    * EM and HAD fractions - optional

* Candidate
    * position, eta, phi, pt
    * charge

* Muon (MIP) - a charged track that propagates through calorimeter
    * Charge
    * Momentum vector

* Vertex - a global point in 3D
    * 3D position
    * 3x3 matrix error, presented as ellipsoid

* MET
    * arrow in space
    * length presents energy

<span style="color:green">
Missing ...

* CaloTower 
    * eta, phi, pt presented as energy deposit

* Hits
    * collection in points
    * what else ??? 

</span>

## Detector Geometry and Magnetic Field
Detector geometry is mostly needed as a background for high level objects, but it also contains some important information, such as the boundary of low material region (pixel and tracker volume), the detector boundary including the muon detectors and a simplified magnetic field model to visualize charged tracks properly.


Specific Cylindrical geometry input
* tracker propagator boundaries
    * r = 139.5 cm
    * z = 290 cm
* muon propagator boundaries
   * r = 850 cm
   * z = 1100 cm
* magentic field values are provided with implementation of ROOT::Experimental::REveMagField
    * r < 350 cm, value -3.5 T
    * r => 350 cm, value 2T


# Preview
Use existing randomly generated  samples in the universal data format service https://fireworks.cern.ch/cmsShowWeb/revetor-uni.cgi


# Workflow

## Build libraries 
The VSD libraries are not yet distributed in ROOT. Currently one needs to build them with the sources in this repository. 
Setup a ROOT environment and use [Makefile](/Makefile) to build libraries
```
git clone https://github.com/alja/VSDNano.git
make libVsdDict.co libFWDict.co
```

## Write TTree with branches with vector of VSD objects
Write a vector of the VSD structures in a plain root tree and relate it the ROOT's tree branch. See the snippet below:
See a python script example in [UserVsd.py](/UserVsd.py) 
```
Vtree = ROOT.TTree("VSD", "Custom plain VSD tree")

pcv = ROOT.std.vector('VsdCandidate')()
candBr = Vtree.Branch("PinkCands", pcv)

# fill the VSDCandidate branch by adding the VSD objects in the std::vector 

for j in 5:
    cnd = ROOT.VsdCandidate(
        ROOT.gRandom.Uniform(0.1, 20),
        ROOT.gRandom.Uniform(-2.5, 2.5),
        ROOT.gRandom.Uniform(-ROOT.TMath.Pi(), ROOT.TMath.Pi()))
    cnd.name = f"Candidate_{j}"
    pcv.push_back(cnd)

Vtree.Fill(;)

candBr.SetTitle(json.dumps(candCfg))
```

## Run event display through web service
Run the event display through the web service with the data sample once it is publicly available on eos location (e.g. /eos/user/a/amraktad/Fireworks-Test/ksmm_background.root)

https://fireworks.cern.ch/cmsShowWeb/revetor-uni.cgi

<br>

# Developers information

To change the VSD structures or their graphic representation, you need a recent ROOT master (or a development branch of it) built with REve. The steps below build ROOT, build VSDNano against it, make a sample VSD file with `UserVsd.py`, and view it with `evd_run`. They were run on vocms0102 as user `viz` in September 2026, on EL9 with the system gcc 11.5, cmake 3.31, git and cvmfs. Change the paths to match your own area.

## 1. Build ROOT from a development branch

ROOT is built with plain cmake and Makefiles and used straight from its build directory; no install is needed.

### Get the source

```bash
export TOP=/home/viz/universal-format/root-dev/master2   # any empty directory
mkdir -p $TOP && cd $TOP
git clone -b rhoz-axis+geo-check-master https://github.com/alja/root.git root
mkdir build
```

`$TOP/root` holds the source and `$TOP/build` the build tree. Replace the branch with the one you need.

### Configure

```bash
cd $TOP/build
cmake ../root \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_CXX_STANDARD=17 \
  -DCMAKE_INSTALL_PREFIX=$TOP/install \
  -Dhttp=ON -Dwebgui=ON -Droot7=ON \
  -Dbuiltin_civetweb=ON -Dbuiltin_openui5=ON \
  -Dbuiltin_ftgl=ON -Dbuiltin_vdt=ON -Dbuiltin_cfitsio=ON \
  -DPython3_EXECUTABLE=/cvmfs/sft.cern.ch/lcg/views/LCG_106/x86_64-el9-gcc13-opt/bin/python3 \
  > cmake.log 2>&1
```

| Option | Why |
| --- | --- |
| `http`, `webgui`, `root7`, `builtin_civetweb`, `builtin_openui5` | Build the web server and the web-based Eve (REve) that VSDNano uses. |
| `builtin_ftgl`, `builtin_vdt`, `builtin_cfitsio` | ROOT master stops at configure if these are not installed on the system, instead of quietly building its own copies. |
| `Python3_EXECUTABLE` | ROOT master needs Python 3.11 or newer; the EL9 system `python3` is 3.9. The LCG_106 interpreter on cvmfs is 3.11.9. |

Without `Python3_EXECUTABLE`, configure turns PyROOT off and the build later fails while generating `man/root.1` with `_Python3_EXECUTABLE-NOTFOUND: command not found`.

Check the `-- Enabled support for:` line near the end of `cmake.log`. It should list `builtin_civetweb`, `builtin_openui5`, `http`, `webgui`, `root7` and `pyroot`.

### Build

```bash
make -j30 > build.log 2>&1        # set -j to about the number of cores
```

This also builds the bundled LLVM/cling, so it takes a while. Over ssh, start it detached so it survives a logout:

```bash
setsid nohup bash -c 'make -j30 > build.log 2>&1; echo EXIT=$? >> build.log' < /dev/null > /dev/null 2>&1 &
```

The build is finished when `build.log` ends with `EXIT=0`.

### Use it

```bash
source $TOP/build/bin/thisroot.sh
root-config --version --features      # expect 6.41.xx, builtin_civetweb, builtin_openui5, webgui ...
```

PyROOT only works with the Python ROOT was built with. Under the system `python3`, `import ROOT` fails with "ROOT was built for Python 3.11.9, but you are running Python 3.9.25". Use `/cvmfs/sft.cern.ch/lcg/views/LCG_106/x86_64-el9-gcc13-opt/bin/python3` instead.

## 2. Build VSDNano with that ROOT

The [Makefile](/Makefile) takes every compile and link flag from `root-config`, so sourcing a ROOT's `thisroot.sh` is what picks the ROOT you build against.

### Get the source

```bash
cd /home/viz/universal-format
git clone git@github.com:alja/VSDNano.git sept-VSDNano
cd sept-VSDNano
```

After cloning, `ls` should show the sources (`Makefile`, `evd_run.cc`, `ui5/`, `data/` ...). If there is only `.git`, the checkout did not happen: delete the directory and clone again.

### Build

```bash
source /home/viz/universal-format/root-dev/master2/build/bin/thisroot.sh
which root-config rootcling          # both must come from the ROOT above
make -j8 all service > build.log 2>&1
```

| File | What it is |
| --- | --- |
| `libVsdDict.so` | Dictionary for the VSD data classes (`VsdBase.h`) |
| `libFWDict.so` | Event manager, collections and proxies for the display |
| `evd_run` | Standalone event display (the default `make` target) |
| `service` | Multi-session web service (needs `make service`) |

Both executables carry an rpath to their own directory and to ROOT's `lib/`. To check the linking:

```bash
ldd evd_run | grep -E "ROOTEve|libCore|not found"
```

Both libraries should point into your ROOT build's `lib/`, and nothing should say `not found`. When you switch to a different ROOT, run `make clean` before rebuilding.

## 3. Generate a sample VSD file with Python

Run [UserVsd.py](/UserVsd.py) from the checkout with the LCG Python 3.11; it writes `UserVsd.root` with 10 random events.

```bash
cd /home/viz/universal-format/sept-VSDNano
source /home/viz/universal-format/root-dev/master2/build/bin/thisroot.sh
PY=/cvmfs/sft.cern.ch/lcg/views/LCG_106/x86_64-el9-gcc13-opt/bin/python3
$PY UserVsd.py                       # writes UserVsd.root in the current directory
```

- **Use the Python that ROOT was built with.** Plain `python UserVsd.py`, which is also what `make UserVsd.root` runs, gets the EL9 Python 3.9 and fails with "ROOT was built for Python 3.11.9, but you are running Python 3.9.25".
- **Run it from the checkout.** The script calls `ROOT.gSystem.Load("libVsdDict.so")` with a relative name, so build VSDNano first and stay in its directory.

### What the file contains

`UserVsd.py` writes a `TTree` named `VSD`. Each branch is a `std::vector` of one VSD class:

| Branch | Class |
| --- | --- |
| `PinkCands` | `VsdCandidate` |
| `CaloTowers` | `VsdCaloTower` |
| `YellowJets` | `VsdJet` |
| `TestMETs` | `VsdMET` |
| `UMuon` | `VsdMuon` |
| `ErrVertex` | `VsdVertex` |
| `EventInfo` | `VsdEventInfo` |

Display settings for a collection go in its branch title as JSON:

```python
candBr = Vtree.Branch("PinkCands", pcv)
candBr.SetTitle(json.dumps({"filter": "i.pt() > 1", "color": ROOT.kViolet}))
```

`"filter"` is a cut on each item `i`, and `"color"` is a ROOT color. The vertex branch also passes extra parameters in `"var"` (`ScaleEllipse`, `MarkerSize`). A branch without a JSON title still displays; `evd_run` just prints a harmless `parse error <branch>_ at byte 1` for it.

To make your own sample, copy `UserVsd.py`. For each event, `clear()` the vectors, `push_back()` the objects and call `Vtree.Fill()`. Finish with `Vtree.Write()` and `Vfile.Close()`.

### Other generator scripts

| Script | Output | Input needed |
| --- | --- | --- |
| `UserVsd-Simple.py` | `UserVsd-Simple.root` | None (random events) |
| `UserVsd-CaloTower.py` | `UserVsdTower.root` | None (random events) |
| `UserVsd-NanoAOD.py` | `UserVsd-NanoAOD.root` | `data/BdToJpsiKShort.root` |
| `UserVsd-GenPart.py` | Adds `GenParticles` to `UserVsd-NanoAOD.root` | `data/BdToJpsiKShort.root` |
| `UserVsd-BdToJpsiKs.py` | Adds B0 to J/psi Ks branches to `UserVsd-NanoAOD.root` | `data/BdToJpsiKShort.root` |

The NanoAOD input `data/BdToJpsiKShort.root` is not in the repo; you have to supply it. Run `UserVsd-NanoAOD.py` first, then the other two add branches to its output.

## 4. Run evd_run

`evd_run` takes exactly one argument, a VSD `.root` file, and serves the event display over HTTPS.

```bash
cd /home/viz/universal-format/sept-VSDNano        # must run from the checkout
source /home/viz/universal-format/root-dev/master2/build/bin/thisroot.sh
./evd_run UserVsd.root                             # or ../samples/BdToJpsiKShort-vsd.root
```

- **Run it from the checkout.** It loads the geometry from the relative path `data/cms_extract.root` and serves the web UI from `ui5/`.
- **Ready-made samples** are in `/home/viz/universal-format/samples/`: `BdToJpsiKShort-vsd.root`, `UserVsd-*.root`, `UserVsdCaloTower.root`, `VertexErr.root`, `DisplacedJet.root`.
- **The port** comes from the `WebGui.HttpPortMin`/`HttpPortMax` range in `.rootrc`. The log prints it, for example `Starting HTTP server on port 10091s`; the trailing `s` means HTTPS.
- **To view it,** open `https://<host>:<port>/win1/`. vocms0102 is behind lxplus, so you may need a tunnel first: `ssh -L 10091:localhost:10091 vocms0102`, then open `https://localhost:10091/win1/`.
- **To keep it running after logout:** `setsid nohup ./evd_run <file> > evd_run.log 2>&1 < /dev/null &`. Stop it with `pkill -x evd_run`.
