add_executable(01 01.c csapp.c)
add_executable(test read.c)

target_link_libraries(01 pthread)