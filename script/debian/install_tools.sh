#!/bin/bash

packages=""
packages="$packages kdiff3"
packages="$packages qtcreator"


echo "installing packages:"
echo "  $packages"
echo ""
echo ""

sudo apt install $packages -y


