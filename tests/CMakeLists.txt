include(CTest)
include(Catch)

add_executable(nanoargsmod_test libnanoargsmod/nanoargsmod_test.cpp)
target_link_libraries(nanoargsmod_test PRIVATE nanoargsmod
                                               Catch2::Catch2WithMain)
catch_discover_tests(nanoargsmod_test)
