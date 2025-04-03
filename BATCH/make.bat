
@echo off
:Emulator
Set MyProcess=openmsx.exe
tasklist | find /i "%MyProcess%">nul  && (echo %MyProcess% Already running) || start openmsx.exe -script openMSX\1-emul_start_MSX1_config.txt
:_end_