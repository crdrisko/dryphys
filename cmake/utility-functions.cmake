### Function to create a new test from a predefined naming template ###
function(DryPhysNewTest)
    set(options USEFILESYSTEM)
    set(one_value_keywords TESTNAME)
    set(multi_value_keywords)

    cmake_parse_arguments(TEST_ARGS "${options}" "${one_value_keywords}" "${multi_value_keywords}" ${ARGN})

    set(TESTPATH ${DryPhys_SOURCE_DIR}/tests)

    add_executable(Test${TEST_ARGS_TESTNAME} ${TESTPATH}/test${TEST_ARGS_TESTNAME}.cpp)

    target_link_libraries(Test${TEST_ARGS_TESTNAME} dryphys ${GTEST_LIBRARIES} Threads::Threads)

    set_target_properties(Test${TEST_ARGS_TESTNAME} 
                          PROPERTIES INSTALL_RPATH "${CMAKE_INSTALL_RPATH};${CMAKE_INSTALL_PREFIX}/lib;/usr/local/lib") 

    if (TEST_ARGS_USEFILESYSTEM AND CMAKE_COMPILER_IS_GNUCXX)
        target_link_libraries(Test${TEST_ARGS_TESTNAME} stdc++fs)
    endif()

    gtest_discover_tests(Test${TEST_ARGS_TESTNAME} WORKING_DIRECTORY ${TESTPATH})
endfunction()
