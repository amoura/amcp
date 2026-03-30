@echo off

pushd ..\build
cl /std:c++latest /nologo /Zi /WX /D_HAS_EXCEPTIONS=0 /GR- /fsanitize=address   /Fe:tst.exe ..\src\main.cpp
popd
