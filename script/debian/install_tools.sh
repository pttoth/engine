#!/bin/bash

packages=""
packages="$packages kdiff3"
packages="$packages qtcreator"
packages="$packages glew-utils"


echo "installing packages:"
echo "  $packages"
echo ""
echo ""

sudo apt install $packages -y


