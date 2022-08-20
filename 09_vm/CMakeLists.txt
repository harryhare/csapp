add_executable(map map.c)
add_executable(sizeof sizeof.c)
add_executable(mmapcopy mmapcopy.c)
add_executable(staticsize staticsize.c)
add_executable(dynamicsize dynamicsize.c)
add_executable(sbrk sbrk.c)
add_executable(mallocalign mallocalign.c)

add_subdirectory(malloc)