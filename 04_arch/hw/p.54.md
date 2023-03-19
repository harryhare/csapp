* task
  modify file `sim/pipe/pipe-full.hcl`


* compile
```
make psim VERSION=full
```

* test
```
#single
./psim -g ../y86-code/asumi.yo


#y86-code
make testpsimi

#ptest
make SIM=../pipe/psim TFLAGS=-i

```

