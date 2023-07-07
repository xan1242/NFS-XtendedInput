@echo off
:: Make sure PATH has the tools in it
SET PATH=%PATH%;%~dp0../XtendedInputButtons
SET BuildType=%1
SET GameName=%2
:: Prepare directories
echo Creating directories...
cd %~dp0../..
IF EXIST ".Release Packages\%BuildType%-%GameName%" RMDIR /S /Q ".Release Packages\%BuildType%-%GameName%"
MKDIR ".Release Packages\%BuildType%-%GameName%\GLOBAL"
MKDIR ".Release Packages\%BuildType%-%GameName%\scripts"
:: Summon the binary
echo Copying the binary
COPY /Y "src\%BuildType%-%GameName%\NFS_XtendedInput.asi" ".Release Packages\%BuildType%-%GameName%\scripts"
:: Generate the TPK ini
echo Generating the TPK ini
CALL "data\XtendedInputButtons\TpkIniGen.bat" "data\XtendedInputButtons" "data\XtendedInputButtons\8A69054A.ini"
:: Build the TPK -- YOU NEED TO HAVE XNFSTPKTool in your PATH!!!
echo Building TPK with XNFSTPKTool
xnfstpktool -w2 "data\XtendedInputButtons\8A69054A.ini" ".Release Packages\%BuildType%-%GameName%\GLOBAL\XtendedInputButtons.tpk"
:: Summon text files
echo Summoning text files
COPY /Y "data\Config-%GameName%\NFS_XtendedInput.ini" ".Release Packages\%BuildType%-%GameName%\scripts"
COPY /Y "data\Config-%GameName%\NFS_XtendedInput.default.ini" ".Release Packages\%BuildType%-%GameName%\scripts"
COPY /Y "data\Config-%GameName%\EventReference.txt" ".Release Packages\%BuildType%-%GameName%"
COPY /Y "README.md" ".Release Packages\%BuildType%-%GameName%"
:: Summon additional resources
echo Summoning additional resources
COPY /Y "data\Mouse Cursor\nfs_cursor.cur" ".Release Packages\%BuildType%-%GameName%\scripts"

echo Post build done!
