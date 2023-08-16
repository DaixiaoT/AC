call core_path.bat

del /s/q %COER_PATH%\*.o
del /s/q %COER_PATH%\..\comm\*.o
del /s/q *.o *.elf
