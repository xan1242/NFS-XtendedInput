@echo off
:: Prepare directories
echo Creating directories...
IF EXIST "Debug-ProStreet-Win7\Debug-ProStreet-Win7-Pack" RMDIR /S /Q "Debug-ProStreet-Win7\Debug-ProStreet-Win7-Pack"
MKDIR "Debug-ProStreet-Win7\Debug-ProStreet-Win7-Pack"
MKDIR "Debug-ProStreet-Win7\Debug-ProStreet-Win7-Pack\GLOBAL"
MKDIR "Debug-ProStreet-Win7\Debug-ProStreet-Win7-Pack\scripts"
:: Summon the binary
echo Copying the binary
COPY /Y "Debug-ProStreet-Win7\NFS_XtendedInput.asi" "Debug-ProStreet-Win7\Debug-ProStreet-Win7-Pack\scripts"
:: Generate the TPK ini
echo Generating the TPK ini
CALL "XtendedInputButtons\TpkIniGen.bat" "XtendedInputButtons\8A69054A.ini"
:: Build the TPK -- YOU NEED TO HAVE XNFSTPKTool in your PATH!!!
echo Building TPK with XNFSTPKTool
xnfstpktool -w "XtendedInputButtons\8A69054A.ini" "Debug-ProStreet-Win7\Debug-ProStreet-Win7-Pack\GLOBAL\XtendedInputButtons.tpk"
:: Summon text files
echo Summoning text files
COPY /Y "Config-ProStreet\NFS_XtendedInput.ini" "Debug-ProStreet-Win7\Debug-ProStreet-Win7-Pack\scripts"
COPY /Y "Config-ProStreet\EventReference.txt" "Debug-ProStreet-Win7\Debug-ProStreet-Win7-Pack"
COPY /Y "README.md" "Debug-ProStreet-Win7\Debug-ProStreet-Win7-Pack"
:: Summon additional resources
echo Summoning additional resources
COPY /Y "Mouse Cursor\nfs_cursor.cur" "Debug-ProStreet-Win7\Debug-ProStreet-Win7-Pack\scripts"

echo Post build done!
