### External Module File for DryChem ###
if (COMMON_UTILS_INCLUDE_DIR)
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

    if (COMMON_UTILS_INCLUDE_DIR)
        set(DryChem_FOUND TRUE)
    endif()
endif()
