### Function to find and download external projects from Github ###
function(DryPhysExternalDownload)
    set(options)
    set(one_value_keywords PROJECT
                           REPOSITORY
                           TAG)
    set(multi_value_keywords)

    cmake_parse_arguments(EXTERN_ARGS "${options}" "${one_value_keywords}" "${multi_value_keywords}" ${ARGN})

    ## Download and unpack project at configure time ##
    configure_file(${DryPhys_SOURCE_DIR}/cmake/CMakeLists.txt.in ${EXTERN_ARGS_PROJECT}-download/CMakeLists.txt)

    ## Configuration step ##
    execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
                    RESULT_VARIABLE result
                    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${EXTERN_ARGS_PROJECT}-download)
    if (result)
        message(FATAL_ERROR "CMake step for ${EXTERN_ARGS_PROJECT} failed: ${result}")
    endif()

    ## Build Step ##
    execute_process(COMMAND ${CMAKE_COMMAND} --build .
                    RESULT_VARIABLE result
                    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${EXTERN_ARGS_PROJECT}-download)
    if (result)
        message(FATAL_ERROR "Build step for ${EXTERN_ARGS_PROJECT} failed: ${result}")
    endif()

    ## Add project directly to our build ##
    add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/${EXTERN_ARGS_PROJECT}-download/${EXTERN_ARGS_PROJECT}-src
                     ${CMAKE_CURRENT_BINARY_DIR}/${EXTERN_ARGS_PROJECT}-download/${EXTERL_ARGS_PROJECT}-build
                     EXCLUDE_FROM_ALL)
endfunction()


### Function to create a new test from a predefined naming template ###
function(DryPhysNewTest)
    set(options USEFILESYSTEM)
    set(one_value_keywords TESTNAME)
    set(multi_value_keywords)

    cmake_parse_arguments(TEST_ARGS "${options}" "${one_value_keywords}" "${multi_value_keywords}" ${ARGN})

    set(TESTPATH ${DryPhys_SOURCE_DIR}/tests)

    add_executable(Test${TEST_ARGS_TESTNAME} ${TESTPATH}/test${TEST_ARGS_TESTNAME}.cpp)

    target_link_libraries(Test${TEST_ARGS_TESTNAME} ${GTEST_LIBRARIES} Threads::Threads)

    if (TEST_ARGS_USEFILESYSTEM AND CMAKE_COMPILER_IS_GNUCXX)
        target_link_libraries(Test${TEST_ARGS_TESTNAME} stdc++fs)
    endif()

    gtest_discover_tests(Test${TEST_ARGS_TESTNAME} WORKING_DIRECTORY ${TESTPATH})
endfunction()
