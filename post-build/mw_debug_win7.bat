@echo off
:: Prepare directories
echo Creating directories...
IF EXIST "Debug-MW-Win7\Debug-MW-Win7-Pack" RMDIR /S /Q "Debug-MW-Win7\Debug-MW-Win7-Pack"
MKDIR "Debug-MW-Win7\Debug-MW-Win7-Pack"
MKDIR "Debug-MW-Win7\Debug-MW-Win7-Pack\GLOBAL"
MKDIR "Debug-MW-Win7\Debug-MW-Win7-Pack\scripts"
:: Summon the binary
echo Copying the binary
COPY /Y "Debug-MW-Win7\NFS_XtendedInput.asi" "Debug-MW-Win7\Debug-MW-Win7-Pack\scripts"
:: Build the TPK -- YOU NEED TO HAVE XNFSTPKTool in your PATH!!!
echo Building TPK with XNFSTPKTool
xnfstpktool -w2 "XtendedInputButtons\8A69054A.ini" "Debug-MW-Win7\Debug-MW-Win7-Pack\GLOBAL\XtendedInputButtons.tpk"
:: Summon text files
echo Summoning text files
COPY /Y "Config-MW\NFS_XtendedInput.ini" "Debug-MW-Win7\Debug-MW-Win7-Pack\scripts"
COPY /Y "Config-MW\EventReference.txt" "Debug-MW-Win7\Debug-MW-Win7-Pack"
COPY /Y "README.md" "Debug-MW-Win7\Debug-MW-Win7-Pack"
:: Summon additional resources
echo Summoning additional resources
COPY /Y "Mouse Cursor\nfs_cursor.cur" "Debug-MW-Win7\Debug-MW-Win7-Pack\scripts"

echo Post build done!
