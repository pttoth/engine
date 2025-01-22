#!/bin/bash

# -------------------------
#   Input sanitation
# -------------------------
# Check if any argument is provided.
if [ -z "$1" ]; then
    echo "Error: No argument provided. Please provide 'debian', 'win64', or another value."
    return 1
fi

# Make input param case-insensitive.
input=$(echo "$1" | tr '[:upper:]' '[:lower:]')


# -------------------------
#   Variables
# -------------------------
projectfile="proj.txt"

# 'debian' | 'win64'
macro_platform=""




# -------------------------
#   Variable Setup
# -------------------------
if [ "$input" == "debian" ]; then
  macro_platform=DEBIAN
    
elif [ "$input" == "win64" ]; then
  macro_platform=WIN64
  
else
    echo "Invalid parameter! Must be either 'debian' or 'win64'!"
    return 1
fi


# -------------------------
#   Main
# -------------------------
cat "cmake_setup.model" \
    > "${projectfile}"

cat "project_macros.model" \
    | sed  "s/__PLATFORM_MACRO__/${macro_platform}/g" \
    >> "${projectfile}"

cat "engine_files.model" \
    >> "${projectfile}"

cat "engine_dependencies.model" \
    >> "${projectfile}"
    
cat "rest_of_project.model" \
    >> "${projectfile}"



function SetupVariables {
    
}





return 0

