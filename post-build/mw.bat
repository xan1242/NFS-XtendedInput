@echo off
:: Prepare directories
echo Creating directories...
IF EXIST "Release-MW\Release-MW-Pack" RMDIR /S /Q "Release-MW\Release-MW-Pack"
MKDIR "Release-MW\Release-MW-Pack"
MKDIR "Release-MW\Release-MW-Pack\Global"
MKDIR "Release-MW\Release-MW-Pack\scripts"
:: Build the TPK -- YOU NEED TO HAVE XNFSTPKTool in your PATH!!!
echo Building TPK with XNFSTPKTool
xnfstpktool -w2 "XtendedInputButtons\8A69054A.ini" "Release-MW\Release-MW-Pack\Global\XtendedInputButtons.tpk"
:: Summon text files
echo Summoning text files
COPY /Y "Config-MW\NFS_XtendedInput.ini" "Release-MW\Release-MW-Pack\scripts"
COPY /Y "Config-MW\EventReference.txt" "Release-MW\Release-MW-Pack"
COPY /Y "README.md" "Release-MW\Release-MW-Pack"
:: Summon additional resources
echo Summoning additional resources
COPY /Y "Mouse Cursor\nfs_cursor.cur" "Release-MW\Release-MW-Pack\scripts"

echo Post build done!
