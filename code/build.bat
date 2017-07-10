@echo off

if not exist ..\build mkdir ..\build
pushd ..\build
cl /nologo /Zi /MDd /EHsc /I..\code\include -c ..\code\lib.c
popd