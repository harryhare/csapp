
add_compile_options(-Wall -O2 -fcommon)
add_definitions(-DHAS_GUI)

link_directories(/usr/local/lib)
link_libraries(tk tcl)

include_directories(/usr/local/include)
include_directories(../misc)


#add_custom_command(
#        OUTPUT seq-std.c
#        COMMAND "../misc/hcl2c <seq-std.hcl >seq-std.c"
#        DEPENDS seq-std.hcl
#)
#add_custom_command(
#        OUTPUT seq-std.c
#        COMMAND ../misc/hcl2c <seq-std.hcl >seq-std.c
#        INPUT seq-std.hcl
#)
#add_executable(ssim ssim.c seq.c ../misc/isa.c)
#add_executable(ssim ssim.c seq-std.c ../misc/isa.c)
#add_executable(ssim ssim.c seq+.c ../misc/isa.c)
#add_executable(ssim ssim.c seq+-std.c ../misc/isa.c)
add_executable(ssim ssim.c seq-full.c ../misc/isa.c)



#target_compile_definitions(ssim PRIVATE -DHAS_GUI)
#target_link_libraries(ssim tk tcl)