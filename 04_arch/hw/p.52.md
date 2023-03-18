* task
modify file `sim/seq/seq-full.hcl`

  
* compile
```
make ssim VERSION=full
```

* test
```
#single
./ssim -g ../y86-code/asumi.yo


#y86-code
make testssimi

#ptest
make SIM=../seq/ssim TFLAGS=-i

```



