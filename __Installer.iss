[Setup]
AppName=1 Click Torrent Youtube FFMPEG Client
AppVersion=1.0.0.0
VersionInfoVersion=1.0.0.0
AppPublisher=Open Source Developer Stephanie W. Myers
AppCopyright=2016-2020 Open Source Developer Stephanie W. Myers
PrivilegesRequired=lowest
DefaultDirName={userappdata}\1ClickTorrentYoutubeFFMPEGClient
LicenseFile=_license.txt
DefaultGroupName=1 Click Torrent Youtube FFMPEG Client
UninstallDisplayIcon={app}\1ClickTorrentYoutubeFFMPEGClient\favicon_new.ico
SolidCompression=yes
Compression=lzma2/ultra64
;LZMAUseSeparateProcess=yes
;LZMADictionarySize=1048576
;LZMANumFastBytes=273
OutputDir=.\bin
AlwaysShowDirOnReadyPage=yes
AlwaysShowGroupOnReadyPage=yes
WizardImageFile=_wizardimage.bmp
WizardSmallImageFile=_wizardimagesmall.bmp
#ifnexist "_DEBUG"
OutputBaseFilename=Install_1ClickTorrentYoutubeFFMPEGClient_1.0.0.0
#else
OutputBaseFilename=Install_1ClickTorrentYoutubeFFMPEGClient_1.0.0.0d
#endif
CloseApplications=force
; Historically we have to preserve mutex name for compatibility
SetupMutex=Setup_1ClickTorrentYoutubeFFMPEGClient
DirExistsWarning=no
ExtraDiskSpaceRequired=10485760
Encryption=no


[Dirs]
; Note it only removes dir if it is empty after automatic file uninstalling done
Name: "{app}"; Flags: uninsalwaysuninstall;


[Files]
Source: "ffmpeg_4_2_2\win32\*"; DestDir: "{app}\1ClickTorrentYoutubeFFMPEGClient\win32"; Flags: ignoreversion recursesubdirs;
Source: "bin\Release\Win32\OneClickTorrentDownload.exe"; DestDir: "{app}\1ClickTorrentYoutubeFFMPEGClient\win32"; Flags: ignoreversion;
Source: "Resources\youtube-dl.exe"; DestDir: "{app}\1ClickTorrentYoutubeFFMPEGClient\win32"; Flags: ignoreversion;
Source: "Resources\youtube_url_example.txt"; DestDir: "{app}\1ClickTorrentYoutubeFFMPEGClient\sample_videos"; Flags: ignoreversion;
Source: "Resources\input_h265.mkv"; DestDir: "{app}\1ClickTorrentYoutubeFFMPEGClient\sample_videos"; Flags: ignoreversion;
Source: "Resources\night_of_the_living_dead_archive.torrent"; DestDir: "{app}\1ClickTorrentYoutubeFFMPEGClient\sample_videos"; Flags: ignoreversion;
Source: "Resources\favicon_new.ico"; DestDir: "{app}\1ClickTorrentYoutubeFFMPEGClient"; Flags: ignoreversion;
Source: "Resources\audio.ico"; DestDir: "{app}\1ClickTorrentYoutubeFFMPEGClient"; Flags: ignoreversion;
Source: "Resources\encode.ico"; DestDir: "{app}\1ClickTorrentYoutubeFFMPEGClient"; Flags: ignoreversion;
Source: "Resources\transcode.ico"; DestDir: "{app}\1ClickTorrentYoutubeFFMPEGClient"; Flags: ignoreversion;


[Icons]
Name: "{userdesktop}\1 Click Torrent Youtube FFMPEG Client"; Filename: "{app}\1ClickTorrentYoutubeFFMPEGClient\sample_videos"; WorkingDir: "{app}\1ClickTorrentYoutubeFFMPEGClient\sample_videos"
Name: "{group}\1 Click Torrent Youtube FFMPEG Client sample videos"; Filename: "{app}\1ClickTorrentYoutubeFFMPEGClient\sample_videos"; WorkingDir: "{app}\1ClickTorrentYoutubeFFMPEGClient\sample_videos"


[Registry]
;magnet
Root: HKCU; Subkey: "Software\Classes\magnet"; ValueType: string; ValueName: ""; ValueData: "URL:MAGNET URI"; Flags: uninsdeletekey
Root: HKCU; Subkey: "Software\Classes\magnet"; ValueType: string; ValueName: "URL Protocol"; ValueData: "";
Root: HKCU; Subkey: "Software\Classes\magnet\DefaultIcon"; ValueType: string; ValueName: ""; ValueData: """{app}\1ClickTorrentYoutubeFFMPEGClient\win32\OneClickTorrentDownload.exe""";
Root: HKCU; Subkey: "Software\Classes\magnet\shell"; ValueType: string; ValueName: ""; ValueData: "";
Root: HKCU; Subkey: "Software\Classes\magnet\shell\open"; ValueType: string; ValueName: ""; ValueData: "";
Root: HKCU; Subkey: "Software\Classes\magnet\shell\open\command"; ValueType: string; ValueName: ""; ValueData: "cmd.exe /C """"{app}\1ClickTorrentYoutubeFFMPEGClient\win32\OneClickTorrentDownload.exe"" ""%1"" play""";
;1ClickTorrentYoutubeFFMPEGClient
Root: HKCU; Subkey: "Software\Classes\*\shell\1ClickTorrentYoutubeFFMPEGClient"; ValueType: string; ValueName: "MUIVerb"; ValueData: "1 Click Torrent Youtube FFMPEG Client"; Flags: uninsdeletekey
Root: HKCU; Subkey: "Software\Classes\*\shell\1ClickTorrentYoutubeFFMPEGClient"; ValueType: string; ValueName: "SubCommands"; ValueData: "";
Root: HKCU; Subkey: "Software\Classes\*\shell\1ClickTorrentYoutubeFFMPEGClient"; ValueType: expandsz; ValueName: "Icon"; ValueData: "{app}\1ClickTorrentYoutubeFFMPEGClient\favicon_new.ico";
Root: HKCU; Subkey: "Software\Classes\*\shell\1ClickTorrentYoutubeFFMPEGClient\shell\Play"; ValueType: string; ValueName: "MUIVerb"; ValueData: "Play video/audio";
Root: HKCU; Subkey: "Software\Classes\*\shell\1ClickTorrentYoutubeFFMPEGClient\shell\Play\command"; ValueType: string; ValueName: ""; ValueData: "cmd.exe /C """"{app}\1ClickTorrentYoutubeFFMPEGClient\win32\ffconsole_play.bat"" ""%1""""";
Root: HKCU; Subkey: "Software\Classes\*\shell\1ClickTorrentYoutubeFFMPEGClient\shell\Console_32"; ValueType: string; ValueName: "MUIVerb"; ValueData: "FFMPEG v4.2.2 32-bit console editable with this file pre-typed";
Root: HKCU; Subkey: "Software\Classes\*\shell\1ClickTorrentYoutubeFFMPEGClient\shell\Console_32\command"; ValueType: string; ValueName: ""; ValueData: "cmd.exe /C """"{app}\1ClickTorrentYoutubeFFMPEGClient\win32\ffconsole_editable.bat"" ""%1""""";
Root: HKCU; Subkey: "Software\Classes\*\shell\1ClickTorrentYoutubeFFMPEGClient\shell\Console_32_edit"; ValueType: string; ValueName: "MUIVerb"; ValueData: "Edit FFMPEG v4.2.2 32-bit pre-typed console bat-file";
Root: HKCU; Subkey: "Software\Classes\*\shell\1ClickTorrentYoutubeFFMPEGClient\shell\Console_32_edit\command"; ValueType: string; ValueName: ""; ValueData: "notepad.exe ""{app}\1ClickTorrentYoutubeFFMPEGClient\win32\ffconsole_editable.bat""";
Root: HKCU; Subkey: "Software\Classes\*\shell\1ClickTorrentYoutubeFFMPEGClient\shell\OneClickTorrentDownload"; ValueType: string; ValueName: "MUIVerb"; ValueData: "Download torrent";
Root: HKCU; Subkey: "Software\Classes\*\shell\1ClickTorrentYoutubeFFMPEGClient\shell\OneClickTorrentDownload\command"; ValueType: string; ValueName: ""; ValueData: "cmd.exe /C """"{app}\1ClickTorrentYoutubeFFMPEGClient\win32\ffconsole_download_torrent.bat"" ""%1""""";
Root: HKCU; Subkey: "Software\Classes\*\shell\1ClickTorrentYoutubeFFMPEGClient\shell\OneClickTorrentDownplay"; ValueType: string; ValueName: "MUIVerb"; ValueData: "Download torrent and start to play @10%";
Root: HKCU; Subkey: "Software\Classes\*\shell\1ClickTorrentYoutubeFFMPEGClient\shell\OneClickTorrentDownplay\command"; ValueType: string; ValueName: ""; ValueData: "cmd.exe /C """"{app}\1ClickTorrentYoutubeFFMPEGClient\win32\ffconsole_downplay_torrent.bat"" ""%1""""";
Root: HKCU; Subkey: "Software\Classes\*\shell\1ClickTorrentYoutubeFFMPEGClient\shell\OneClickYoutubeDownload"; ValueType: string; ValueName: "MUIVerb"; ValueData: "Download Youtube URL from text file";
Root: HKCU; Subkey: "Software\Classes\*\shell\1ClickTorrentYoutubeFFMPEGClient\shell\OneClickYoutubeDownload\command"; ValueType: string; ValueName: ""; ValueData: "cmd.exe /C """"{app}\1ClickTorrentYoutubeFFMPEGClient\win32\ffconsole_download_youtube.bat"" ""%1""""";
Root: HKCU; Subkey: "Software\Classes\*\shell\1ClickTorrentYoutubeFFMPEGClient\shell\TranscodeTo"; ValueType: string; ValueName: "MUIVerb"; ValueData: "Transcode to (extremely fast) ...";
Root: HKCU; Subkey: "Software\Classes\*\shell\1ClickTorrentYoutubeFFMPEGClient\shell\TranscodeTo"; ValueType: string; ValueName: "SubCommands"; ValueData: "";
Root: HKCU; Subkey: "Software\Classes\*\shell\1ClickTorrentYoutubeFFMPEGClient\shell\TranscodeTo"; ValueType: expandsz; ValueName: "Icon"; ValueData: "{app}\1ClickTorrentYoutubeFFMPEGClient\transcode.ico";
Root: HKCU; Subkey: "Software\Classes\*\shell\1ClickTorrentYoutubeFFMPEGClient\shell\TranscodeTo\shell\TranscodeToMp4"; ValueType: string; ValueName: "MUIVerb"; ValueData: "Transcode to mp4";
Root: HKCU; Subkey: "Software\Classes\*\shell\1ClickTorrentYoutubeFFMPEGClient\shell\TranscodeTo\shell\TranscodeToMp4\command"; ValueType: string; ValueName: ""; ValueData: "cmd.exe /C """"{app}\1ClickTorrentYoutubeFFMPEGClient\win32\ffconsole_transcode_to_mp4.bat"" ""%1""""";
Root: HKCU; Subkey: "Software\Classes\*\shell\1ClickTorrentYoutubeFFMPEGClient\shell\TranscodeTo\shell\TranscodeToMov"; ValueType: string; ValueName: "MUIVerb"; ValueData: "Transcode to mov";
Root: HKCU; Subkey: "Software\Classes\*\shell\1ClickTorrentYoutubeFFMPEGClient\shell\TranscodeTo\shell\TranscodeToMov\command"; ValueType: string; ValueName: ""; ValueData: "cmd.exe /C """"{app}\1ClickTorrentYoutubeFFMPEGClient\win32\ffconsole_transcode_to_mov.bat"" ""%1""""";
Root: HKCU; Subkey: "Software\Classes\*\shell\1ClickTorrentYoutubeFFMPEGClient\shell\TranscodeTo\shell\TranscodeToMkv"; ValueType: string; ValueName: "MUIVerb"; ValueData: "Transcode to mkv";
Root: HKCU; Subkey: "Software\Classes\*\shell\1ClickTorrentYoutubeFFMPEGClient\shell\TranscodeTo\shell\TranscodeToMkv\command"; ValueType: string; ValueName: ""; ValueData: "cmd.exe /C """"{app}\1ClickTorrentYoutubeFFMPEGClient\win32\ffconsole_transcode_to_mkv.bat"" ""%1""""";
Root: HKCU; Subkey: "Software\Classes\*\shell\1ClickTorrentYoutubeFFMPEGClient\shell\EncodeTo"; ValueType: string; ValueName: "MUIVerb"; ValueData: "Encode to (moderately fast) ...";
Root: HKCU; Subkey: "Software\Classes\*\shell\1ClickTorrentYoutubeFFMPEGClient\shell\EncodeTo"; ValueType: string; ValueName: "SubCommands"; ValueData: "";
Root: HKCU; Subkey: "Software\Classes\*\shell\1ClickTorrentYoutubeFFMPEGClient\shell\EncodeTo"; ValueType: expandsz; ValueName: "Icon"; ValueData: "{app}\1ClickTorrentYoutubeFFMPEGClient\encode.ico";
Root: HKCU; Subkey: "Software\Classes\*\shell\1ClickTorrentYoutubeFFMPEGClient\shell\EncodeTo\shell\EncodeToStandard480pMp4SW"; ValueType: string; ValueName: "MUIVerb"; ValueData: "Encode/scale to standard 480p h264 mp4 (software libx264)";
Root: HKCU; Subkey: "Software\Classes\*\shell\1ClickTorrentYoutubeFFMPEGClient\shell\EncodeTo\shell\EncodeToStandard480pMp4SW\command"; ValueType: string; ValueName: ""; ValueData: "cmd.exe /C """"{app}\1ClickTorrentYoutubeFFMPEGClient\win32\ffconsole_encode_to_standard_480p_mp4_sw.bat"" ""%1""""";
Root: HKCU; Subkey: "Software\Classes\*\shell\1ClickTorrentYoutubeFFMPEGClient\shell\EncodeTo\shell\EncodeToStandard720pMp4SW"; ValueType: string; ValueName: "MUIVerb"; ValueData: "Encode/scale to standard 720p h264 mp4 (software libx264)";
Root: HKCU; Subkey: "Software\Classes\*\shell\1ClickTorrentYoutubeFFMPEGClient\shell\EncodeTo\shell\EncodeToStandard720pMp4SW\command"; ValueType: string; ValueName: ""; ValueData: "cmd.exe /C """"{app}\1ClickTorrentYoutubeFFMPEGClient\win32\ffconsole_encode_to_standard_720p_mp4_sw.bat"" ""%1""""";
Root: HKCU; Subkey: "Software\Classes\*\shell\1ClickTorrentYoutubeFFMPEGClient\shell\EncodeTo\shell\EncodeToStandard1080pMp4SW"; ValueType: string; ValueName: "MUIVerb"; ValueData: "Encode/scale to standard 1080p h264 mp4 (software libx264)";
Root: HKCU; Subkey: "Software\Classes\*\shell\1ClickTorrentYoutubeFFMPEGClient\shell\EncodeTo\shell\EncodeToStandard1080pMp4SW\command"; ValueType: string; ValueName: ""; ValueData: "cmd.exe /C """"{app}\1ClickTorrentYoutubeFFMPEGClient\win32\ffconsole_encode_to_standard_1080p_mp4_sw.bat"" ""%1""""";
Root: HKCU; Subkey: "Software\Classes\*\shell\1ClickTorrentYoutubeFFMPEGClient\shell\AudioOnly"; ValueType: string; ValueName: "MUIVerb"; ValueData: "Audio only (very fast) ...";
Root: HKCU; Subkey: "Software\Classes\*\shell\1ClickTorrentYoutubeFFMPEGClient\shell\AudioOnly"; ValueType: string; ValueName: "SubCommands"; ValueData: "";
Root: HKCU; Subkey: "Software\Classes\*\shell\1ClickTorrentYoutubeFFMPEGClient\shell\AudioOnly"; ValueType: expandsz; ValueName: "Icon"; ValueData: "{app}\1ClickTorrentYoutubeFFMPEGClient\audio.ico";
Root: HKCU; Subkey: "Software\Classes\*\shell\1ClickTorrentYoutubeFFMPEGClient\shell\AudioOnly\shell\IncVolumeNoClipping"; ValueType: string; ValueName: "MUIVerb"; ValueData: "Inclrease volume without clipping";
Root: HKCU; Subkey: "Software\Classes\*\shell\1ClickTorrentYoutubeFFMPEGClient\shell\AudioOnly\shell\IncVolumeNoClipping\command"; ValueType: string; ValueName: ""; ValueData: "cmd.exe /C """"{app}\1ClickTorrentYoutubeFFMPEGClient\win32\ffconsole_inc_vol_no_clipping.bat"" ""%1""""";
Root: HKCU; Subkey: "Software\Classes\*\shell\1ClickTorrentYoutubeFFMPEGClient\shell\AudioOnly\shell\ExtractStereoMp3"; ValueType: string; ValueName: "MUIVerb"; ValueData: "Extract stereo mp3";
Root: HKCU; Subkey: "Software\Classes\*\shell\1ClickTorrentYoutubeFFMPEGClient\shell\AudioOnly\shell\ExtractStereoMp3\command"; ValueType: string; ValueName: ""; ValueData: "cmd.exe /C """"{app}\1ClickTorrentYoutubeFFMPEGClient\win32\ffconsole_extract_stereo_mp3.bat"" ""%1""""";


[Run]
Filename: "{app}\1ClickTorrentYoutubeFFMPEGClient\sample_videos"; Description: "View the sample videos. Select a file then right-click the mouse"; Flags: postinstall shellexec skipifsilent
Filename: "https://magnetdl.me/download/movies/"; Description: "Open a search site with movies by default browser. Click on a magnet link to download and play @10%"; Flags: postinstall shellexec skipifsilent


[UninstallDelete]
Type: filesandordirs; Name: "{app}\1ClickTorrentYoutubeFFMPEGClient"


[CustomMessages]
AppName=1 Click Torrent Youtube FFMPEG Client version 1.0.0.0
LaunchProgram=Start application after finishing installation
