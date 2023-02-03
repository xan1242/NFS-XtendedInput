@echo off
:: Prepare directories
echo Creating directories...
IF EXIST "Debug-Carbon-Win7\Debug-Carbon-Win7-Pack" RMDIR /S /Q "Debug-Carbon-Win7\Debug-Carbon-Win7-Pack"
MKDIR "Debug-Carbon-Win7\Debug-Carbon-Win7-Pack"
MKDIR "Debug-Carbon-Win7\Debug-Carbon-Win7-Pack\GLOBAL"
MKDIR "Debug-Carbon-Win7\Debug-Carbon-Win7-Pack\scripts"
:: Summon the binary
echo Copying the binary
COPY /Y "Debug-Carbon-Win7\NFS_XtendedInput.asi" "Debug-Carbon-Win7\Debug-Carbon-Win7-Pack\scripts"
:: Generate the TPK ini
echo Generating the TPK ini
CALL "XtendedInputButtons\TpkIniGen.bat" "XtendedInputButtons" "XtendedInputButtons\8A69054A.ini"
:: Build the TPK -- YOU NEED TO HAVE XNFSTPKTool in your PATH!!!
echo Building TPK with XNFSTPKTool
xnfstpktool -w "XtendedInputButtons\8A69054A.ini" "Debug-Carbon-Win7\Debug-Carbon-Win7-Pack\GLOBAL\XtendedInputButtons.tpk"
:: Summon text files
echo Summoning text files
COPY /Y "Config-Carbon\NFS_XtendedInput.ini" "Debug-Carbon-Win7\Debug-Carbon-Win7-Pack\scripts"
COPY /Y "Config-Carbon\EventReference.txt" "Debug-Carbon-Win7\Debug-Carbon-Win7-Pack"
COPY /Y "README.md" "Debug-Carbon-Win7\Debug-Carbon-Win7-Pack"
:: Summon additional resources
echo Summoning additional resources
COPY /Y "Mouse Cursor\nfs_cursor.cur" "Debug-Carbon-Win7\Debug-Carbon-Win7-Pack\scripts"

echo Post build done!
