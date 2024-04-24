
# UNIVERSAL DATA VISUALZIATION

# Main Idea
Provide a simple web-based service to read data in a custom format and visualize it using a set of high level objects such as jets, charged and neutral tracks, muons etc using custom detector geometry and a C++ macro to convert custom event representation to a standard format. The custom format is called Visual Summary Data (VSD) format.


# High Level Objects

A minimal set of high level objects includes

* Jet - a cone with some indication of jet energy
    * Momentum vector
    * Jet size - optional
    * EM and HAD fractions - optional

* VSDCandidate
    * position, eta, phi, pt
    * charge

* Muon (MIP) - a charged track that propagates through calorimeter
    * Charge
    * Momentum vector

* Vertex - a global point in 3D
    * 3D position
    * 3x3 matrix error, presented as ellipsoid

* CaloTower (status: incomplete)
    * eta, phi, pt presented as energy deposit

* RecHits (status: missing)
    * collection in points

See [VSDBase.h](/VsdBase.h) for all implementation details.

## Detector Geometry and Magnetic Field
Detector geometry is mostly needed as a background for high level objects, but it also contains some important information, such as the boundary of low material region (pixel and tracker volume), the detector boundary including the muon detectors and a simplified magnetic field model to visualize charged tracks properly.


Specific Cylindrical geometry input
* tracker propagator boundaries
    * float r = 139.5 cm;
    * float z = 290 cm;
* muon propagator boundaries
   * float r = 850 cm;
   * float z = 1100 cm;
* magentic field values are provided with specific implementation of REveMagField, changing value at radius 350 cm
REveMagFieldDuo(350, -3.5, 2.0));


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

## Write TTree with branches of vector of VSD structures
Write a vector of the VSD structures in a plain root tree and relate it the ROOT's tree branch. 
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
One needs to work with the tip of the ROOT master branch to modify the VSD structures and change their graphic representation.
Use -Droot7="ON" cmake flag to activete building of REve module.

### Building REve in ROOT 
```
git clone git@github.com:root-project/root.git
mkdir root-build
cd root-build
cmake -Dhttp="ON" -Droot7="ON" ../root
make
```
### Run event display locally
```
root.exe 'evd.h("UserVsd.root")'
```

