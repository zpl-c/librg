@echo off
if not exist ..\data mkdir ..\data
pushd ..\data
..\build\test.exe
popd
