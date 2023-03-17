@echo off
:: Make sure PATH has the tools in it
SET PATH=%PATH%;%~dp0..\XtendedInputButtons
SET BuildType=%1
SET GameName=%2
:: Prepare directories
echo Creating directories...
cd %~dp0../..
IF EXIST ".Release Packages" RMDIR /S /Q ".Release Packages"
MKDIR ".Release Packages\%BuildType%-%GameName%\GLOBAL"
MKDIR ".Release Packages\%BuildType%-%GameName%\scripts"
:: Summon the binary
echo Copying the binary
COPY /Y "src\%BuildType%-%GameName%\NFS_XtendedInput.asi" ".Release Packages\%BuildType%-%GameName%\scripts"
:: Summon text files
echo Summoning text files
COPY /Y "data\Config-%GameName%\NFS_XtendedInput.ini" ".Release Packages\%BuildType%-%GameName%\scripts"
COPY /Y "data\Config-%GameName%\EventReference.txt" ".Release Packages\%BuildType%-%GameName%"
COPY /Y "README.md" ".Release Packages\%BuildType%-%GameName%"

echo Post build done!
