@echo off
:: Prepare directories
echo Creating directories...
IF EXIST "Release-ProStreet-Win7\Release-ProStreet-Win7-Pack" RMDIR /S /Q "Release-ProStreet-Win7\Release-ProStreet-Win7-Pack"
MKDIR "Release-ProStreet-Win7\Release-ProStreet-Win7-Pack"
MKDIR "Release-ProStreet-Win7\Release-ProStreet-Win7-Pack\GLOBAL"
MKDIR "Release-ProStreet-Win7\Release-ProStreet-Win7-Pack\scripts"
:: Summon the binary
echo Copying the binary
COPY /Y "Release-ProStreet-Win7\NFS_XtendedInput.asi" "Release-ProStreet-Win7\Release-ProStreet-Win7-Pack\scripts"
:: Generate the TPK ini
echo Generating the TPK ini
CALL "XtendedInputButtons\TpkIniGen.bat" "XtendedInputButtons" "XtendedInputButtons\8A69054A.ini"
:: Build the TPK -- YOU NEED TO HAVE XNFSTPKTool in your PATH!!!
echo Building TPK with XNFSTPKTool
xnfstpktool -w "XtendedInputButtons\8A69054A.ini" "Release-ProStreet-Win7\Release-ProStreet-Win7-Pack\GLOBAL\XtendedInputButtons.tpk"
:: Summon text files
echo Summoning text files
COPY /Y "Config-ProStreet\NFS_XtendedInput.ini" "Release-ProStreet-Win7\Release-ProStreet-Win7-Pack\scripts"
COPY /Y "Config-ProStreet\EventReference.txt" "Release-ProStreet-Win7\Release-ProStreet-Win7-Pack"
COPY /Y "README.md" "Release-ProStreet-Win7\Release-ProStreet-Win7-Pack"
:: Summon additional resources
echo Summoning additional resources
COPY /Y "Mouse Cursor\nfs_cursor.cur" "Release-ProStreet-Win7\Release-ProStreet-Win7-Pack\scripts"

echo Post build done!
