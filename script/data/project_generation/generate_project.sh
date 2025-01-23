#!/bin/bash

#find out script directory
pushd $(dirname "${BASH_SOURCE[0]}") > /dev/null
scriptdir=$(pwd)
popd > /dev/null

pushd $scriptdir

# -------------------------
#   Input sanitation
# -------------------------
# Check if any argument is provided.
if [ -z "$1" ]; then
    echo "Error: No argument provided. Please provide 'debian', 'win64', or another value."
    exit 1
fi

# Make input param case-insensitive.
input=$(echo "$1" | tr '[:upper:]' '[:lower:]')


# -------------------------
#   Variables
# -------------------------
projectfile="proj.txt"

# 'debian' | 'win'
macro_platform=""
macro_platform_compile_flags=''
macro_engine_source_files=''
macro_engine_header_files=''

# used at end of files, like <name>.debian.txt
platform_extension=''

# -------------------------
#   Variable Setup
# -------------------------
if [ "$input" == "debian" ]; then
  macro_platform=DEBIAN
  macro_platform_compile_flags="-Wl,--no-as-needed -lpthread -ldl"
  #macro_engine_source_files=''
  #macro_engine_header_files=''
  platform_extension=debian


  
elif [ "$input" == "win" ]; then
  macro_platform=WIN64
  macro_platform_compile_flags=''
  #macro_engine_source_files=''
  #macro_engine_header_files=''
  platform_extension=win64


else
    echo "Invalid parameter! Must be either 'debian' or 'win64'!"
    exit 1
fi


# Read file lists, prepare prefixes for them and store the results as temporary files
path_tmp=/tmp/pt
path_engine_sources="${path_tmp}"/engine_source_files.txt
path_engine_headers="${path_tmp}"/engine_header_files.txt
path_engine_includes="${path_tmp}"/engine_includes.txt
path_engine_links="${path_tmp}"/engine_links.txt
path_engine_test_sources="${path_tmp}"/engine_sources.txt
#path_engine_test_headers="${path_tmp}"/engine_headers.txt

mkdir -p "${path_tmp}"
rm "${path_engine_sources}"         > /dev/null 2>&1
rm "${path_engine_headers}"         > /dev/null 2>&1
rm "${path_engine_includes}"        > /dev/null 2>&1
rm "${path_engine_links}"           > /dev/null 2>&1
rm "${path_engine_test_sources}"    > /dev/null 2>&1
#rm "${path_engine_test_headers}"    > /dev/null 2>&1
touch "${path_engine_sources}"
touch "${path_engine_headers}"
touch "${path_engine_includes}"
touch "${path_engine_links}"
touch "${path_engine_test_sources}"
#touch "${path_engine_test_headers}"

# read file lists
while IFS= read -r line; do
  # Skip empty lines
  if [ -n "$line" ]; then
    #echo "    \${MY_PROJ_ROOT}/$line" >> "${path_engine_sources}"
    echo "    $line" | sed 's|\./|\${MY_PROJ_ROOT}/|g' >> "${path_engine_sources}"
  fi
done < ./__ENGINE_SOURCE_FILES__.txt

while IFS= read -r line; do
  # Skip empty lines
  if [ -n "$line" ]; then
    #echo "    \${MY_PROJ_ROOT}/$line" >> "${path_engine_headers}"
    echo "    $line" | sed 's|\./|\${MY_PROJ_ROOT}/|g' >> "${path_engine_headers}"
  fi
done < ./__ENGINE_HEADER_FILES__.txt

while IFS= read -r line; do
  # Skip empty lines
  if [ -n "$line" ]; then
    echo "    $line" | sed 's|\./|\${MY_PROJ_ROOT}/|g' >> "${path_engine_includes}"
  fi
done < ./__ENGINE_INCLUDES__.${platform_extension}.txt

while IFS= read -r line; do
  # Skip empty lines
  if [ -n "$line" ]; then
    echo "    $line" | sed 's|\./|\${MY_PROJ_ROOT}/|g' >> "${path_engine_links}"
  fi
done < ./__ENGINE_LINKS__.${platform_extension}.txt

while IFS= read -r line; do
  # Skip empty lines
  if [ -n "$line" ]; then
    echo "    $line" | sed 's|\./|\${MY_PROJ_ROOT}/|g' >> "${path_engine_test_sources}"
  fi
done < ./__ENGINE_TEST_SOURCES__.txt





# -------------------------
#   Main
# -------------------------
list_engine_sources=$(cat "${path_engine_sources}" | tr '\n' ';')
list_engine_headers=$(cat "${path_engine_headers}" | tr '\n' ';')
list_engine_includes=$(cat "${path_engine_includes}" | tr '\n' ';')
list_engine_libraries=$(cat "${path_engine_links}" | tr '\n' ';')
list_engine_test_sources=$(cat "${path_engine_test_sources}" | tr '\n' ';')
#list_engine_test_headers=$(cat "${path_engine_test_headers}" | tr '\n' ';')

cat "CMakeLists.txt.model" \
    | sed "s/__PLATFORM_MACRO__/${macro_platform}/g" \
    | sed "s/__PLATFORM_SPECIFIC_COMPILE_FLAGS__/${macro_platform_compile_flags}/g" \
    | sed "s|__SOURCE_FILES__|$list_engine_sources|g" \
    | sed "s|__HEADER_FILES__|$list_engine_headers|g" \
    | sed "s|__ENGINE_INCLUDES__|$list_engine_includes|g" \
    | sed "s|__ENGINE_LINKS__|$list_engine_libraries|g" \
    | sed "s|__ENGINE_TEST_SOURCES__|$list_engine_test_sources|g" \
    | tr ';' '\n' \
    > "${projectfile}"




















#cat "cmake_setup.model" \
#    > "${projectfile}"


#cat "project_macros.model" \
#    | sed  "s/__PLATFORM_MACRO__/${macro_platform}/g" \
#    | sed  "s/__PLATFORM_SPECIFIC_COMPILE_FLAGS__/${macro_platform_compile_flags}/g" \
#    >> "${projectfile}"


# ';' characters will be replaced with '\n' in project!
#list_engine_sources=$(cat "${path_engine_sources}" | tr '\n' ';')
#list_engine_headers=$(cat "${path_engine_headers}" | tr '\n' ';')
#cat "engine_files.model" \
#    | sed "s|__SOURCE_FILES__|$list_engine_sources|g" \
#    | sed "s|__HEADER_FILES__|$list_engine_headers|g" \
#    | tr ';' '\n' \
#   >> "${projectfile}"


# ';' characters will be replaced with '\n' in project!
#list_engine_includes=
#list_engine_libraries=
#cat "engine_dependencies.model" \
#    
#    >> "${projectfile}"
    
#cat "rest_of_project.model" \
#    >> "${projectfile}"




popd
# from scriptdir to initial dir

exit 0

