# Setting up a developers environemnt
```
git clone git@github.com:root-project/root.git
mkfir root-build
cd root-build
cmake -Dhttp="ON" -Droot7="ON" ../root
```


### Build libraries 
Setup root environment and use [Makefile](/Makefile) to build libraries
```
make libVsdDict.co libFWDict.co
```

### Generate a sample
```
python UserVsd.py
```
### Run event display with the data sample
```
root.exe 'evd.h("UserVsd.root")'
```
