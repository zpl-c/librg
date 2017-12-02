@echo off

REM Execute inside root dir!
REM Make sure you also install SDL2.

if not exist build mkdir build

pushd build
cmake -G "Visual Studio 15 2017 Win64" -DLIBRG_DEMO=1 -DLIBRG_CLI_TEST=1 -DVIDEO_OPENGLES=1 -DLIBRG_STATIC=1 ..
popd
