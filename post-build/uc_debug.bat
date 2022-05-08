@echo off
:: Prepare directories
echo Creating directories...
IF EXIST "Debug-UC\Debug-UC-Pack" RMDIR /S /Q "Debug-UC\Debug-UC-Pack"
MKDIR "Debug-UC\Debug-UC-Pack"
MKDIR "Debug-UC\Debug-UC-Pack\scripts"

:: Summon text files
echo Summoning text files
COPY /Y "Config-UC\NFS_XtendedInput.ini" "Debug-UC\Debug-UC-Pack\scripts"
COPY /Y "Config-UC\EventReference.txt" "Debug-UC\Debug-UC-Pack"
COPY /Y "README.md" "Debug-UC\Debug-UC-Pack"

echo Post build done!
