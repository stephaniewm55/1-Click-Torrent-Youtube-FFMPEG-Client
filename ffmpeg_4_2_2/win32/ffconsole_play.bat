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

TITLE Torrent Youtube Downloader for FFMPEG v1.0.0.0 32-bit - ffplay.exe "%Name1%"

set PATH=%Folder%;%SYSTEMROOT%\SysWOW64;%SYSTEMROOT%\System32

set FFREPORT=file=%Name1%.play.mp4.log:level=32
rem echo %FFREPORT%

ffplay.exe "%Name1%"

IF %ERRORLEVEL% EQU 0 goto SUCCESS_LABEL

echo See "%Name1%.play.mp4.log" for details.
pause

goto END_LABEL

:SUCCESS_LABEL

del /Q /F "%Name1%.play.mp4.log"

:END_LABEL
