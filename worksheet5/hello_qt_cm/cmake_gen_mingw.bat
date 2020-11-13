@echo OFF
REM cmake_gen_mingw.bat ----------------------------------------------
REM Make sure this file is in the system path
REM Overwrite the path with minimal path to the correct compiler and Qt tools
REM This avoids problems if multiple (32 and 64bit) versions of MinGW have been installed
set PATH=C:\Users\milkw\Documents\Computer_Project\cmake-3.19.0-rc2-win64-x64\cmake-3.19.0-rc2-win64-x64\bin;C:\Qt\Tools\mingw810_64\bin;C:\Qt\5.15.1\mingw81_64\bin
REM Invoke cmake, telling it which version of Qt to use
cmake -G "MinGW Makefiles" %1
REM ------------------------------------------------------------------