@echo off
:: Make sure PATH has the tools in it
SET PATH=%PATH%;%~dp0..\XtendedInputButtons
:: Prepare directories
echo Creating directories...
IF EXIST "Debug-UC-Win7\Debug-UC-Win7-Pack" RMDIR /S /Q "Debug-UC-Win7\Debug-UC-Win7-Pack"
MKDIR "Debug-UC-Win7\Debug-UC-Win7-Pack"
MKDIR "Debug-UC-Win7\Debug-UC-Win7-Pack\scripts"
:: Summon the binary
echo Copying the binary
COPY /Y "Debug-UC-Win7\NFS_XtendedInput.asi" "Debug-UC-Win7\Debug-UC-Win7-Pack\scripts"
:: Summon text files
echo Summoning text files
COPY /Y "Config-UC\NFS_XtendedInput.ini" "Debug-UC-Win7\Debug-UC-Win7-Pack\scripts"
COPY /Y "Config-UC\EventReference.txt" "Debug-UC-Win7\Debug-UC-Win7-Pack"
COPY /Y "README.md" "Debug-UC-Win7\Debug-UC-Win7-Pack"

echo Post build done!
