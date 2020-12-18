@echo off

rem echo %~F0
rem echo %~F1

Set filename=%~F0
For %%A in ("%filename%") do (
    Set Folder=%%~dpA
    Set Name=%%~nxA
)

Set filename1=%~F1
For %%B in ("%filename1%") do (
    Set Folder1=%%~dpB
    Set Name1=%%~nxB
)

rem echo "%Folder1%"
rem echo "%Name1%"
rem pause

cd "%Folder1%"

TITLE Torrent Youtube Downloader for FFMPEG v1.0.0.0 32-bit - Increase volume without clipping

set PATH=%Folder%;%SYSTEMROOT%\SysWOW64;%SYSTEMROOT%\System32

set FFREPORT=file=%Name1%.increase_volume_no_clipping.mp4.log:level=32
rem echo %FFREPORT%

ffmpeg.exe -i "%Name1%" -af "dynaudnorm,compand=attacks=0.3 0.3:decays=0.8 0.8:points=-90/-900 -70/-50 -20/-12 -10/-5 0/0 100/0:soft-knee=0.01:gain=0:volume=-90:delay=0.8" -c:v copy -c:a aac -y "%Name1%.increase_volume_no_clipping.mp4"

IF %ERRORLEVEL% EQU 0 goto SUCCESS_LABEL

echo See "%Name1%.increase_volume_no_clipping.mp4.log" for details.
pause

goto END_LABEL

:SUCCESS_LABEL

del /Q /F "%Name1%.increase_volume_no_clipping.mp4.log"

:END_LABEL
