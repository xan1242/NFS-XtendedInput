@echo off
:: Make sure PATH has the tools in it
SET PATH=%PATH%;%~dp0..\XtendedInputButtons
:: Prepare directories
echo Creating directories...
IF EXIST "Release-Carbon\Release-Carbon-Pack" RMDIR /S /Q "Release-Carbon\Release-Carbon-Pack"
MKDIR "Release-Carbon\Release-Carbon-Pack"
MKDIR "Release-Carbon\Release-Carbon-Pack\GLOBAL"
MKDIR "Release-Carbon\Release-Carbon-Pack\scripts"
:: Summon the binary
echo Copying the binary
COPY /Y "Release-Carbon\NFS_XtendedInput.asi" "Release-Carbon\Release-Carbon-Pack\scripts"
:: Generate the TPK ini
echo Generating the TPK ini
CALL "XtendedInputButtons\TpkIniGen.bat" "XtendedInputButtons" "XtendedInputButtons\8A69054A.ini"
:: Build the TPK -- YOU NEED TO HAVE XNFSTPKTool in your PATH!!!
echo Building TPK with XNFSTPKTool
xnfstpktool -w "XtendedInputButtons\8A69054A.ini" "Release-Carbon\Release-Carbon-Pack\GLOBAL\XtendedInputButtons.tpk"
:: Summon text files
echo Summoning text files
COPY /Y "Config-Carbon\NFS_XtendedInput.ini" "Release-Carbon\Release-Carbon-Pack\scripts"
COPY /Y "Config-Carbon\EventReference.txt" "Release-Carbon\Release-Carbon-Pack"
COPY /Y "README.md" "Release-Carbon\Release-Carbon-Pack"
:: Summon additional resources
echo Summoning additional resources
COPY /Y "Mouse Cursor\nfs_cursor.cur" "Release-Carbon\Release-Carbon-Pack\scripts"

echo Post build done!
