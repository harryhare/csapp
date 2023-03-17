
add_compile_options(-Wall -O2 -fcommon)
add_definitions(-DHAS_GUI)

link_directories(/usr/local/lib)
link_libraries(tk tcl)

include_directories(/usr/local/include)
include_directories(../misc)

add_executable(psim psim.c pipe-std.c ../misc/isa.c)


#target_compile_definitions(psim PRIVATE -DHAS_GUI)
#target_link_libraries(psim tk tcl)