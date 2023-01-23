echo off
setlocal enabledelayedexpansion

:: 指定起始文件夹
set DIR="%cd%"
:: 指定clang-format.exe路径
SET CLANG_FORMAT="G:\Program Files\LLVM\bin\clang-format"

:: 参数 /R 表示需要遍历子文件夹,去掉表示不遍历子文件夹
:: %%f 是一个变量,类似于迭代器,但是这个变量只能由一个字母组成,前面带上%%
:: 括号中是通配符,可以指定后缀名,*.*表示所有文件
SET /a index=0
for /R %DIR% %%f in (*.h, *.c, *.hpp, *.cpp, *.cc) do (
    %CLANG_FORMAT% -style=File -i %%f
    set /a index+=1
    Set "NUM=0000!index!"&Set "NUM=!NUM:~-4!"
    echo [!NUM!]Format file: %%f
)
echo.
echo Total format !index! files.
echo.
pause