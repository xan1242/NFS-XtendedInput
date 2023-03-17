@echo off
:: Make sure PATH has the tools in it
SET PATH=%PATH%;%~dp0..\XtendedInputButtons
:: Prepare directories
echo Creating directories...
IF EXIST "Debug-World-Win7\Debug-World-Win7-Pack" RMDIR /S /Q "Debug-World-Win7\Debug-World-Win7-Pack"
MKDIR "Debug-World-Win7\Debug-World-Win7-Pack"
MKDIR "Debug-World-Win7\Debug-World-Win7-Pack\scripts"
:: Summon the binary
echo Copying the binary
COPY /Y "Debug-World-Win7\NFS_XtendedInput.asi" "Debug-World-Win7\Debug-World-Win7-Pack\scripts"
:: Summon text files
echo Summoning text files
COPY /Y "Config-World\NFS_XtendedInput.ini" "Debug-World-Win7\Debug-World-Win7-Pack\scripts"
COPY /Y "Config-World\EventReference.txt" "Debug-World-Win7\Debug-World-Win7-Pack"
COPY /Y "README.md" "Debug-World-Win7\Debug-World-Win7-Pack"

echo Post build done!
