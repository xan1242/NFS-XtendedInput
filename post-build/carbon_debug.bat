@echo off
:: Make sure PATH has the tools in it
SET PATH=%PATH%;%~dp0..\XtendedInputButtons
:: Prepare directories
echo Creating directories...
IF EXIST "Debug-Carbon\Debug-Carbon-Pack" RMDIR /S /Q "Debug-Carbon\Debug-Carbon-Pack"
MKDIR "Debug-Carbon\Debug-Carbon-Pack"
MKDIR "Debug-Carbon\Debug-Carbon-Pack\GLOBAL"
MKDIR "Debug-Carbon\Debug-Carbon-Pack\scripts"
:: Summon the binary
echo Copying the binary
COPY /Y "Debug-Carbon\NFS_XtendedInput.asi" "Debug-Carbon\Debug-Carbon-Pack\scripts"
:: Generate the TPK ini
echo Generating the TPK ini
CALL "XtendedInputButtons\TpkIniGen.bat" "XtendedInputButtons" "XtendedInputButtons\8A69054A.ini"
:: Build the TPK -- YOU NEED TO HAVE XNFSTPKTool in your PATH!!!
echo Building TPK with XNFSTPKTool
xnfstpktool -w "XtendedInputButtons\8A69054A.ini" "Debug-Carbon\Debug-Carbon-Pack\GLOBAL\XtendedInputButtons.tpk"
:: Summon text files
echo Summoning text files
COPY /Y "Config-Carbon\NFS_XtendedInput.ini" "Debug-Carbon\Debug-Carbon-Pack\scripts"
COPY /Y "Config-Carbon\EventReference.txt" "Debug-Carbon\Debug-Carbon-Pack"
COPY /Y "README.md" "Debug-Carbon\Debug-Carbon-Pack"
:: Summon additional resources
echo Summoning additional resources
COPY /Y "Mouse Cursor\nfs_cursor.cur" "Debug-Carbon\Debug-Carbon-Pack\scripts"

echo Post build done!
