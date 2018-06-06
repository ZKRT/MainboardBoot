@echo off
md ..\Release

echo turn executable files from axf to bin by tool names fromelf in filefordler of D:\Keil_v5\ARM\ARMCC\bin\ .
D:\Keil_v5\ARM\ARMCC\bin\fromelf.exe --bin -o ..\Project\Objects\Mboot.bin ..\Project\Objects\Mboot.axf

echo copy release files to release folder.
copy ..\Project\Objects\Mboot.bin ..\Release\Mboot-V01-00-18xxxx.bin
copy ..\Project\Objects\Mboot.hex ..\Release\Mboot-V01-00-18xxxx.hex
