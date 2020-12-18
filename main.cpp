/*
Magnet tests
https://yifytorrenthd.net/movie/5727-aliens-vs-predator-requiem-2007
good site with catalog
https://eztv.re/
yet another good site
https://magnetdl.me/download/movies/
TODO: ShellExec default browser on install
*/

/*
https://forum.deluge-torrent.org/viewtopic.php?t=39917

[HKEY_CURRENT_USER\Software\Classes\magnet]
@="URL:MAGNET URI"
"URL Protocol"=""

[HKEY_CURRENT_USER\Software\Classes\magnet\DefaultIcon]
@="C:\\Program Files\\Deluge\\deluge.exe"

[HKEY_CURRENT_USER\Software\Classes\magnet\shell]

[HKEY_CURRENT_USER\Software\Classes\magnet\shell\open]

[HKEY_CURRENT_USER\Software\Classes\magnet\shell\open\command]
@="C:\\Program Files\\Deluge\\deluge.exe \"%1\""
*/

/*

Copyright (c) 2016, Arvid Norberg
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

	* Redistributions of source code must retain the above copyright
	  notice, this list of conditions and the following disclaimer.
	* Redistributions in binary form must reproduce the above copyright
	  notice, this list of conditions and the following disclaimer in
	  the documentation and/or other materials provided with the distribution.
	* Neither the name of the author nor the names of its
	  contributors may be used to endorse or promote products derived
	  from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.

*/

#include <iostream>
#include <thread>
#include <chrono>
#include <fstream>
#include <regex>
#include <string>
#include <cstdlib>

#include <libtorrent/session.hpp>
#include <libtorrent/add_torrent_params.hpp>
#include <libtorrent/torrent_handle.hpp>
#include <libtorrent/alert_types.hpp>
#include <libtorrent/bencode.hpp>
#include <libtorrent/torrent_status.hpp>
#include <libtorrent/read_resume_data.hpp>
#include <libtorrent/write_resume_data.hpp>
#include <libtorrent/error_code.hpp>
#include <libtorrent/magnet_uri.hpp>
#include <libtorrent/torrent_info.hpp>

#include <shellapi.h>
#include <Shlobj.h>

using clk = std::chrono::steady_clock;

// return the name of a torrent status enum
char const* state(lt::torrent_status::state_t s)
{
	switch (s) {
	case lt::torrent_status::checking_files: return "checking";
	case lt::torrent_status::downloading_metadata: return "dl metadata";
	case lt::torrent_status::downloading: return "downloading";
	case lt::torrent_status::finished: return "finished";
	case lt::torrent_status::seeding: return "seeding";
	case lt::torrent_status::allocating: return "allocating";
	case lt::torrent_status::checking_resume_data: return "checking resume";
	default: return "<>";
	}
}

void* ReadLibrary(const char* dllfilename, size_t* pSize)
{
	size_t read;
	void* result;
	FILE* fp;

	fp = fopen(dllfilename, "rb");
	if (fp == NULL)
	{
		return NULL;
	}

	fseek(fp, 0, SEEK_END);
	*pSize = (size_t)(ftell(fp));
	if (*pSize == 0)
	{
		fclose(fp);
		return NULL;
	}

	result = (unsigned char*)malloc(*pSize + 1);
	if (result == NULL)
	{
		return NULL;
	}

	fseek(fp, 0, SEEK_SET);
	read = fread(result, 1, *pSize, fp);
	fclose(fp);
	if (read != *pSize)
	{
		free(result);
		return NULL;
	}

	*((unsigned char*)result + *pSize) = '\0';

	return result;
}

//////////////////////////////////////////////////////////////

typedef UINT_PTR(__stdcall* LPOFNHOOKPROC) (HWND, UINT, WPARAM, LPARAM);
typedef struct tagEDITMENU
{
	HMENU   hmenu;
	WORD    idEdit;
	WORD    idCut;
	WORD    idCopy;
	WORD    idPaste;
	WORD    idClear;
	WORD    idUndo;
} EDITMENU;
typedef EDITMENU* LPEDITMENU;
typedef struct tagOFNW {
	DWORD         lStructSize;
	HWND          hwndOwner;
	HINSTANCE     hInstance;
	LPCWSTR       lpstrFilter;
	LPWSTR        lpstrCustomFilter;
	DWORD         nMaxCustFilter;
	DWORD         nFilterIndex;
	LPWSTR        lpstrFile;
	DWORD         nMaxFile;
	LPWSTR        lpstrFileTitle;
	DWORD         nMaxFileTitle;
	LPCWSTR       lpstrInitialDir;
	LPCWSTR       lpstrTitle;
	DWORD         Flags;
	WORD          nFileOffset;
	WORD          nFileExtension;
	LPCWSTR       lpstrDefExt;
	LPARAM        lCustData;
	LPOFNHOOKPROC lpfnHook;
	LPCWSTR       lpTemplateName;
	LPEDITMENU    lpEditInfo;
	LPCSTR        lpstrPrompt;
	void* pvReserved;
	DWORD         dwReserved;
	DWORD         FlagsEx;
} OPENFILENAMEW, * LPOPENFILENAMEW;

extern "C" BOOL  __stdcall GetSaveFileNameW(LPOPENFILENAMEW);
extern "C" DWORD __stdcall CommDlgExtendedError();

#define OFN_EXPLORER                 0x00080000     // new look commdlg
#define OFN_HIDEREADONLY             0x00000004

#define CDSIZEOF_STRUCT(structname, member)  (((int)((LPBYTE)(&((structname*)0)->member) - ((LPBYTE)((structname*)0)))) + sizeof(((structname*)0)->member))

#define OPENFILENAME_SIZE_VERSION_400W  CDSIZEOF_STRUCT(OPENFILENAMEW,lpTemplateName)

//////////////////////////////////////////////////////////////

static int WriteBinaryW(const wchar_t* wfilename, const unsigned char* pChar, size_t pSize)
{
	FILE* fp;

	fp = _wfopen(wfilename, L"wb");
	if (fp == NULL)
	{
		return __LINE__;
	}

	size_t szw = fwrite(pChar, pSize, 1, fp);
	if (szw != 1)
		return __LINE__;

	fclose(fp);
	return 0;
}

static void* ReadBinaryW(const wchar_t* wfilename, size_t* pSize)
{
	size_t read;
	void* result;
	FILE* fp;

	fp = _wfopen(wfilename, L"rb");
	if (fp == NULL)
	{
		return NULL;
	}

	fseek(fp, 0, SEEK_END);
	*pSize = (size_t)(ftell(fp));
	if (*pSize == 0)
	{
		fclose(fp);
		return NULL;
	}

	result = (unsigned char*)malloc(*pSize);
	if (result == NULL)
	{
		return NULL;
	}

	fseek(fp, 0, SEEK_SET);
	read = fread(result, 1, *pSize, fp);
	fclose(fp);
	if (read != *pSize)
	{
		free(result);
		return NULL;
	}

	return result;
}

//////////////////////////////////////////////////////////////

/// <summary>Automatically closes a search handle upon destruction</summary>
class SearchHandleScope {

	/// <summary>Initializes a new search handle closer</summary>
	/// <param name="searchHandle">Search handle that will be closed on destruction</param>
public: SearchHandleScope(HANDLE searchHandle) :
	m_searchHandle(searchHandle) {}

	  /// <summary>Closes the search handle</summary>
public: ~SearchHandleScope() {
	::FindClose(this->m_searchHandle);
}

	  /// <summary>Search handle that will be closed when the instance is destroyed</summary>
private: HANDLE m_searchHandle;

};

typedef int (*FileCallback)(const std::wstring& path, const WIN32_FIND_DATAW& fnd_data);

static int ScanOneLevelDirectory(const std::wstring& path, const std::wstring& allFilesMask_in, FileCallback pfFileCallback) {
	//wprintf(L"ScanOneLevelDirectory() path = %s ...\n", path.c_str());
	WIN32_FIND_DATAW findData;

	// First, delete the contents of the directory, recursively for subdirectories
	std::wstring searchMask = path + allFilesMask_in;
	//wprintf(L"ScanOneLevelDirectory() searchMask = %s ...\n", searchMask.c_str());
	HANDLE searchHandle = ::FindFirstFileExW(
		searchMask.c_str(), FindExInfoBasic, &findData, FindExSearchNameMatch, nullptr, FIND_FIRST_EX_LARGE_FETCH
	);
	if (searchHandle == INVALID_HANDLE_VALUE) {
		DWORD lastError = ::GetLastError();
		if (lastError != ERROR_FILE_NOT_FOUND) { // or ERROR_NO_MORE_FILES, ERROR_NOT_FOUND?
			return __LINE__;
		}
		//wprintf(L"ScanOneLevelDirectory() path = %s ERROR_FILE_NOT_FOUND is OK, return 0\n", path.c_str());
		return 0;
	}

	// Did this directory have any contents? If so, delete them first

	SearchHandleScope scope(searchHandle);
	for (;;) {

		// Do not process the obligatory '.' and '..' directories
		bool isDirectory =
			((findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0);
		bool isGoodDirectory =
			((findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0)
			&& (findData.cFileName[0] != '.');

		// Subdirectories need to be handled by deleting their contents first
		std::wstring filePath = path + L'\\' + findData.cFileName;
		//wprintf(L"ScanOneLevelDirectory(iteration) filePath = %s ...\n", filePath.c_str());
		if (!isDirectory) {
			int ret = (*pfFileCallback)(filePath, findData);
			if (ret != 0)
				return ret;
		}

		if (isGoodDirectory)
		{
			int rres = ScanOneLevelDirectory(filePath, allFilesMask_in, pfFileCallback);
		}

		// Advance to the next file in the directory
		//wprintf(L"ScanOneLevelDirectory(FindNextFileW) ...\n");
		BOOL result = ::FindNextFileW(searchHandle, &findData);
		if (result == FALSE) {
			DWORD lastError = ::GetLastError();
			if (lastError != ERROR_NO_MORE_FILES) {
				return __LINE__;
			}
			//wprintf(L"ScanOneLevelDirectory(FindNextFileW) ERROR_NO_MORE_FILES is OK, break\n");
			break; // All directory contents enumerated and deleted
		}
		//wprintf(L"ScanOneLevelDirectory(FindNextFileW) OK.\n");

	} // for

	return 0;
}

std::wstring g_biggest_movie_path = L"";
typedef unsigned long long ULL64;
ULL64 g_biggest_movie_size = 0;

static bool MovieFile(const WIN32_FIND_DATAW& fnd_data)
{
	std::wstring wstrFileName = std::wstring(fnd_data.cFileName);
	if (wstrFileName.find(L".mp4") == (wstrFileName.size() - 4))
		return true;
	if (wstrFileName.find(L".mkv") == (wstrFileName.size() - 4))
		return true;
	if (wstrFileName.find(L".mov") == (wstrFileName.size() - 4))
		return true;
	if (wstrFileName.find(L".avi") == (wstrFileName.size() - 4))
		return true;
	return false;
}

static int BiggestMovieFileCallback(const std::wstring& path, const WIN32_FIND_DATAW& fnd_data)
{
	ULL64 curFileSize = (ULL64(fnd_data.nFileSizeHigh) << 32) | ULL64(fnd_data.nFileSizeLow);
	if ((MovieFile(fnd_data)) && (curFileSize > g_biggest_movie_size))
	{
		g_biggest_movie_size = curFileSize;
		g_biggest_movie_path = path;
	}
	return 0;
}

static int BiggestFileCallback(const std::wstring& path, const WIN32_FIND_DATAW& fnd_data)
{
	ULL64 curFileSize = (ULL64(fnd_data.nFileSizeHigh) << 32) | ULL64(fnd_data.nFileSizeLow);
	if ((curFileSize > g_biggest_movie_size))
	{
		g_biggest_movie_size = curFileSize;
		g_biggest_movie_path = path;
	}
	return 0;
}

static int SetCmdExePath(WCHAR cmdPathW[MAX_PATH])
{
	static const WCHAR* wcmd = L"\\cmd.exe";

	WCHAR* pszPathW = NULL;
	HRESULT hres = SHGetKnownFolderPath(FOLDERID_System, 0, NULL, &pszPathW);

	if (hres != S_OK)
		return __LINE__;

	size_t wl = wcslen(pszPathW);
	size_t wc = wcslen(wcmd);

	if (wl > (MAX_PATH - 1 - wc))
		return __LINE__;

	memset(cmdPathW, 0, MAX_PATH * sizeof(WCHAR));
	wcscpy(cmdPathW, pszPathW);
	wcscpy(cmdPathW + wl, wcmd);

	return 0;
}

bool g_bPlayerAlreadyStarted = false;

//////////////////////////////////////////////////////////////

typedef std::map<std::string, std::wstring> AllWideStrings;
static AllWideStrings g_allWideStrings;

static const wchar_t* ToWStr(const char* sA)
{
	AllWideStrings::const_iterator it = g_allWideStrings.find(sA);

	if (it != g_allWideStrings.end())
		return it->second.c_str();

	size_t srcLen = strlen(sA);
	wchar_t* sW = (wchar_t*)_alloca((srcLen + 1) * sizeof(wchar_t));

	if (sW == NULL)
		return L"NULL";

	for (size_t i = 0; i < (srcLen + 1); ++i)
		sW[i] = wchar_t(sA[i]);

	g_allWideStrings[sA] = sW;

	return ToWStr(sA);
}

//////////////////////////////////////////////////////////////

FILE* fp_log = NULL;
std::string strMagnetOrTorrent;

int main(int argc, char const* argv[]) try
{
/*
	WCHAR path[MAX_PATH];
	memset(path, 0, MAX_PATH * sizeof(WCHAR));
	DWORD plen = GetModuleFileNameW(NULL, path, MAX_PATH);
	if (plen == 0)
		return __LINE__;
	size_t i = MAX_PATH;
	while (path[--i] != L'\\');
	path[i] = L'\0';
	BOOL BRes = SetCurrentDirectoryW(path);
	if (!BRes)
		return __LINE__;
*/
	if ((argc != 2) && (argc != 3)) {
		std::cerr << "usage: " << argv[0] << " <magnet_link_or_torrent_file> [play]" << std::endl;
		return 1;
	}

	if (argc == 3) {
		printf("\nI will start playing automatically after 10%% of torrent will be downloaded.\n");
	}

	if (std::getenv("FFREPORT") != NULL)
	{
		std::string strFFREPORT = std::getenv("FFREPORT");
		std::string::size_type icol = strFFREPORT.rfind(':');
		if (icol != std::string::npos)
			strFFREPORT = strFFREPORT.substr(0, icol);
		icol = strFFREPORT.find("file=");
		if (icol != std::string::npos)
			strFFREPORT = strFFREPORT.substr(icol + 5);
		icol = strFFREPORT.find(".log");
		if ((icol != 0) && (icol != std::string::npos))
		{
			fp_log = fopen(strFFREPORT.c_str(), "wb");
		}
	}

	bool bIsTorrentFile = false;

	strMagnetOrTorrent = argv[1];

	bool bIsIndirect = (strMagnetOrTorrent.find(".txt") == (strMagnetOrTorrent.size() - 4));

	if (bIsIndirect) {
		size_t szIndirectBody = 0;
		char* pIndirectBody = (char*)ReadLibrary(strMagnetOrTorrent.c_str(), &szIndirectBody);
		if ((pIndirectBody == NULL) || (szIndirectBody == 0)) {
			if (fp_log) fprintf(fp_log, "Error: %s - %s\n", argv[1], "Indirect body - bad file");
			if (fp_log) fclose(fp_log);
			if (!fp_log) MessageBoxA(NULL, argv[1], "Indirect body - bad file", MB_ICONERROR);
			exit(__LINE__);
		}

		strMagnetOrTorrent = std::string(pIndirectBody);
	}

	if (strMagnetOrTorrent.find("magnet:?") != 0)
	{
		const std::string dotTorrent = ".torrent";
		bIsTorrentFile = strMagnetOrTorrent.find(dotTorrent) == (strMagnetOrTorrent.size() - dotTorrent.size());
		if (!bIsTorrentFile)
		{
			if (fp_log) fprintf(fp_log, "Error: %s - %s\n", argv[1], "Not a magnet link, not a torrent file");
			if (fp_log) fclose(fp_log);
			if (!fp_log) MessageBoxA(NULL, argv[1], "Not a magnet link, not a torrent file", MB_ICONERROR);
			exit(__LINE__);
		}
	}

	std::shared_ptr<lt::torrent_info> ti;

	//strMagnetOrTorrent = std::regex_replace(strMagnetOrTorrent, std::regex(" "), "%32");

	/*
	size_t szTorrentBody = 0;
	char* pTorrentBody = (char *)ReadLibrary(argv[1], &szTorrentBody);
	if (pTorrentBody == NULL)
		abort();
	*/
	printf("\nmagnet-url/torrent-file: %s\n", strMagnetOrTorrent.c_str());

	lt::settings_pack pack;
	pack.set_int(lt::settings_pack::alert_mask
		, lt::alert::error_notification
		| lt::alert::storage_notification
		| lt::alert::status_notification
		| lt::alert::piece_progress_notification
	);

	lt::session ses(pack);
	clk::time_point last_save_resume = clk::now();

	wchar_t szFileName[MAX_PATH];
	ZeroMemory(szFileName, MAX_PATH * sizeof(wchar_t));
	if (bIsTorrentFile)
		wcscpy(szFileName, L"torrent.txt");
	else
		wcscpy(szFileName, L"magnet.txt");

	OPENFILENAMEW ofn;
	ZeroMemory(&ofn, sizeof(OPENFILENAMEW));

	std::string strMagnetTxtAbsPath = "";

	if ((!bIsTorrentFile) && (!bIsIndirect))
	{
		ofn.lStructSize = OPENFILENAME_SIZE_VERSION_400W;// sizeof(OPENFILENAMEW);
		ofn.hwndOwner = NULL;
		ofn.lpstrFilter = L"Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
		ofn.lpstrFile = szFileName;
		ofn.nMaxFile = MAX_PATH;
		ofn.Flags = OFN_EXPLORER | OFN_HIDEREADONLY;
		ofn.lpstrDefExt = L"txt";

		BOOL BResGSFN = GetSaveFileNameW(&ofn);
		if (BResGSFN != TRUE)
		{
			DWORD cderr = CommDlgExtendedError();
			if (cderr != 0)
			{
				char buferr[256];
				sprintf_s(buferr, 255, "%04X", cderr);
				if (fp_log) fprintf(fp_log, "Error: %s - %s\n", buferr, "File dialog error");
				if (fp_log) fclose(fp_log);
				if (!fp_log) MessageBoxA(NULL, buferr, "File dialog error", MB_ICONERROR);
				exit(__LINE__);
			}
			else
			{
				exit(__LINE__);
			}
		}
	}
	else if ((!bIsTorrentFile) && (bIsIndirect)) {
		DWORD szCurDirLen = GetCurrentDirectoryA(0, NULL);
		char* bufCurDir = (char*)malloc(szCurDirLen);
		if (bufCurDir)
		{
			szCurDirLen = GetCurrentDirectoryA(szCurDirLen, bufCurDir);
			if (szCurDirLen > 0)
			{
				strMagnetTxtAbsPath = std::string(bufCurDir) + std::string("\\") + std::string(argv[1]);
				ofn.lpstrFile = (LPWSTR)ToWStr(strMagnetTxtAbsPath.c_str());
			}
		}
	}

	std::string strFullPathMagnetOrTorrent = strMagnetOrTorrent;
	if (bIsTorrentFile)
	{
		if (strFullPathMagnetOrTorrent.find(':') != 1) // Not an absolute path already
		{
			DWORD szCurDirLen = GetCurrentDirectoryA(0, NULL);
			char* bufCurDir = (char*)malloc(szCurDirLen);
			if (bufCurDir)
			{
				szCurDirLen = GetCurrentDirectoryA(szCurDirLen, bufCurDir);
				if (szCurDirLen > 0)
				{
					strFullPathMagnetOrTorrent = std::string(bufCurDir) + std::string("\\") + strMagnetOrTorrent;

					lt::error_code ec;
					ti = std::make_shared<lt::torrent_info>(strFullPathMagnetOrTorrent, ec);
					if (ec)
					{
						if (fp_log) fprintf(fp_log, "Error: %s - %s\n", strFullPathMagnetOrTorrent.c_str(), "Bad torrent file");
						if (fp_log) fclose(fp_log);
						exit(__LINE__);
					}
				}
			}
		}
		else {
			lt::error_code ec;
			ti = std::make_shared<lt::torrent_info>(strFullPathMagnetOrTorrent, ec);
			if (ec)
			{
				if (fp_log) fprintf(fp_log, "Error: %s - %s\n", strFullPathMagnetOrTorrent.c_str(), "Bad torrent file");
				if (fp_log) fclose(fp_log);
				exit(__LINE__);
			}
		}
	}

	std::string torrent_txt_file_path = "";

	if (bIsTorrentFile)
	{
		torrent_txt_file_path = strFullPathMagnetOrTorrent + std::string(".txt");
		ofn.lpstrFile = (LPWSTR)ToWStr(torrent_txt_file_path.c_str());
	}

	size_t szReadBody = 0;
	void* pReadBody = ReadBinaryW(ofn.lpstrFile, &szReadBody);

	if ((pReadBody != NULL) && (strFullPathMagnetOrTorrent.size() != szReadBody) && (memcmp(strFullPathMagnetOrTorrent.c_str(), pReadBody, strFullPathMagnetOrTorrent.size()) != 0))
	{
		if (fp_log) fwprintf(fp_log, L"Error: %s - %s\n", ofn.lpstrFile, L"File with the same torrent info exist, select another");
		if (fp_log) fclose(fp_log);
		if (!fp_log) MessageBoxW(NULL, ofn.lpstrFile, L"File with the same torrent info exist, select another", MB_ICONERROR);
		exit(__LINE__);
	}

	int wres = WriteBinaryW(ofn.lpstrFile, (const unsigned char *)strFullPathMagnetOrTorrent.c_str(), strFullPathMagnetOrTorrent.size());
	if (wres != 0)
	{
		if (fp_log) fwprintf(fp_log, L"Error: %s - %s\n", ofn.lpstrFile, L"Can't write to this file");
		if (fp_log) fclose(fp_log);
		if (!fp_log) MessageBoxW(NULL, ofn.lpstrFile, L"Can't write to this file", MB_ICONERROR);
		exit(__LINE__);
	}

	std::wstring wstrShellDir = ofn.lpstrFile;
	wstrShellDir = wstrShellDir.substr(0, wstrShellDir.find_last_of(L"/\\"));
	ShellExecuteW(NULL, L"open", wstrShellDir.c_str(), NULL, NULL, SW_SHOWDEFAULT);

	std::wstring wstrDownloadDir = std::wstring(ofn.lpstrFile) + std::wstring(L".downloaded_data");
	CreateDirectoryW(wstrDownloadDir.c_str(), NULL);
	BOOL BRes2 = SetCurrentDirectoryW(wstrDownloadDir.c_str());
	if (BRes2 != TRUE)
	{
		if (fp_log) fwprintf(fp_log, L"Error: %s - %s\n", wstrDownloadDir.c_str(), L"SetCurrentDirectoryW error");
		if (fp_log) fclose(fp_log);
		if (!fp_log) MessageBoxW(NULL, wstrDownloadDir.c_str(), L"SetCurrentDirectoryW error", MB_ICONERROR);
		exit(__LINE__);
	}

	// load resume data from disk and pass it in as we add the magnet link
	std::ifstream ifs(".resume_file", std::ios_base::binary);
	ifs.unsetf(std::ios_base::skipws);
	std::vector<char> buf{ std::istream_iterator<char>(ifs)
		, std::istream_iterator<char>() };

	lt::add_torrent_params atp;
	if (buf.size() > 0)
		atp = lt::read_resume_data(buf);

	if (bIsTorrentFile)
	{
		lt::add_torrent_params torrent;
		torrent.ti = ti;
		torrent.flags &= ~lt::torrent_flags::duplicate_is_error;
		torrent.info_hash = ti->info_hash();

		if (atp.info_hash != torrent.info_hash) {
			atp = std::move(torrent);
		}
	}
	else
	{
		lt::add_torrent_params magnet = lt::parse_magnet_uri(strMagnetOrTorrent.c_str());
		if (atp.info_hash != magnet.info_hash) {
			atp = std::move(magnet);
		}
	}
	atp.save_path = "."; // save in current dir
	ses.async_add_torrent(std::move(atp));

	////////////////////////////////////////////////////////////////////////
	
	std::vector<lt::download_priority_t> piece_vec;
	size_t piece_vec_size = 0;
	std::vector<char> piece_downloaded_vec;
	
	size_t szpiece_downloaded_vec_data = 0;
	void* ppiece_downloaded_vec_data = ReadBinaryW(L"piece_downloaded_vec.dat", &szpiece_downloaded_vec_data);
	if ((ppiece_downloaded_vec_data != NULL) && (szpiece_downloaded_vec_data > 0)) {
		piece_downloaded_vec.resize(szpiece_downloaded_vec_data);
		memcpy(&piece_downloaded_vec[0], ppiece_downloaded_vec_data, szpiece_downloaded_vec_data);
	}
	
	////////////////////////////////////////////////////////////////////////

	// this is the handle we'll set once we get the notification of it being
	// added
	lt::torrent_handle h;
	for (;;) {
		std::vector<lt::alert*> alerts;
		ses.pop_alerts(&alerts);

		for (lt::alert const* a : alerts) {
			if (auto at = lt::alert_cast<lt::add_torrent_alert>(a)) {
				h = at->handle;
			}

			if (lt::alert_cast<lt::torrent_checked_alert>(a)) {
				//h.set_upload_limit(1024);
				//std::vector<lt::download_priority_t> file_vec = h.get_file_priorities();
				piece_vec = h.get_piece_priorities();
				piece_vec_size = piece_vec.size();

				if (piece_vec_size >= 1) {
					if (piece_downloaded_vec.size() == 0) {
						piece_downloaded_vec.resize(piece_vec_size);
						std::fill(piece_downloaded_vec.begin(), piece_downloaded_vec.end(), false);
					}

					if (argc == 3) {
						for (size_t i = 0; i < piece_vec_size; ++i) {
							h.set_piece_deadline(i, i);
						}

						if (piece_vec_size >= 100) {
							for (size_t i = 0; i < piece_vec_size / 20; ++i) {
								h.set_piece_deadline(i, i);
							}
							for (size_t i = 0; i < piece_vec_size / 20; ++i) {
								h.set_piece_deadline(piece_vec_size - 1 - i, piece_vec_size / 20 + i);
							}
							for (size_t i = piece_vec_size / 20; i < piece_vec_size - piece_vec_size / 20; ++i) {
								h.set_piece_deadline(i, piece_vec_size / 20 + piece_vec_size / 20 + i);
							}
						}
					}
				}
			}

			if (auto rpa = lt::alert_cast<lt::piece_finished_alert>(a)) {
				int p = rpa->piece_index;

				if (piece_vec_size >= 1) {
					piece_downloaded_vec[p] = true;

					if (argc == 3) {
						bool bTenPercentDownloaded = true;
						bTenPercentDownloaded = bTenPercentDownloaded && (piece_downloaded_vec[0] == true);
						bTenPercentDownloaded = bTenPercentDownloaded && (piece_downloaded_vec[piece_vec_size - 1] == true);

						if (piece_vec_size >= 100) {
							for (size_t i = 0; i < piece_vec_size / 20; ++i) {
								bTenPercentDownloaded = bTenPercentDownloaded && (piece_downloaded_vec[i] == true);
								bTenPercentDownloaded = bTenPercentDownloaded && (piece_downloaded_vec[piece_vec_size - 1 - i] == true);
							}
						}

						if (bTenPercentDownloaded) {
							if (!g_bPlayerAlreadyStarted) {
								int sres = ScanOneLevelDirectory(wstrDownloadDir, L"\\*", BiggestMovieFileCallback);
								if (sres != 0)
								{
									if (fp_log) fwprintf(fp_log, L"Error: %s - %s\n", wstrDownloadDir.c_str(), L"BiggestMovieFileCallback error");
									if (fp_log) fclose(fp_log);
									if (!fp_log) MessageBoxW(NULL, wstrDownloadDir.c_str(), L"BiggestMovieFileCallback error", MB_ICONERROR);
									exit(__LINE__);
								}

								if (g_biggest_movie_size == 0) {
									int sres = ScanOneLevelDirectory(wstrDownloadDir, L"\\*", BiggestFileCallback);
									if (sres != 0)
									{
										if (fp_log) fwprintf(fp_log, L"Error: %s - %s\n", wstrDownloadDir.c_str(), L"BiggestFileCallback error");
										if (fp_log) fclose(fp_log);
										if (!fp_log) MessageBoxW(NULL, wstrDownloadDir.c_str(), L"BiggestFileCallback error", MB_ICONERROR);
										exit(__LINE__);
									}
								}

								wprintf(L"\r\n%s\n", g_biggest_movie_path.c_str());

								WCHAR path[MAX_PATH];
								memset(path, 0, MAX_PATH * sizeof(WCHAR));
								DWORD plen = GetModuleFileNameW(NULL, path, MAX_PATH);
								if (plen == 0)
									return __LINE__;
								size_t i = MAX_PATH;
								while (path[--i] != L'\\');
								path[i] = L'\0';

								std::wstring fullBatFilePath = path + std::wstring(L"\\ffconsole_play.bat");

								WCHAR cmdPathW[MAX_PATH];
								int resCmd = SetCmdExePath(cmdPathW);
								if (resCmd != 0)
								{
									if (fp_log) fwprintf(fp_log, L"Error: %s - %s\n", wstrDownloadDir.c_str(), L"SetCmdExePath error");
									if (fp_log) fclose(fp_log);
									if (!fp_log) MessageBoxW(NULL, wstrDownloadDir.c_str(), L"SetCmdExePath error", MB_ICONERROR);
									exit(__LINE__);
								}

								DWORD dwCreationFlags = 0;
								DWORD waitRes = 0;
								DWORD dwExitCode = 0;

								STARTUPINFOW si;
								PROCESS_INFORMATION pi;

								::ZeroMemory(&si, sizeof(si));
								si.cb = sizeof(si);
								::ZeroMemory(&pi, sizeof(pi));

								wchar_t wbuf[65536];
								int p = swprintf(wbuf, 65535, L"/C \"\"%s\" \"%s\"\"", fullBatFilePath.c_str(), g_biggest_movie_path.c_str());
								if ((p > 65530) || (p < 3))
								{
									if (fp_log) fwprintf(fp_log, L"Error: %s - %s\n", g_biggest_movie_path.c_str(), L"wprintf error");
									if (fp_log) fclose(fp_log);
									if (!fp_log) MessageBoxW(NULL, g_biggest_movie_path.c_str(), L"wprintf error", MB_ICONERROR);
									exit(__LINE__);
								}

								wprintf(L"\r\n%s\n", wbuf);

								// Start the child process. 
								if (CreateProcessW(
									cmdPathW   // Module name (TODO: alternate data stream welcome)
									, wbuf        // Command line TODO: host, port, weak-encrypted-sha256Image
									, NULL           // Process handle not inheritable
									, NULL           // Thread handle not inheritable
									, FALSE          // Set handle inheritance to FALSE
									, dwCreationFlags              // No creation flags
									, NULL           // Use parent's environment block
									, NULL		// Use parent's starting directory 
									, &si            // Pointer to STARTUPINFO structure
									, &pi)           // Pointer to PROCESS_INFORMATION structure
									)
								{
									CloseHandle(pi.hProcess);
									CloseHandle(pi.hThread);
									g_bPlayerAlreadyStarted = true;
								}
								else
								{
									if (fp_log) fwprintf(fp_log, L"Error: %s - %s\n", g_biggest_movie_path.c_str(), L"CreateProcessW error");
									if (fp_log) fclose(fp_log);
									if (!fp_log) MessageBoxW(NULL, g_biggest_movie_path.c_str(), L"CreateProcessW error", MB_ICONERROR);
									exit(__LINE__);
								}
							}
						}
					}
				}
			}

			// if we receive the finished alert or an error, we're done
			if (lt::alert_cast<lt::torrent_finished_alert>(a)) {
				h.save_resume_data();
				goto done;
			}

			if (lt::alert_cast<lt::torrent_error_alert>(a)) {
				std::cout << a->message() << std::endl;
				goto done;
			}

			// when resume data is ready, save it
			if (auto rd = lt::alert_cast<lt::save_resume_data_alert>(a)) {
				std::ofstream of(".resume_file", std::ios_base::binary);
				of.unsetf(std::ios_base::skipws);
				auto const b = write_resume_data_buf(rd->params);
				of.write(b.data(), b.size());

				if (piece_downloaded_vec.size() > 0)
				{
					int wr = WriteBinaryW(L"piece_downloaded_vec.dat", (const unsigned char*)&piece_downloaded_vec[0], piece_downloaded_vec.size() * sizeof(char));
					if (wr != 0) {
						if (fp_log) fwprintf(fp_log, L"Error: %s - %s\n", L"piece_downloaded_vec.dat", L"WriteBinaryW error");
						if (fp_log) fclose(fp_log);
						if (!fp_log) MessageBoxW(NULL, L"piece_downloaded_vec.dat", L"WriteBinaryW error", MB_ICONERROR);
						exit(__LINE__);
					}
				}
			}

			if (auto st = lt::alert_cast<lt::state_update_alert>(a)) {
				if (st->status.empty()) continue;

				// we only have a single torrent, so we know which one
				// the status is for
				lt::torrent_status const& s = st->status[0];
				std::cout << "\r" << state(s.state) << " "
					<< (s.download_payload_rate / 1000) << " kB/s "
					<< (s.total_done / 1000) << " kB ("
					<< (s.progress_ppm / 10000) << "%) downloaded";
				std::cout.flush();
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(500));

		// ask the session to post a state_update_alert, to update our
		// state output for the torrent
		ses.post_torrent_updates();

		// save resume data once every 30 seconds
		if (clk::now() - last_save_resume > std::chrono::seconds(30)) {
			h.save_resume_data();
			last_save_resume = clk::now();
		}
	}

	// TODO: ideally we should save resume data here

done:
	std::cout << "\ndone, shutting down" << std::endl;
	exit(0);
}
catch (std::exception & e)
{
	std::cerr << "Error: " << e.what() << std::endl;
	if (fp_log) fprintf(fp_log, "Error: %s - std::exception: %s\n", strMagnetOrTorrent.c_str(), e.what());
	if (fp_log) fclose(fp_log);
	if (!fp_log) MessageBoxA(NULL, e.what(), "std::exception", MB_ICONERROR);
	exit(__LINE__);
}

