#!/bin/bash

# Check the input data directory
if [ -ne ${INPUT_DATA_DIR} ]
then
    echo "Input data directory does not exist"
    exit 1
fi

# Check the output data directory
if [ -ne ${OUTPUT_DATA_DIR} ]
then
    echo "Output data directory does not exist"
    exit 1
fi

# Check the executable command
if [ -ne ${EXECUTABLE} ]
then
    echo "Executable file does not exist"
fi

INPUT_FILES=$(find ${INPUT_DATA_DIR} -type f)

# Define the execution function
doit(){
    ${EXECUTABLE} -i $1 -o ${OUTPUT_DATA_DIR}/$(basename $1)
}

export -f doit
parallel doit ::: ${INPUT_FILES}
