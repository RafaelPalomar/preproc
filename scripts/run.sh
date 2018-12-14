#!/bin/bash

# Check the input data directory
if [ ! -d ${INPUT_DATA_DIR} ]
then
    echo "Input data directory does not exist"
    exit 1
fi

# Check the output data directory
if [ ! -d ${OUTPUT_DATA_DIR} ]
then
    echo "Output data directory does not exist"
    exit 1
fi

# Check the executable command
if [ ! -f ${EXECUTABLE} ]
then
    echo "Executable file does not exist"
fi

INPUT_FILES=$(find ${INPUT_DATA_DIR} -type f)

flipx=""
if [ "${FLIPX}" = "1" ]
then
    export flipx="-x"
fi

flipy=""
if [ "${FLIPY}" = "1" ]
then
    export flipy="-y"
fi

# Define the execution function
doit(){
    mkdir -p ${OUTPUT_DATA_DIR}/$(basename $1)

    echo ${EXECUTABLE} -i $1 -o ${OUTPUT_DATA_DIR}/$(basename $1) -s ${SIZE:-0} ${flipx} ${flipy} -f ${FORMAT:-"nrrd"}
    ${EXECUTABLE} -i $1 -o ${OUTPUT_DATA_DIR}/$(basename $1) -s ${SIZE:-0} ${flipx} ${flipy} -f ${FORMAT:-"nrrd"}
}

export -f doit
parallel doit ::: ${INPUT_FILES}
