#!/bin/bash
# Copyright (c) 2024 C.R. Drisko. All rights reserved.
# Licensed under the MIT License. See the LICENSE file in the project root for more information.
#
# Name: format-codebase.sh - Version 1.3.0
# Author: crdrisko
# Date: 06/12/2024-06:13:18
# Description: Use clang-format on all files in the repository with the option to ignore specified files


### Functions ###
printHelpMessage()      #@ DESCRIPTION: Print the format-code program's help message
{                       #@ USAGE: printHelpMessage
    printf "\nUSAGE: format-codebase [-h] [-f path] [-i fileName]\n\n"
    printf "  -h  Prints help information about the format-codebase program.\n\n"
    printf "  -f  REQUIRED: Absolute path to clang-format.\n"
    printf "  -i  OPTIONAL: Filename and path (relative to project root) to ignore\n"
    printf "        when formatting.\n\n"
    printf "EXAMPLE: format-codebase -f clang-format -i include/engine2d/entityMemoryPool.hpp\n\n"
}

formatFiles()           #@ DESCRIPTION: Use clang-format to format each file in the repository
{                       #@ USAGE: formatFiles LIST
    for elem in "$@"
    do
        if [[ -f "$elem" && ("${elem##*.}" == cpp || "${elem##*.}" == hpp) ]]
        then
            for file in "${ignoreFiles[@]}"
            do
                if [[ "$PWD/$elem" == "$file" ]]
                then
                    continue 2;
                fi
            done

            printf "Formatting: %s\n" "$elem"
            "${formatterPath:?Path to clang-format is required.}" -i -style=file "$elem"
        elif [[ -d $elem ]]
        then
            cd "$elem" || ( printf "Could not change into required directory.\n" && exit 1 )

            formatFiles -- *

            cd ../
        fi
    done
}

main()                  #@ DESCRIPTION: Execute the main portion of the script's code
{                       #@ USAGE: main LIST
    declare -a ignoreFiles
    declare formatterPath

    local OPTIND opt

    while getopts f:i:h opt
    do
        case $opt in
            f)  formatterPath="${OPTARG}" ;;
            i)  if [ "${OPTARG:$(( ${#OPTARG} - 1 )):1}" == '*' ]
                then
                    for file in $PWD/$OPTARG
                    do
                        ignoreFiles+=( "$file" )
                    done
                else
                    ignoreFiles+=( "$PWD/$OPTARG" )
                fi ;;
            h)  printHelpMessage && exit 0 ;;
            *)  printf "Invalid option flag passed to program.\n" && exit 1 ;;
        esac
    done

    shift $((OPTIND-1))

    formatFiles -- *
}


### Main Code ###
main "$@"
