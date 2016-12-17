@echo off

mkdir ..\build\vc14-x64
pushd ..\build\vc14-x64

cmake ^
	-G"Visual Studio 14 Win64" -T "v140_xp" ^
	"-DBUILD_WITH_STATIC_CRT:BOOL=ON" ^
	"-Wno-dev" ^
	../..
if errorlevel 1 goto :error

echo [*] Project generation succeeded!

popd
goto :eof

:error
echo [!] ERROR: Failed to generate project - see above and correct.
popd
exit /b 1
