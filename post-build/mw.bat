@echo off
:: Prepare directories
echo Creating directories...
IF EXIST "Release-MW\Release-MW-Pack" RMDIR /S /Q "Release-MW\Release-MW-Pack"
MKDIR "Release-MW\Release-MW-Pack"
MKDIR "Release-MW\Release-MW-Pack\GLOBAL"
MKDIR "Release-MW\Release-MW-Pack\scripts"
:: Summon the binary
echo Copying the binary
COPY /Y "Release-MW\NFS_XtendedInput.asi" "Release-MW\Release-MW-Pack\scripts"
:: Generate the TPK ini
echo Generating the TPK ini
CALL "XtendedInputButtons\TpkIniGen.bat" "XtendedInputButtons" "XtendedInputButtons\8A69054A.ini"
:: Build the TPK -- YOU NEED TO HAVE XNFSTPKTool in your PATH!!!
echo Building TPK with XNFSTPKTool
xnfstpktool -w2 "XtendedInputButtons\8A69054A.ini" "Release-MW\Release-MW-Pack\GLOBAL\XtendedInputButtons.tpk"
:: Summon text files
echo Summoning text files
COPY /Y "Config-MW\NFS_XtendedInput.ini" "Release-MW\Release-MW-Pack\scripts"
COPY /Y "Config-MW\EventReference.txt" "Release-MW\Release-MW-Pack"
COPY /Y "README.md" "Release-MW\Release-MW-Pack"
:: Summon additional resources
echo Summoning additional resources
COPY /Y "Mouse Cursor\nfs_cursor.cur" "Release-MW\Release-MW-Pack\scripts"

echo Post build done!
