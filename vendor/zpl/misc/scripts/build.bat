@echo off

set CommonCompilerFlags=-Od -MTd -nologo -fp:fast -fp:except- -Gm- -GR- -EHa- -Zo -Oi -W4 -wd4201 -wd4100 -wd4189 -wd4505 -wd4204 -wd4067 -wd4057 -wd4244 -wd4456 -wd4706 -wd4127 -FC -Z7
set CommonLinkerFlags= -incremental:no -opt:ref user32.lib gdi32.lib winmm.lib opengl32.lib

IF "%FRAGWORK_STREAMING%"=="" goto nostreaming
set CommonCompilerFlags=%CommonCompilerFlags% -DFRAGWORK_STREAMING=1
:nostreaming

REM TODO - can we just build both with one exe?

IF NOT EXIST ..\build mkdir ..\build
pushd ..\build

del *.pdb > NUL 2> NUL

cl %CommonCompilerFlags% ..\code\test.c /link %CommonLinkerFlags%

popd