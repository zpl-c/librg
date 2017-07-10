@echo off


pushd
rem call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" x64 1> NUL
call "D:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvarsall.bat" x64 1> NUL
popd

set _NO_DEBUG_HEAP=1

set path=w:\odin\misc;%path%

cls
