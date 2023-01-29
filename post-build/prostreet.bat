@echo off
:: Prepare directories
echo Creating directories...
IF EXIST "Release-ProStreet\Release-ProStreet-Pack" RMDIR /S /Q "Release-ProStreet\Release-ProStreet-Pack"
MKDIR "Release-ProStreet\Release-ProStreet-Pack"
MKDIR "Release-ProStreet\Release-ProStreet-Pack\GLOBAL"
MKDIR "Release-ProStreet\Release-ProStreet-Pack\scripts"
:: Summon the binary
echo Copying the binary
COPY /Y "Release-ProStreet\NFS_XtendedInput.asi" "Release-ProStreet\Release-ProStreet-Pack\scripts"
:: Build the TPK -- YOU NEED TO HAVE XNFSTPKTool in your PATH!!!
echo Building TPK with XNFSTPKTool
xnfstpktool -w "XtendedInputButtons\8A69054A.ini" "Release-ProStreet\Release-ProStreet-Pack\GLOBAL\XtendedInputButtons.tpk"
:: Summon text files
echo Summoning text files
COPY /Y "Config-ProStreet\NFS_XtendedInput.ini" "Release-ProStreet\Release-ProStreet-Pack\scripts"
COPY /Y "Config-ProStreet\EventReference.txt" "Release-ProStreet\Release-ProStreet-Pack"
COPY /Y "README.md" "Release-ProStreet\Release-ProStreet-Pack"
:: Summon additional resources
echo Summoning additional resources
COPY /Y "Mouse Cursor\nfs_cursor.cur" "Release-ProStreet\Release-ProStreet-Pack\scripts"

echo Post build done!
