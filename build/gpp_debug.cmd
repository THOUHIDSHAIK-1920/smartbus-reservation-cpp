@echo off
cd /d C:\Users\shaik\Documents\bus_reservation_cpp_dashboard\build
echo RUNNING > debug_out.txt
echo CWD=%CD% >> debug_out.txt
echo g++ exists=%~$PATH: C:\Qt\Tools\mingw1310_64\bin\g++.exe >> debug_out.txt
echo --- >> debug_out.txt
echo #error test > tmp_test.cpp
"C:\Qt\Tools\mingw1310_64\bin\g++.exe" -fsyntax-only tmp_test.cpp > stdout.txt 2> stderr.txt
echo RC=%ERRORLEVEL% >> debug_out.txt
echo --- STDOUT --- >> debug_out.txt
type stdout.txt >> debug_out.txt
echo --- STDERR --- >> debug_out.txt
type stderr.txt >> debug_out.txt
echo --- END --- >> debug_out.txt
del tmp_test.cpp >nul 2>nul
del stdout.txt >nul 2>nul
del stderr.txt >nul 2>nul
