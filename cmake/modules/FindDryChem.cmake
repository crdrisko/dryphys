### External Module File for DryChem ###
if (DRYCHEM_INCLUDE_DIR)
    ## In cache already or user-specified ##
    set(DryChem_FOUND TRUE)
else()
    set(DryChem_FOUND FALSE)

    if (NOT DRYCHEM_INCLUDE_DIR)
        find_path(DRYCHEM_INCLUDE_DIR drychem
                  PATHS /opt/local/include/drychem
                        /opt/local/include
                        /usr/include/drychem
                        /usr/include
                        /usr/local/include/drychem
                        /usr/local/include
                        ~/include/drychem
                        ~/include)
    endif()

    if (DRYCHEM_INCLUDE_DIR)
        set(DryChem_FOUND TRUE)
    endif()
endif()
