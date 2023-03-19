* task
  modify file `sim/pipe/pipe-lf.hcl`


* compile
```
make psim VERSION=lf
```

* test
```

#y86-code
make testpsim

#ptest
make SIM=../pipe/psim

```

