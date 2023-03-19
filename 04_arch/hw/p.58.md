* task
  modify file `sim/pipe/pipe-1w.hcl`


* compile
```
make psim VERSION=1w
```

* test
```

#y86-code
make testpsim

#ptest
make SIM=../pipe/psim

```

