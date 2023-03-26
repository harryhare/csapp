the `sim` is downloaded from the website
http://csapp.cs.cmu.edu/3e/archlab-handout.tar (pdf instructions include)
or
http://csapp.cs.cmu.edu/3e/sim.tar (only source files)

you may  encounter error when try to compile the files
1. install depency
```bash
sudo apt install flex
```

1.1 
this won't work
```bash
apt install tcl-dev tk-dev
```
need install tcl & tk from source code
https://sourceforge.net/projects/tcl/files/Tcl/

both tk & tcl need to be compiled
https://www.tcl.tk/doc/howto/compile.html
```
cd unix
./configure
make
make install
```


2. change gcc flag

https://stackoverflow.com/questions/63152352/fail-to-compile-the-y86-simulatur-csapp
```text
-fcommon
```

3. undefined reference to `matherr'
https://stackoverflow.com/questions/52903488/fail-to-build-y86-64-simulator-from-sources
```c
extern int matherr();
int *tclDummyMathPtr = (int *) matherr;
```