@echo off
:: Prepare directories
echo Creating directories...
IF EXIST "Release-MW-Win7\Release-MW-Win7-Pack" RMDIR /S /Q "Release-MW-Win7\Release-MW-Win7-Pack"
MKDIR "Release-MW-Win7\Release-MW-Win7-Pack"
MKDIR "Release-MW-Win7\Release-MW-Win7-Pack\GLOBAL"
MKDIR "Release-MW-Win7\Release-MW-Win7-Pack\scripts"
:: Summon the binary
echo Copying the binary
COPY /Y "Release-MW-Win7\NFS_XtendedInput.asi" "Release-MW-Win7\Release-MW-Win7-Pack\scripts"
:: Generate the TPK ini
echo Generating the TPK ini
CALL "XtendedInputButtons\TpkIniGen.bat" "XtendedInputButtons" "XtendedInputButtons\8A69054A.ini"
:: Build the TPK -- YOU NEED TO HAVE XNFSTPKTool in your PATH!!!
echo Building TPK with XNFSTPKTool
xnfstpktool -w2 "XtendedInputButtons\8A69054A.ini" "Release-MW-Win7\Release-MW-Win7-Pack\GLOBAL\XtendedInputButtons.tpk"
:: Summon text files
echo Summoning text files
COPY /Y "Config-MW\NFS_XtendedInput.ini" "Release-MW-Win7\Release-MW-Win7-Pack\scripts"
COPY /Y "Config-MW\EventReference.txt" "Release-MW-Win7\Release-MW-Win7-Pack"
COPY /Y "README.md" "Release-MW-Win7\Release-MW-Win7-Pack"
:: Summon additional resources
echo Summoning additional resources
COPY /Y "Mouse Cursor\nfs_cursor.cur" "Release-MW-Win7\Release-MW-Win7-Pack\scripts"

echo Post build done!
