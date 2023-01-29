@echo off
:: Prepare directories
echo Creating directories...
IF EXIST "Release-Carbon-Win7\Release-Carbon-Win7-Pack" RMDIR /S /Q "Release-Carbon-Win7\Release-Carbon-Win7-Pack"
MKDIR "Release-Carbon-Win7\Release-Carbon-Win7-Pack"
MKDIR "Release-Carbon-Win7\Release-Carbon-Win7-Pack\Global"
MKDIR "Release-Carbon-Win7\Release-Carbon-Win7-Pack\scripts"
:: Summon the binary
echo Copying the binary
COPY /Y "Release-Carbon-Win7\NFS_XtendedInput.asi" "Release-Carbon-Win7\Release-Carbon-Win7-Pack\scripts"
:: Build the TPK -- YOU NEED TO HAVE XNFSTPKTool in your PATH!!!
echo Building TPK with XNFSTPKTool
xnfstpktool -w "XtendedInputButtons\8A69054A.ini" "Release-Carbon-Win7\Release-Carbon-Win7-Pack\Global\XtendedInputButtons.tpk"
:: Summon text files
echo Summoning text files
COPY /Y "Config-Carbon\NFS_XtendedInput.ini" "Release-Carbon-Win7\Release-Carbon-Win7-Pack\scripts"
COPY /Y "Config-Carbon\EventReference.txt" "Release-Carbon-Win7\Release-Carbon-Win7-Pack"
COPY /Y "README.md" "Release-Carbon-Win7\Release-Carbon-Win7-Pack"
:: Summon additional resources
echo Summoning additional resources
COPY /Y "Mouse Cursor\nfs_cursor.cur" "Release-Carbon-Win7\Release-Carbon-Win7-Pack\scripts"

echo Post build done!
