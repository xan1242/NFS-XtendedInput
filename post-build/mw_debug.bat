@echo off
:: Prepare directories
echo Creating directories...
IF EXIST "Debug-MW\Debug-MW-Pack" RMDIR /S /Q "Debug-MW\Debug-MW-Pack"
MKDIR "Debug-MW\Debug-MW-Pack"
MKDIR "Debug-MW\Debug-MW-Pack\Global"
MKDIR "Debug-MW\Debug-MW-Pack\scripts"
:: Summon the binary
echo Copying the binary
COPY /Y "Debug-MW\NFS_XtendedInput.asi" "Debug-MW\Debug-MW-Pack\scripts"
:: Build the TPK -- YOU NEED TO HAVE XNFSTPKTool in your PATH!!!
echo Building TPK with XNFSTPKTool
xnfstpktool -w2 "XtendedInputButtons\8A69054A.ini" "Debug-MW\Debug-MW-Pack\Global\XtendedInputButtons.tpk"
:: Summon text files
echo Summoning text files
COPY /Y "Config-MW\NFS_XtendedInput.ini" "Debug-MW\Debug-MW-Pack\scripts"
COPY /Y "Config-MW\EventReference.txt" "Debug-MW\Debug-MW-Pack"
COPY /Y "README.md" "Debug-MW\Debug-MW-Pack"
:: Summon additional resources
echo Summoning additional resources
COPY /Y "Mouse Cursor\nfs_cursor.cur" "Debug-MW\Debug-MW-Pack\scripts"

echo Post build done!
