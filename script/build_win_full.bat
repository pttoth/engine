@echo off
pushd %~dp0

call .\win\3download_and_build_dependencies.bat
@echo off

call .\build_project.bat
@echo off

popd
@echo on
