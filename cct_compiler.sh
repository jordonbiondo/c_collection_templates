#!/bin/bash

# insert everything above MACRO_DEFINE and turn MACRO_DEFINE into a real macro
cat $1 |
    sed '/MACRO_DEFINE/q' |
    sed -e 's/\/\/ *MACRO_DEFINE\(.*\)/#define\1 \\/g'

# take everything between MACRO_DEFINE and MACRO_END and squish it into on line,
# this line will be beneath the macro definition
# then replace prefix with prefix## so the macro will build the functions
# and replace dummy_type with chosen_type
# then append it to the output file
cat $1 |
    sed '/MACRO_END/q' |
    sed '$d' |
    tail -r |
    sed '/MACRO_DEFINE/q' |
    sed '$d' |
    tail -r |
    tr '\n' ' ' | tr -s ' ' |
    sed -e 's/prefix_/prefix##_/g' |
    sed -e 's/dummy_type/chosen_type/g'

# put all the stuff after macro end at the end of the file
tail -r $1 |
    sed '/MACRO_END/q' |
    tail -r
