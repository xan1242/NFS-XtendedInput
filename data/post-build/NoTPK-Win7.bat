@echo off
:: Make sure PATH has the tools in it
SET PATH=%PATH%;%~dp0..\XtendedInputButtons
SET BuildType=%1
SET GameName=%2
:: Prepare directories
echo Creating directories...
cd %~dp0../..
IF EXIST ".Release Packages\%BuildType%-%GameName%-Win7" RMDIR /S /Q ".Release Packages\%BuildType%-%GameName%-Win7"
MKDIR ".Release Packages\%BuildType%-%GameName%-Win7\GLOBAL"
MKDIR ".Release Packages\%BuildType%-%GameName%-Win7\scripts"
:: Summon the binary
echo Copying the binary
COPY /Y "src\%BuildType%-%GameName%-Win7\NFS_XtendedInput.asi" ".Release Packages\%BuildType%-%GameName%-Win7\scripts"
:: Summon text files
echo Summoning text files
COPY /Y "data\Config-%GameName%\NFS_XtendedInput.ini" ".Release Packages\%BuildType%-%GameName%-Win7\scripts"
COPY /Y "data\Config-%GameName%\EventReference.txt" ".Release Packages\%BuildType%-%GameName%-Win7"
COPY /Y "README.md" ".Release Packages\%BuildType%-%GameName%-Win7"

echo Post build done!
