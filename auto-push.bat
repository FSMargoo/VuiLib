@echo off
setlocal enabledelayedexpansion
echo Before code push, format the code files
set DIR="%cd%"
SET CLANG_FORMAT="G:\Program Files\LLVM\bin\clang-format"

SET /a index=0
for /R %DIR% %%f in (*.h, *.c, *.hpp, *.cpp, *.cc) do (
    %CLANG_FORMAT% -style=File -i %%f
    set /a index+=1
    Set "NUM=0000!index!"&Set "NUM=!NUM:~-4!"
    echo [!NUM!]Format file: %%f
)

echo Total format !index! files.

git add *
set /p commit_info="Git commit info: "
git commit -m "%commit_info%"
:pushlabel
git push vlib main
pause
set /p repeat="Dose the push finished? Try again? (y/n)"
if %repeat% == "y" (goto pushlabel)