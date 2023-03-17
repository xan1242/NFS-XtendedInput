@echo off
:: Make sure PATH has the tools in it
SET PATH=%PATH%;%~dp0..\XtendedInputButtons
:: Prepare directories
echo Creating directories...
IF EXIST "Release-World-Win7\Release-World-Win7-Pack" RMDIR /S /Q "Release-World-Win7\Release-World-Win7-Pack"
MKDIR "Release-World-Win7\Release-World-Win7-Pack"
MKDIR "Release-World-Win7\Release-World-Win7-Pack\scripts"
:: Summon the binary
echo Copying the binary
COPY /Y "Release-World-Win7\NFS_XtendedInput.asi" "Release-World-Win7\Release-World-Win7-Pack\scripts"
:: Summon text files
echo Summoning text files
COPY /Y "Config-World\NFS_XtendedInput.ini" "Release-World-Win7\Release-World-Win7-Pack\scripts"
COPY /Y "Config-World\EventReference.txt" "Release-World-Win7\Release-World-Win7-Pack"
COPY /Y "README.md" "Release-World-Win7\Release-World-Win7-Pack"

echo Post build done!
