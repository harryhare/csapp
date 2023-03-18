* task
modify file `sim/pipe/pipe-nobypass.hcl`

  
* compile
```
make psim VERSION=nobypass
```

* test
```
#single
./pipe -g ../y86-code/asumi.yo


#y86-code
make testpsim

#ptest
make SIM=../pipe/psim

```



