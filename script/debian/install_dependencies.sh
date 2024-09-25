#!/bin/bash

packages=""
packages="$packages git"
packages="$packages cmake"
packages="$packages build-essential"
packages="$packages libglew-dev"
packages="$packages libsdl2-dev"
packages="$packages libpng-dev"
packages="$packages libassimp-dev"
#packages="$packages libasio-dev"

echo "installing packages:"
echo "  $packages"
echo ""
echo ""

sleep 5

apt install $packages -y
