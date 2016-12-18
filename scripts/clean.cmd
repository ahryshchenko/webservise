@echo off

rd /q /s ..\build

:: Prevent CI from stopping the build.
set errorlevel=0
