#!/bin/bash

output=collection_templates.h
common=(src/cct_common_includes.h)
templates=(src/dynamic_list_template.h src/circular_buffer_template.h)

function start () {
    echo > $output
}

function include_common () {
    for file in "${common[@]}"; do
        cat $file >> $output
    done
}

function include_macrofied_files () {
    for file in "${templates[@]}"; do
        cat $file |
            sed '/MACRO_DEFINE/q' |
            sed -e 's/\/\/ *MACRO_DEFINE\(.*\)/#define\1 \\/g' >> $output

        # take everything between MACRO_DEFINE and MACRO_END and squish it into on line,
        # this line will be beneath the macro definition
        # then replace prefix with prefix## so the macro will build the functions
        # and replace dummy_type with chosen_type
        # then append it to the $output file
        cat $file |
            sed '/MACRO_END/q' |
            sed '$d' |
            tail -r |
            sed '/MACRO_DEFINE/q' |
            sed '$d' |
            tail -r |
            tr '\n' ' ' | tr -s ' ' |
            sed -e 's/prefix_/prefix##_/g' |
            sed -e 's/dummy_type/chosen_type/g' >> $output

        # put all the stuff after macro end at the end of the file
        tail -r $file |
            sed '/MACRO_END/q' |
            tail -r >> $output
    done
}


start
include_common
include_macrofied_files


