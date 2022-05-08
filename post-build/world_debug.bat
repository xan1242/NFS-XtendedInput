@echo off
:: Prepare directories
echo Creating directories...
IF EXIST "Debug-World\Debug-World-Pack" RMDIR /S /Q "Debug-World\Debug-World-Pack"
MKDIR "Debug-World\Debug-World-Pack"
MKDIR "Debug-World\Debug-World-Pack\scripts"
:: Summon the binary
echo Copying the binary
COPY /Y "Debug-World\NFS_XtendedInput.asi" "Debug-World\Debug-World-Pack\scripts"
:: Summon text files
echo Summoning text files
COPY /Y "Config-World\NFS_XtendedInput.ini" "Debug-World\Debug-World-Pack\scripts"
COPY /Y "Config-World\EventReference.txt" "Debug-World\Debug-World-Pack"
COPY /Y "README.md" "Debug-World\Debug-World-Pack"

echo Post build done!
