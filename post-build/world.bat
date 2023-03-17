@echo off
:: Make sure PATH has the tools in it
SET PATH=%PATH%;%~dp0..\XtendedInputButtons
:: Prepare directories
echo Creating directories...
IF EXIST "Release-World\Release-World-Pack" RMDIR /S /Q "Release-World\Release-World-Pack"
MKDIR "Release-World\Release-World-Pack"
MKDIR "Release-World\Release-World-Pack\scripts"
:: Summon the binary
echo Copying the binary
COPY /Y "Release-World\NFS_XtendedInput.asi" "Release-World\Release-World-Pack\scripts"
:: Summon text files
echo Summoning text files
COPY /Y "Config-World\NFS_XtendedInput.ini" "Release-World\Release-World-Pack\scripts"
COPY /Y "Config-World\EventReference.txt" "Release-World\Release-World-Pack"
COPY /Y "README.md" "Release-World\Release-World-Pack"

echo Post build done!
