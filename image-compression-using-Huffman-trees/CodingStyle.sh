#!/bin/bash

function check_style {
    is_style_ok=1
    echo -e "\nRun cpplint\n" > result
    cpplint --filter=-legal/copyright,-readability/casting,-build/include_subdir,-runtime/threadsafe_fn,-build/header_guard,-runtime/int \
        --linelength=120 $( find . -name \*.h -or -name \*.c ) &>> result

    if [ ! $? -eq 0 ]; then
        is_style_ok=0
    fi

    echo -e "\nRun clang-tidy\n" >> result
    clang-tidy -checks='-*,cppcoreguidelines*' --warnings-as-errors='-*,cppcoreguidelines*' $( find . -name \*.h -or -name \*.c ) -- &>> result

    if [ ! $? -eq 0 ]; then
        is_style_ok=0
    fi

    # Display detailed report
    echo -e "\n================================"
    echo -e "= Detailed coding style report ="
    echo -e "================================\n"
    cat result
    echo " "
    echo " "
}

function run_checker {
    check_style
}

TIMEOUT_VAL=1

export -f run_checker check_style
run_checker
