@echo off

REM
REM  To run this at startup, use this as your shortcut target:
REM  %windir%\system32\cmd.exe /k w:\handmade\misc\shell.bat
REM

rem call "C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\vcvarsall.bat" x64
call vs17.bat
w:
cd w:\
set path=w:\handmade\misc;w:\;%path%
set _NO_DEBUG_HEAP=1
