
cmake_minimum_required (VERSION 2.6)

project (GDPKonsoll)

add_library(GDPLibFunctions ../GDPLib/src/konkurranse.c ../GDPLib/src/kontroller.c ../GDPLib/src/log.c ../GDPLib/src/modell.c ../GDPLib/src/par.c ../GDPLib/src/person.c )

add_library(GDPTestFunctions ../GDPTest/src/test.c ../GDPTest/src/test_modell.c)

include_directories ("../GDPLib/src/" "../GDPTest/src/")

include_directories("src/")

add_executable(gubb src/util.c src/rapport.c src/main.c)

target_link_libraries (gubb GDPLibFunctions GDPTestFunctions)
