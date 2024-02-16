#!/bin/bash

#--------------------------------------------------
#  global vars
#--------------------------------------------------

bIsHpp="false"
bHasParent="false"
bHasNamespace="false"
bExplicitDefault="false"

sHeaderExt=".h"
sClassname="n/a"
sFileNameH="n/a"
sFileNameS="n/a"
sInclPathH="n/a"

sParentClassname="n/a"
sNamespace="n/a"

#TODO: read
sFilePathH=""
sFilePathS=""


#--------------------------------------------------
#  function definitions
#--------------------------------------------------

fnReadClassName()#CFG_START
{
	local sInBuffer=""

	while [ -z $sInBuffer ];
	do
		echo -n "class name: "
		read sInBuffer
	done

	sClassname=${sInBuffer}
	sFileNameH=${sInBuffer}
	sFileNameS=${sInBuffer}.cpp
	#sFileNameH=$(echo $ClassName | tr [:upper:] [:lower:])
	#sFileNameS=$(echo $ClassName | tr [:upper:] [:lower:])
}


fnReadHeaderExtension()
{
	local sInBuffer=""
	local bValid=""

	while [ -z $bValid ];
	do
		echo -n "Header is .h and NOT .hpp? (Y/n): "
		read sInBuffer

		if [ -z $sInBuffer ];
		then
			bValid=1
		elif [ $sInBuffer == "y" ] || [ $sInBuffer == "Y" ];
		then
			bValid=1
			bIsHpp="true"
			sHeaderExt=".h"
		elif [ $sInBuffer == "n" ] || [ $sInBuffer == "N" ] ;
		then
			bValid=1
			bIsHpp="false"
			sHeaderExt=".hpp"
		else
			bValid=""
			echo "invalid parameter!"
		fi
	done
	sFileNameH=${sFileNameH}${sHeaderExt}
	sFilePathH=${sFileNameH}
	sFilePathS=${sFileNameS}
}


fnReadFilePath()
{
	#check parameter count
	if [[ "$#" != 1 ]];
	then
		echo "error: invalid argument count"
		echo "  param1: path to project folder"
		return 1
	fi
	
	#local vars
	local lsProjectPath=$1
	local lsInBuffer=""
	local lbValid="" # "" or 1
	local lbFilesExist=0
	local lsFilePathH=""
	local lsFilePathS=""

	while [ -z $lbValid ];
	do
		lbValid=""
		lbFilesExist=0
		lsInBuffer=""
		echo -n "Path inside 'include/' and 'src/': "
		read lsInBuffer

		#TODO: check this, whether it defaults to root, etc.
		#		it shouldn't do root, and give error instead
		if [[ -n $lsInBuffer ]];
		then
			lsInBuffer=$lsInBuffer"/"
		fi

		lsFilePathH=${lsInBuffer}${sFileNameH}
		lsFilePathS=${lsInBuffer}${sFileNameS}
		sFilePathH='include/'${lsFilePathH}
		sFilePathS='src/'${lsFilePathS}
		sAbsPathH=${lsProjectPath}'/include/'${lsFilePathH}
		sAbsPathS=${lsProjectPath}'/src/'${lsFilePathS}
		
		sInclPathH=${lsFilePathH}

		
		#check whether files exist
		if [ -a $sAbsPathH ];
		then
			echo "ERROR: " $sAbsPathH " already exists!"
			lbFilesExist=1
			lbValid=""
		fi

		if [ -a $sAbsPathS ];
		then
			echo "ERROR: " $sAbsPathS " already exists!"
			lbFilesExist=1
			lbValid=""
		fi


		if [[ 0 == $lbFilesExist ]];
		then
			echo Header path: "include/"${lsFilePathH}
			echo Source path: "src/"${lsFilePathS}
			echo -n "OK? (Y/n) "

			lsFilePathH="${ProjectDir}/include/"${lsInBuffer}${sFileNameH}
			lsFilePathS="${ProjectDir}/src/"${lsInBuffer}${sFileNameS}


			lsInBuffer=""
			read lsInBuffer

			if [ -z $lsInBuffer ] || [[ $lsInBuffer == "y" ]] || [[ $lsInBuffer == "Y" ]];
			then
				lbValid=1
			else
				if [[ $lsInBuffer == "n" ]] || [[ $lsInBuffer == "N" ]] ;
				then
					lbValid=""
				else
					echo "invalid parameter!"
				fi
			fi
		fi
	done
}



fnReadParentClass()
{
	local sInBuffer=""
	local bValid=""

	while [ -z $bValid ];
	do
		echo "Inherits from a class? (y/N) "
		read sInBuffer

		if [ -z $sInBuffer ];
		then
			bValid=1
		elif [ $sInBuffer == "y" ] || [ $sInBuffer == "Y" ];
		then
			bValid=1
			bHasParent="true"
		elif [ $sInBuffer == "n" ] || [ $sInBuffer == "N" ];
		then
			bValid=1
			bHasParent="false"
			sParentClassname="n/a"
		else
			bValid=""
			echo "invalid parameter!"
		fi
	done

	sInBuffer=""
	if [ $bHasParent == "true" ]
	then
		while [ -z $sInBuffer ]
		do
			echo "Parent class name:"
			read sInBuffer
		done

		sParentClassname=$sInBuffer
	fi
}


fnReadNamespace()
{
	local sInBuffer=""
	local bValid=""

	while [[ -z $bValid ]]
	do
		echo -n "Add under a namespace? (y/N) "
		read sInBuffer

		if [ -z $sInBuffer ];
		then
			bValid=1
		elif [ $sInBuffer == "y" ] || [ $sInBuffer == "Y" ];
		then
			bValid=1
			bHasNamespace="true"
		elif [ $sInBuffer == "n" ] || [ $sInBuffer == "N" ];
		then
			bValid=1
			bHasNamespace="false"
			sNamespace="n/a"
		else
			bValid=""
			echo "invalid parameter!"
		fi
	done

	sInBuffer=""
	if [ $bHasNamespace == "true" ]
	then
		while [[ -z $sInBuffer ]]
		do
			echo "Namespace name:"
			read sInBuffer
		done

		sNamespace=$sInBuffer
	fi	
}


fnReadExplicitFunctions()
{
	local sInBuffer=""
	local bValid=""

	while [[ -z $bValid ]]
	do
		echo -n "Make functions explicit default? (y/N) "
		read sInBuffer
		if [ -z $sInBuffer ];
		then
			bValid=1
		elif [ $sInBuffer == "y" ] || [ $sInBuffer == "Y" ];
		then
			bValid=1
			bExplicitDefault="true"
		elif [ $sInBuffer == "n" ] || [ $sInBuffer == "N" ];
		then
			bValid=1
			bExplicitDefault="false"
		else
			bValid=""
			echo "invalid parameter!"
		fi
	done

}


#--------------------------------------------------
#  Script Start
#--------------------------------------------------

#find out script directory
pushd $(dirname "${BASH_SOURCE[0]}") > /dev/null
scriptdir=$(pwd)
popd > /dev/null

#at this point, the path is <projectpath>/script/..
#fix it to <projectpath>
ProjectDir=$(dirname $scriptdir)


#prompt the user for the class properties
fnReadClassName
fnReadHeaderExtension
fnReadFilePath $ProjectDir
#fnReadParentClass
#fnReadNamespace
fnReadExplicitFunctions


#print an overview of the answers
echo "--------------------------------------------------"
if [ $bHasNamespace == "true" ];
then
	echo "Namespace name:             " $sNamespace
fi

echo "Class name:                 " $sClassname
echo "Header file name:           " $sFileNameH
echo "Header file relative path:  " $sFilePathH
echo "Source file name:           " $sFileNameS
echo "Source file relative path:  " $sFilePathS

if [ $bHasParent == "true" ];
then
	echo "Parent class name:          " $sParentClassname
fi

if [ $bExplicitDefault == "true" ];
then
	echo "Explicit default functions: " $bExplicitDefault
fi
echo "--------------------------------------------------"



#--------------------------------------------------
#  create file
#--------------------------------------------------

mkdir -p $(dirname $sAbsPathH)
mkdir -p $(dirname $sAbsPathS)

#TODO: check again for existing files and silently rename them to <name>.bak, just in case someone created them since checking


#set up substitutions for string patterns
soParentInclude="\#include '$sParentClassname'.h"
soNamespaceOpen="//namespace ClassNameSpace{"
soNamespaceClose="//} // end of namespace 'ClassNameSpace'"
soClassName=$sClassname
soParentClassInherit=": public "$sParentClassname
soDefDefault=" = default"


if [[ bHasParent=="false" ]];
then
	soParentInclude=""
	soParentClassInherit=""
fi


if [[ bHasNamespace=="false" ]];
then
	soNamespace=""
	#soNamespaceOpen=""
	#soNamespaceClose=""
else
	#TODO: set up 'mkdir' for namespace paths
	echo
fi


if [[ bExplicitDefault=="false" ]];
then
	soDefDefault=""
fi

sIncludeLine="#include \"$sInclPathH\""

cat $scriptdir/data/newclass.h.model | sed "s/__parentInclude__/$soParentInclude/g" | sed "s|__namespaceOpen__|$soNamespaceOpen|g" | sed "s|__namespaceClose__|$soNamespaceClose|g" | sed "s/__className__/$soClassName/g" | sed "s/__parentClassInherit__/$soParentClassInherit/g" | sed "s/__defDefault__/$soDefDefault/g" > $scriptdir/../$sFilePathH
#cat $scriptdir/data/newclass.cpp.model | sed "s/__headerInclude__/$sIncludeLine/g" | sed "s/__className__/$soClassName/g" | sed "s/__usingNamespaceName__/\#using namespace ClassNameSpace;/g" > $sFilePathS

cat $scriptdir/data/newclass.cpp.model | sed -e "s|__headerInclude__|$sIncludeLine|g" | sed "s/__className__/$soClassName/g" | sed "s/__usingNamespaceName__/\/\/using namespace ClassNameSpace;/g" > $scriptdir/../$sFilePathS
