@echo off
pushd %~dp0

call .\set_env.bat
@echo off

@echo .
@echo .
@echo --------------------------------------------------
@echo Please delete the temporary files by hand
@echo An automatic folder delete command is dangerous to run
@echo --------------------------------------------------
@echo .
@echo .
@echo .
pause

@echo "You can delete everything in this folder."            >  "%temp_dir%\_delete_everything_in_this_folder.txt"
@echo "They were build files that are no longer needed."     >> "%temp_dir%\_delete_everything_in_this_folder.txt"


::DOESN'T WORK
::explorer "%temp_dir%"

::workaround, because batch is a degenerate language...
pushd "%temp_dir%"
explorer .
popd

::recursive folder delete command
::rmdir /s "%temp_dir%" /q

popd
@echo on
