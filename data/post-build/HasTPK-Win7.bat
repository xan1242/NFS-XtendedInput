@echo off
:: Make sure PATH has the tools in it
SET PATH=%PATH%;%~dp0../XtendedInputButtons
SET BuildType=%1
SET GameName=%2
:: Prepare directories
echo Creating directories...
cd %~dp0../..
IF EXIST ".Release Packages\%BuildType%-%GameName%-Win7" RMDIR /S /Q ".Release Packages\%BuildType%-%GameName%-Win7"
MKDIR ".Release Packages\%BuildType%-%GameName%-Win7\GLOBAL"
MKDIR ".Release Packages\%BuildType%-%GameName%-Win7\scripts"
:: Summon the binary
echo Copying the binary
COPY /Y "src\%BuildType%-%GameName%-Win7\NFS_XtendedInput.asi" ".Release Packages\%BuildType%-%GameName%-Win7\scripts"
:: Generate the TPK ini
echo Generating the TPK ini
CALL "data\XtendedInputButtons\TpkIniGen.bat" "data\XtendedInputButtons" "data\XtendedInputButtons\8A69054A.ini"
:: Build the TPK -- YOU NEED TO HAVE XNFSTPKTool in your PATH!!!
echo Building TPK with XNFSTPKTool
xnfstpktool -w "data\XtendedInputButtons\8A69054A.ini" ".Release Packages\%BuildType%-%GameName%-Win7\GLOBAL\XtendedInputButtons.tpk"
:: Summon text files
echo Summoning text files
COPY /Y "data\Config-%GameName%\NFS_XtendedInput.ini" ".Release Packages\%BuildType%-%GameName%-Win7\scripts"
COPY /Y "data\Config-%GameName%\NFS_XtendedInput.default.ini" ".Release Packages\%BuildType%-%GameName%-Win7\scripts"
COPY /Y "data\Config-%GameName%\EventReference.txt" ".Release Packages\%BuildType%-%GameName%-Win7"
COPY /Y "README.md" ".Release Packages\%BuildType%-%GameName%-Win7"
:: Summon additional resources
echo Summoning additional resources
COPY /Y "data\Mouse Cursor\nfs_cursor.cur" ".Release Packages\%BuildType%-%GameName%-Win7\scripts"

echo Post build done!
