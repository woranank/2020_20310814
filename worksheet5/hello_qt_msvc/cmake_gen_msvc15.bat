@echo OFF
REM cmake_gen_msvc15.bat ----------------------------------------------
REM Make sure this file is in the system path
REM Overwrite the path with path to the correct compiler and Qt tools
REM Remember the old path so can change back if needed.
set OLD_PATH=%PATH%
set PATH=C:\Users\milkw\Documents\Computer_Project\cmake-3.19.0-rc2-win64-x64\cmake-3.19.0-rc2-win64-x64\bin;C:\Qt\5.15.1\msvc2019_64\bin
REM New vcvarsall.bat will cd to C:\users\exxxx\source unless tell it to user current directory
set VSCMD_START_DIR=%CD%
REM Invoke cmake, telling it where the correct version of Qt is located
cmake -DCMAKE_PREFIX_PATH=C:\Qt\5.15.1\msvc2019_64\bin -G "Visual Studio 16 2019" %1
REM Restore old path (otherwise vcvarsall will fail) but make sure Qt bin dir has been added to it
set PATH=%OLD_PATH%;C:\Qt\5.15.1\msvc2019_64\bin
REM Setup Compiler Path
"C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" amd64
REM ------------------------------------------------------------------