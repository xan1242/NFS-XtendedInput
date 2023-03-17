
:: Make sure PATH has the tools in it
SET PATH=%PATH%;%~dp0../XtendedInputButtons
:: Prepare directories
echo Creating directories...
cd %~dp0../..
IF EXIST ".Release Packages" RMDIR /S /Q ".Release Packages"
MKDIR ".Release Packages\Release-ProStreet\GLOBAL"
MKDIR ".Release Packages\Release-ProStreet\scripts"
:: Summon the binary
echo Copying the binary
COPY /Y "src\Release-ProStreet\NFS_XtendedInput.asi" ".Release Packages\Release-ProStreet\scripts"
:: Generate the TPK ini
echo Generating the TPK ini
CALL "data\XtendedInputButtons\TpkIniGen.bat" "data\XtendedInputButtons" "data\XtendedInputButtons\8A69054A.ini"
:: Build the TPK -- YOU NEED TO HAVE XNFSTPKTool in your PATH!!!
echo Building TPK with XNFSTPKTool
xnfstpktool -w "data\XtendedInputButtons\8A69054A.ini" ".Release Packages\Release-ProStreet\GLOBAL\XtendedInputButtons.tpk"
:: Summon text files
echo Summoning text files
COPY /Y "data\Config-ProStreet\NFS_XtendedInput.ini" ".Release Packages\Release-ProStreet\scripts"
COPY /Y "data\Config-ProStreet\EventReference.txt" ".Release Packages\Release-ProStreet"
COPY /Y "README.md" ".Release Packages\Release-ProStreet"
:: Summon additional resources
echo Summoning additional resources
COPY /Y "data\Mouse Cursor\nfs_cursor.cur" ".Release Packages\Release-ProStreet\scripts"

echo Post build done!
