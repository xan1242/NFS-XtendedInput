@echo off
for /R %%f in (*.png) do (
	texconv -px PS3_ -nologo -m 3 -w 128 -h 128 -if TRIANGLE -mf TRIANGLE -f DXT5 -o ../ "%%f"
 )
pause