#!/bin/bash

packages=""
packages="$packages git"
packages="$packages cmake"
packages="$packages build-essential"
packages="$packages libglew-dev"
packages="$packages libsdl2-dev"
packages="$packages libpng-dev"
packages="$packages libassimp-dev"

echo "installing packages:"
echo "  $packages"
echo ""
echo ""

sudo apt install $packages -y


