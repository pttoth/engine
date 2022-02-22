@echo off
pushd %~dp0

call .\set_env.bat
@echo off

call .\fetch_and_build_all.bat
@echo off

call .\4clean_up_temp.bat
@echo off

popd
@echo on
