import subprocess
import os
import sys

print('CWD:', os.getcwd())
print('Exists g++:', os.path.exists(r'C:\Qt\Tools\mingw1310_64\bin\g++.exe'))
print('Exists cpp file:', os.path.exists('tmp_test.cpp'))
print('Files in CWD:', os.listdir('.'))

cmd = [r'C:\Qt\Tools\mingw1310_64\bin\g++.exe', '-fsyntax-only', 'tmp_test.cpp']
print('CMD:', cmd)
proc = subprocess.run(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
print('RETURN CODE:', proc.returncode)
print('STDOUT:', repr(proc.stdout))
print('STDERR:', repr(proc.stderr))
print('END')
