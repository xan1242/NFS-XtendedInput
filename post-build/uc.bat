@echo off
:: Prepare directories
echo Creating directories...
IF EXIST "Release-UC\Release-UC-Pack" RMDIR /S /Q "Release-UC\Release-UC-Pack"
MKDIR "Release-UC\Release-UC-Pack"
MKDIR "Release-UC\Release-UC-Pack\scripts"

:: Summon text files
echo Summoning text files
COPY /Y "Config-UC\NFS_XtendedInput.ini" "Release-UC\Release-UC-Pack\scripts"
COPY /Y "Config-UC\EventReference.txt" "Release-UC\Release-UC-Pack"
COPY /Y "README.md" "Release-UC\Release-UC-Pack"

echo Post build done!
