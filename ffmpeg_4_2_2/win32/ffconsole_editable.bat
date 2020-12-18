@if (@CodeSection == @Batch) @then

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

TITLE Torrent Youtube Downloader for FFMPEG v1.0.0.0 32-bit - console editable with file pre-typed: '%Name1%'

set PATH=%Folder%;%SYSTEMROOT%\SysWOW64;%SYSTEMROOT%\System32

set FFREPORT=file=%Name1%.console.mp4.log:level=32
rem echo %FFREPORT%

rem Use %SendKeys% to send keys to the keyboard buffer
set SendKeys=CScript /nologo /E:JScript "%~F0"
rem set Name1=%Name1: =" "%
set FFCOMMAND=ffmpeg.exe -i "%Name1%" -vf "format=pix_fmts=yuv420p" -c:v libx264 -c:a aac -y "%Name1%.console.mp4"

echo %FFCOMMAND% > tmp.txt

rem Start the other program in the same Window
start "" /B cmd

%SendKeys% tmp.txt

del /F /Q tmp.txt

ping -n 5 -w 1 127.0.0.1 > NUL

goto :EOF

@end

// JScript section

// define constants
// Note: if a file exists, using forWriting will set
// the contents of the file to zero before writing to
// it.
var forReading = 1, forWriting = 2, forAppending = 8;

// define array to store lines.
rline = new Array();

// Create the object
fs = new ActiveXObject("Scripting.FileSystemObject");
f = fs.GetFile(WScript.Arguments(0));

// Open the file
is = f.OpenAsTextStream( forReading, 0 );

// start and continue to read until we hit
// the end of the file.
var count = 0;
while( !is.AtEndOfStream ){
   rline[count] = is.ReadLine();
   count++;
}

// Close the stream
is.Close();

var WshShell = WScript.CreateObject("WScript.Shell");
WshShell.SendKeys(rline[0]);
