
@echo off
:Emulator
Set MyProcess=openmsx.exe
tasklist | find /i "%MyProcess%">nul  && (echo %MyProcess% Already running) || start openmsx.exe -script openMSX\emul_start_config.txt
:_end_