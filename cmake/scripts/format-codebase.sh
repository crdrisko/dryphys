#!/bin/bash
# Copyright (c) 2024 C.R. Drisko. All rights reserved.
# Licensed under the MIT License. See the LICENSE file in the project root for more information.
#
# Name: format-codebase.sh - Version 1.0.0
# Author: crdrisko
# Date: 06/12/2024-06:13:18
# Description: Use clang-format on all files in the repository with the option to ignore specified files


### Functions ###
printHelpMessage()      #@ DESCRIPTION: Print the format-code program's help message
{                       #@ USAGE: printHelpMessage
    printf "\nUSAGE: format-codebase [-h] [-i fileName]\n\n"
    printf "  -h  Prints help information about the format-codebase program.\n\n"
    printf "  -i  OPTIONAL: Filename and path (relative to project root) to ignore\n"
    printf "        when formatting.\n\n"
    printf "EXAMPLE: format-codebase -i include/dryphys/precision.hpp\n\n"
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
            /opt/local/libexec/llvm-16/bin/clang-format -i -style=file "$elem"

        elif [[ -d $elem ]]
        then
            cd "$elem" || ( printf "Could not change into required directory.\n" && exit 1 )

            formatFiles -- *

            cd ../
        fi
    done
}


### Initial Variables / Default Values ###
declare -a ignoreFiles


### Runtime Configuration ###
while getopts i:h opt
do
    case $opt in
        i) if [ "${OPTARG:$(( ${#OPTARG} - 1 )):1}" == '*' ]
           then
               for file in $PWD/$OPTARG
               do
                   ignoreFiles+=( "$file" )
               done
           else
               ignoreFiles+=( "$PWD/$OPTARG" )
           fi ;;
        h) printHelpMessage && exit 0 ;;
        *) printf "Invalid option flag passed to program.\n" && exit 1 ;;
    esac
done


### Main Code ###
formatFiles -- *
