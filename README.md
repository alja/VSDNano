
# Visual Summary Data Format Visualization

## Main Idea
Provide a simple web-based service to read data in a custom format and visualize it using a set of high level objects such as jets, charged and neutral tracks, muons etc using custom detector geometry and a C++ macro to convert custom event representation to a standard format. The custom format is called Visual Summary Data (VSD) format.


### High Level Objects

A minimal set of high level objects includes

* Jet - a cone with some indication of jet energ
    * Momentum vector
    * Jet size - optional
    * EM and HAD fractions - optional

* Charged hadron - a track that stops at the calorimeter surface
    * Charge
    * Momentum vector
    * ECAL and HCAL energy - optional
    * Neutral hadron - energy deposition in calorimeter
    * Momentum vector
    * EM and HAD fractions - optional

* Muon (MIP) - a charged track that propagates through calorimeter
    * Charge
    * Momentum vector

* Vertex - a global point in 3D
    * 3D position
    * Error

See [VSDBase.h](/VsdBase.h) for all implementation details.

## Detector Geometry and Magnetic Field
Detector geometry is mostly needed as a background for high level objects, but it also contains some important information, such as the boundary of low material region (pixel and tracker volume), the detector boundary including the muon detectors and a simplified magnetic field model to visualize charged tracks properly.


## Workflow

### Build libraries 
The VSD libraries are not yet distributed in ROOT. Currently one needs to build them with the sources in this repository. 
Setup a ROOT environment and use [Makefile](/Makefile) to build libraries
```
make libVsdDict.co libFWDict.co
```

## Generate a sample
Write a vector of the VSD structures in a plain root tree and relate it the ROOT's tree branch. See an example in [UserVsd.py](/UserVsd.py) 
```
python UserVsd.py
```

## Run event display through web service
When file is publicly available through eos or xrootd, one can run the event display through the web service

https://fireworks.cern.ch/cmsShowWeb/revetor-uni.cgi


## Developers's info

### Building ROOT with web gui enabled 
```
git clone git@github.com:root-project/root.git
mkdir root-build
cd root-build
cmake -Dhttp="ON" -Droot7="ON" ../root
```
### Run event display locally
```
root.exe 'evd.h("UserVsd.root")'
```

## Universal data format web service
### build service and run
```
make service
service --port 5566
```
Use script to run @CERN 
```
/home/viz/universal-format/runUniversalServ.sh
```
