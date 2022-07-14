add_executable(echoclient echoclient.c)
add_executable(select select.c echo.c)
add_executable(echoserverp echoserverp.c echo.c)
add_executable(echoservers echoservers.c echo.c)
add_executable(echoservert echoservert.c echo.c)
add_executable(echoservert_pre echoservert_pre.c sbuf.c echo_cnt.c)
add_executable(echoservert_cnt echoservert_cnt.c)

add_executable(hello hello.c)
add_executable(hellobug hellobug.c)

add_executable(goodcnt goodcnt.c)
add_executable(badcnt badcnt.c)

add_executable(ctime_ts ctime_ts.c)

add_executable(sharing sharing.c)

add_executable(threadunsafe threadunsafe.c)

add_executable(race race.c)

add_executable(psum-array psum-array.c)
add_executable(psum-local psum-local.c)
add_executable(psum-mutex psum-mutex.c)
add_executable(vsum vsum.c)
target_link_libraries(vsum m)


add_executable(rand rand.c)
add_executable(test test.c)

add_executable(p.16 hw/p.16.c)
add_executable(p.17 hw/p.17.c)
add_executable(p.22 hw/p.22.c)
add_executable(p.23 hw/p.23.c echo.c)
add_executable(p.26 hw/p.26.c)
add_executable(p.31 hw/p.31.c)
add_executable(p.32 hw/p.32.c)
add_executable(p.33 hw/p.33.c)
add_executable(p.34 hw/p.34.c)
target_link_libraries(p.34 m)

add_executable(p.35 hw/p.35.c)
add_executable(p.36 hw/p.36.c)
add_executable(p.37 hw/p.37.c)
add_executable(p.38 hw/p.38.c sbuf.c)
