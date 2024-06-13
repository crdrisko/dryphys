### External Module File for DryChem ###
if (DryChem_INCLUDE_DIR)
    ## In cache already or user-specified ##
    set(DryChem_FOUND TRUE)
else()
    set(DryChem_FOUND FALSE)

    if (NOT COMMON_UTILS_INCLUDE_DIR)
        find_path(COMMON_UTILS_INCLUDE_DIR common-utils
                  PATHS /opt/local/include/common-utils
                        /opt/local/include
                        /usr/include/common-utils
                        /usr/include
                        /usr/local/include/common-utils
                        /usr/local/include
                        ~/include/common-utils
                        ~/include)
    endif()

    if (NOT CPP_UNITS_INCLUDE_DIR)
        find_path(CPP_UNITS_INCLUDE_DIR cpp-units
                  PATHS /opt/local/include/cpp-units
                        /opt/local/include
                        /usr/include/cpp-units
                        /usr/include
                        /usr/local/include/cpp-units
                        /usr/local/include
                        ~/include/cpp-units
                        ~/include)
    endif()

    if (COMMON_UTILS_INCLUDE_DIR AND CPP_UNITS_INCLUDE_DIR)
        set(DryChem_FOUND TRUE)
    endif()
endif()
