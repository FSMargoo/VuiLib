echo off
cls
setlocal enabledelayedexpansion

set DIR="%cd%"
SET CLANG_FORMAT="Z:\seal-tester\mingw64\bin\clang-format"

SET /a index=0
for /R %DIR% %%f in (*.h, *.c, *.hpp, *.cpp, *.cc) do (
    %CLANG_FORMAT% -style=File -i %%f
    set /a index+=1
    Set "NUM=0000!index!"&Set "NUM=!NUM:~-4!"
    echo [!NUM!]Format file: %%f
)
echo.