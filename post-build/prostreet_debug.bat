@echo off
:: Prepare directories
echo Creating directories...
IF EXIST "Debug-ProStreet\Debug-ProStreet-Pack" RMDIR /S /Q "Debug-ProStreet\Debug-ProStreet-Pack"
MKDIR "Debug-ProStreet\Debug-ProStreet-Pack"
MKDIR "Debug-ProStreet\Debug-ProStreet-Pack\GLOBAL"
MKDIR "Debug-ProStreet\Debug-ProStreet-Pack\scripts"
:: Summon the binary
echo Copying the binary
COPY /Y "Debug-ProStreet\NFS_XtendedInput.asi" "Debug-ProStreet\Debug-ProStreet-Pack\scripts"
:: Build the TPK -- YOU NEED TO HAVE XNFSTPKTool in your PATH!!!
echo Building TPK with XNFSTPKTool
xnfstpktool -w "XtendedInputButtons\8A69054A.ini" "Debug-ProStreet\Debug-ProStreet-Pack\GLOBAL\XtendedInputButtons.tpk"
:: Summon text files
echo Summoning text files
COPY /Y "Config-ProStreet\NFS_XtendedInput.ini" "Debug-ProStreet\Debug-ProStreet-Pack\scripts"
COPY /Y "Config-ProStreet\EventReference.txt" "Debug-ProStreet\Debug-ProStreet-Pack"
COPY /Y "README.md" "Debug-ProStreet\Debug-ProStreet-Pack"
:: Summon additional resources
echo Summoning additional resources
COPY /Y "Mouse Cursor\nfs_cursor.cur" "Debug-ProStreet\Debug-ProStreet-Pack\scripts"

echo Post build done!
