set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
add_executable(cpstdin.o cpstdin.c)
add_executable(fdprob1.o fdprob1.c)
add_executable(fdprob2.o fdprob2.c)
add_executable(sharing1.o sharing1.c)
add_executable(sharing2.o sharing2.c)
add_executable(sharing3.o sharing3.c)
add_executable(statcheck.o statcheck.c)
add_executable(readdir.o readdir.c)
add_executable(openexamples.o openexamples.c)

add_subdirectory(test-readlineb)


add_executable(p.10.7.o hw/p.10.7.c)
add_executable(p.10.8.o hw/p.10.8.c)
add_executable(p.10.10.o hw/p.10.10.c)


