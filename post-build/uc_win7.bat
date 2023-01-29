@echo off
:: Prepare directories
echo Creating directories...
IF EXIST "Release-UC-Win7\Release-UC-Win7-Pack" RMDIR /S /Q "Release-UC-Win7\Release-UC-Win7-Pack"
MKDIR "Release-UC-Win7\Release-UC-Win7-Pack"
MKDIR "Release-UC-Win7\Release-UC-Win7-Pack\scripts"
:: Summon the binary
echo Copying the binary
COPY /Y "Release-UC-Win7\NFS_XtendedInput.asi" "Release-UC-Win7\Release-UC-Win7-Pack\scripts"
:: Summon text files
echo Summoning text files
COPY /Y "Config-UC\NFS_XtendedInput.ini" "Release-UC-Win7\Release-UC-Win7-Pack\scripts"
COPY /Y "Config-UC\EventReference.txt" "Release-UC-Win7\Release-UC-Win7-Pack"
COPY /Y "README.md" "Release-UC-Win7\Release-UC-Win7-Pack"

echo Post build done!
