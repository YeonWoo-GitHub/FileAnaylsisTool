#include "pch.h"
#include "AnaylsisMainDlg.h"
#include "FindTextDlg.h"
#include "resource.h"
#include "Policy.h"

int g_szListViewSize[] = { 40, 100, 250, 40, 250, 40, 350 };
LPWSTR g_szListViewText[] = { L"NUM", L"MODULEFILE_NAME", L"MODULEFILE_PATH", L"EXIST", L"ENVIRONMENT_VARIABLE_PATH", L"EXIST", L"EXP" };

AnaylsisMainDlg* AnaylsisMainDlg::getInstance()
{
	static AnaylsisMainDlg inst;
	return &inst;
}

AnaylsisMainDlg::AnaylsisMainDlg(void)
{
	SecureZeroMemory(m_szCSVFilePath, sizeof(m_szCSVFilePath));
	SecureZeroMemory(m_szSave_CSVFile, sizeof(m_szSave_CSVFile));
	SecureZeroMemory(m_szMsgTitle, sizeof(m_szMsgTitle));
	m_bAscending = FALSE;

	m_Wndrect = { 0, };

	m_hHeader = 0;
	m_hListHWnd = 0;
	m_hMainDlg = 0;
	m_hDropInfo = 0;

	m_Sizecx = 0;
	m_Sizecy = 0;

	m_Hok = 0;
	m_hIcon = 0;
}

AnaylsisMainDlg::~AnaylsisMainDlg(void)
{
}

int AnaylsisMainDlg::ConfigManagementFile_Analysis_Dialog(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	DialogBoxParam(hInstance, MAKEINTRESOURCEW(IDD_DIALOG_CCISFILE), NULL, DlgProc, (LPARAM)L"");

	return ERROR_SUCCESS;
}

INT_PTR CALLBACK AnaylsisMainDlg::DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_INITDIALOG:
	{
		AnaylsisMainDlg::getInstance()->InitDialog(hDlg);
	}
	break;

	case WM_GETMINMAXINFO:
	{
		MINMAXINFO* mmi = (MINMAXINFO*)lParam;
		mmi->ptMinTrackSize.x = AnaylsisMainDlg::getInstance()->m_Sizecx;
		mmi->ptMinTrackSize.y = AnaylsisMainDlg::getInstance()->m_Sizecy;
	}
	break;

	case WM_SIZE:
	{
		if (AnaylsisMainDlg::getInstance()->m_hListHWnd)
		{
			GetClientRect(AnaylsisMainDlg::getInstance()->m_hMainDlg, (LPRECT)&AnaylsisMainDlg::getInstance()->m_Wndrect);
			SetWindowPos(AnaylsisMainDlg::getInstance()->m_hListHWnd, NULL, 0, 0, AnaylsisMainDlg::getInstance()->m_Wndrect.right, AnaylsisMainDlg::getInstance()->m_Wndrect.bottom, SWP_NOZORDER | SWP_NOMOVE | SWP_SHOWWINDOW);
		}
	}
	break;

	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case ID_FILE_OPEN:
		{
			AnaylsisMainDlg::getInstance()->OnOpenCSVFile(AnaylsisMainDlg::getInstance()->m_hMainDlg, AnaylsisMainDlg::getInstance()->m_hListHWnd);
		}
		break;
		case ID_FILE_SAVE:
		{
			AnaylsisMainDlg::getInstance()->OnFileSave(AnaylsisMainDlg::getInstance()->m_hMainDlg, AnaylsisMainDlg::getInstance()->m_hListHWnd);
		}
		break;
		case ID_FIND:
		{
			AnaylsisMainDlg::getInstance()->OnFindText();
		}
		break;
		case ID_Close:
		{
			UnhookWindowsHookEx(AnaylsisMainDlg::getInstance()->m_Hok);
			AnaylsisMainDlg::getInstance()->m_vecFile_Info.clear();
			EndDialog(hDlg, 0);
			return TRUE;
		}
		break;
		case IDCANCEL:
		{
			UnhookWindowsHookEx(AnaylsisMainDlg::getInstance()->m_Hok);
			AnaylsisMainDlg::getInstance()->m_vecFile_Info.clear();
			EndDialog(hDlg, 0);
			return TRUE;
		}
		}
	}
	break;

	case WM_DROPFILES:
	{
		AnaylsisMainDlg::getInstance()->m_hDropInfo = (HDROP)wParam;
		AnaylsisMainDlg::getInstance()->OnDropFiles(AnaylsisMainDlg::getInstance()->m_hMainDlg, AnaylsisMainDlg::getInstance()->m_hDropInfo, AnaylsisMainDlg::getInstance()->m_hListHWnd);
	}
	break;

	case WM_HOTKEY:
	{
		switch (wParam)
		{
		case FILEOPEN:
		{
			AnaylsisMainDlg::getInstance()->OnOpenCSVFile(AnaylsisMainDlg::getInstance()->m_hMainDlg, AnaylsisMainDlg::getInstance()->m_hListHWnd);
		}
		break;
		case FILESAVE:
		{
			AnaylsisMainDlg::getInstance()->OnFileSave(AnaylsisMainDlg::getInstance()->m_hMainDlg, AnaylsisMainDlg::getInstance()->m_hListHWnd);
		}
		break;
		case QUIT:
		{
			UnhookWindowsHookEx(AnaylsisMainDlg::getInstance()->m_Hok);
			AnaylsisMainDlg::getInstance()->m_vecFile_Info.clear();
			EndDialog(hDlg, 0);
		}
		break;
		case FINDTEXT:
		{
			AnaylsisMainDlg::getInstance()->OnFindText();
		}
		break;
		}

	}
	break;

	case WM_NOTIFY:
	{
		switch (((NMHDR*)lParam)->code)
		{
		case HDN_ITEMCLICK:
		{
			NMHEADER* lpnmhd = (NMHEADER*)lParam;
			AnaylsisMainDlg::getInstance()->OnHdnItemSortingList(lpnmhd);
		}
		break;
		case NM_DBLCLK:
		{
			NMHEADER* lpnmhd = (NMHEADER*)lParam;
			AnaylsisMainDlg::getInstance()->OnDblclkListToFolder(lpnmhd);
		}
		break;
		}
	}
	break;

	default:
	{
		return FALSE;
	}
	}
	return FALSE;
}

int AnaylsisMainDlg::InitDialog(HWND hDlg)
{
	if (0 == hDlg)
	{
		return ERROR_INVALID_PARAMETER;
	}

	int nRetval = 0;
	WCHAR szTitle[MAX_PATH] = { 0, };

	Policy::getInstance()->GetPolicy(MAIN_SECTION, DLG_TITLE, szTitle, MAX_PATH);

	nRetval = SetWindowText(hDlg, szTitle);

	if (0 == nRetval)
	{
		return ERROR_INVALID_FUNCTION;
	}

	Policy::getInstance()->GetPolicy(THIRD_SECTION, MSG_TITLE, m_szMsgTitle, MAX_PATH);

	//m_hIcon = (HICON)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDR_MAINFRAME), IMAGE_ICON, 16, 16, LR_DEFAULTSIZE);		// 아이콘 추가
	//SendMessage(hDlg, WM_SETICON, ICON_SMALL, (LPARAM)m_hIcon);

	::RegisterHotKey(hDlg, FILEOPEN, MOD_CONTROL, 'O');			////파일 열기 단축키
	::RegisterHotKey(hDlg, FILESAVE, MOD_CONTROL, 'S');			////파일 저장 단축키
	::RegisterHotKey(hDlg, QUIT, MOD_CONTROL, 'Q');				////종료 단축키
	::RegisterHotKey(hDlg, FINDTEXT, MOD_CONTROL, 'F');			////찾기 단축키

	m_hMainDlg = hDlg;
	m_hListHWnd = GetDlgItem(m_hMainDlg, IDC_LIST_CCISFILE);
	m_hHeader = ListView_GetHeader(m_hListHWnd);
	m_Hok = SetWindowsHookEx(WH_KEYBOARD, (HOOKPROC)KeybdProc, NULL, GetCurrentThreadId());

	int nRetval2 = ERROR_INVALID_FUNCTION;

	nRetval2 = ListView_Init(m_hListHWnd);

	if (ERROR_INVALID_FUNCTION == nRetval2)
	{
		return ERROR_INVALID_FUNCTION;
	}

	GetWindowRect(m_hMainDlg, &m_Wndrect);
	m_Sizecx = m_Wndrect.right - m_Wndrect.left;
	m_Sizecy = m_Wndrect.bottom - m_Wndrect.top;

	return ERROR_SUCCESS;
}

int AnaylsisMainDlg::ListView_Init(HWND hListView)
{
	if (0 == hListView)
		return ERROR_INVALID_PARAMETER;

	LVCOLUMN COL;
	ListView_SetExtendedListViewStyle(hListView, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	COL.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	COL.fmt = LVCFMT_LEFT;

	WCHAR szText[MAX_PATH] = { 0, };

	for (int i = 0; i < 7; i++)
	{
		SecureZeroMemory(szText, sizeof(szText));
		Policy::getInstance()->GetPolicy(MAIN_SECTION, g_szListViewText[i], szText, MAX_PATH);

		COL.cx = g_szListViewSize[i];
		COL.pszText = szText;
		COL.iSubItem = i;
		ListView_InsertColumn(hListView, i, &COL);
	}

	return ERROR_SUCCESS;
}

int AnaylsisMainDlg::OnDropFiles(HWND hMainDlg, HDROP hDropInfo, HWND hListView)
{
	if (0 == hMainDlg || 0 == hDropInfo || 0 == hListView)
	{
		return ERROR_INVALID_PARAMETER;
	}

	int nFiles = 0;
	int nRetval = ERROR_INVALID_FUNCTION;
	int nRetval2 = 0;
	wstring szFileExtension = L"";

	nFiles = DragQueryFile(hDropInfo, 0xFFFFFFFF, m_szCSVFilePath, MAX_PATH);

	if (1 < nFiles)
	{
		MsgBox(hMainDlg, MSG_SELECT_ONE_FILE, MB_ICONERROR);
		return ERROR_INVALID_FUNCTION;
	}

	nRetval2 = DragQueryFile(hDropInfo, 0, m_szCSVFilePath, MAX_PATH);

	if (0 == nRetval2)
	{
		DragFinish(hDropInfo);
		return ERROR_INVALID_FUNCTION;
	}

	szFileExtension = PathFindExtension(m_szCSVFilePath);

	if (L".csv" == szFileExtension)
	{
		WCHAR szTitle[MAX_PATH] = { 0, };
		WCHAR szText[MAX_PATH] = { 0, };		

		wcscpy_s(szTitle, PathFindFileName(m_szCSVFilePath));
		Policy::getInstance()->GetPolicy(MAIN_SECTION, TITLE_STATE_STRING, szText, MAX_PATH);

		swprintf_s(szTitle, L"%s%s", szTitle, szText);

		SetWindowText(hMainDlg, szTitle);
		nRetval = InteriorLogicFunc(hMainDlg, hListView);

		if (ERROR_INVALID_FUNCTION == nRetval)
		{
			MsgBox(hMainDlg, MSG_ANALYSIS_FILE_FAIL, MB_ICONERROR);			
			DragFinish(hDropInfo);
			return ERROR_INVALID_FUNCTION;
		}
	}
	else
	{
		MsgBox(hMainDlg, MSG_SELECT_FILE, MB_ICONERROR);		
		DragFinish(hDropInfo);
		return ERROR_INVALID_FUNCTION;
	}

	DragFinish(hDropInfo);
	return ERROR_SUCCESS;
}

int AnaylsisMainDlg::OnOpenCSVFile(HWND hMainDlg, HWND hListView)
{
	if (0 == hMainDlg || 0 == hListView)
		return ERROR_INVALID_PARAMETER;

	int nRetval = ERROR_INVALID_FUNCTION;
	OPENFILENAME OFN = { 0, };

	OFN.lStructSize = sizeof(OFN);
	OFN.hwndOwner = hMainDlg;
	OFN.lpstrFile = m_szCSVFilePath;
	OFN.nMaxFile = sizeof(m_szCSVFilePath);
	OFN.lpstrFilter = L"CSV\0*.CSV\0";
	OFN.nFilterIndex = 1;
	OFN.lpstrFileTitle = NULL;
	OFN.nMaxFileTitle = 0;
	OFN.lpstrInitialDir = NULL;
	OFN.lpstrDefExt = L"csv";
	OFN.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (TRUE == GetOpenFileName(&OFN))
	{
		WCHAR szFile_Extension[MAX_PATH] = { 0, };
		wcscpy_s(szFile_Extension, ::PathFindExtension(m_szCSVFilePath));

		if (0 == _wcsicmp(szFile_Extension, L".csv"))
		{
			WCHAR szTitle[MAX_PATH] = { 0, };
			WCHAR szText[MAX_PATH] = { 0, };

			wcscpy_s(szTitle, PathFindFileName(m_szCSVFilePath));
			Policy::getInstance()->GetPolicy(MAIN_SECTION, TITLE_STATE_STRING, szText, MAX_PATH);

			swprintf_s(szTitle, L"%s%s", szTitle, szText);

			SetWindowText(hMainDlg, szTitle);

			nRetval = InteriorLogicFunc(hMainDlg, hListView);

			if (ERROR_INVALID_FUNCTION == nRetval)
			{
				MsgBox(hMainDlg, MSG_ANALYSIS_FILE_FAIL, MB_ICONERROR);

				return ERROR_INVALID_FUNCTION;
			}
			else
			{
				return ERROR_SUCCESS;
			}
		}
		else
		{
			MsgBox(hMainDlg, MSG_SELECT_FILE, MB_ICONERROR);

			return ERROR_INVALID_FUNCTION;
		}
	}
	return ERROR_INVALID_FUNCTION;

}

int AnaylsisMainDlg::InteriorLogicFunc(HWND hMainDlg, HWND hListView)
{
	if (0 == hMainDlg || 0 == hListView)
		return ERROR_INVALID_PARAMETER;

	int nRetval = ERROR_INVALID_FUNCTION;

	if (m_vecSystemEnvPath_Info.size() == 0) {
		nRetval = Read_SystemEnvironment(hMainDlg, &m_vecSystemEnvPath_Info);
		if (ERROR_SUCCESS != nRetval)
		{
			return ERROR_INVALID_FUNCTION;
		}
	}
	m_vecFile_Info.clear();

	nRetval = Read_CSVFile(hMainDlg, m_szCSVFilePath, &m_vecFile_Info);

	if (ERROR_SUCCESS != nRetval)
		return ERROR_INVALID_FUNCTION;

	int szFileInfo_Size = m_vecFile_Info.size();

	for (int i = 0; i < szFileInfo_Size; i++)
	{
		nRetval = Verification_CSVFile(hMainDlg, &m_vecFile_Info[i]);

		if (ERROR_SUCCESS != nRetval)
		{
			return ERROR_INVALID_FUNCTION;
		}

		nRetval = Verification_SystemEnvironment(&m_vecSystemEnvPath_Info, &m_vecFile_Info[i]);

		if (ERROR_SUCCESS != nRetval)
		{
			return ERROR_INVALID_FUNCTION;
		}
	}

	ListView_DeleteAllItems(hListView);
	nRetval = ListView_Print(hListView, &m_vecFile_Info);

	if (ERROR_SUCCESS != nRetval)
		return ERROR_INVALID_FUNCTION;

	return ERROR_SUCCESS;
}

int AnaylsisMainDlg::GetFileEncoding(HWND hMainDlg, WCHAR* pCSVFilePath)
{
	if (INVALID_HANDLE_VALUE == pCSVFilePath)
		return ERROR_INVALID_PARAMETER;

	if (0 == hMainDlg)
		return ERROR_INVALID_PARAMETER;

	unsigned char uniTxt[] = { 0xFF, 0xFE };
	unsigned char utf8Txt[] = { 0xEF, 0xBB };	//// UTF_8 file header

	HANDLE hFile = 0;
	DWORD Encoding = 0;
	DWORD dwBytesRead = 0;
	BOOL bRetVal = FALSE;
	BYTE* lpHeader = new(std::nothrow)BYTE[10];

	if (NULL == lpHeader)
		return ERROR_INVALID_FUNCTION;

	SecureZeroMemory(lpHeader, 10);

	hFile = CreateFile(pCSVFilePath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE != hFile)
	{
		bRetVal = ReadFile(hFile, lpHeader, 10, &dwBytesRead, NULL);

		if (TRUE == bRetVal)
		{
			if (NULL != lpHeader && (lpHeader[0] == uniTxt[0] && lpHeader[1] == uniTxt[1]))
			{
				Encoding = ENCODING_UNICODE;
			}
			else if (NULL != lpHeader && (lpHeader[0] == utf8Txt[0] && lpHeader[1] == utf8Txt[1]))
			{			
				MsgBox(hMainDlg, MSG_SELECT_FILE_WITH_ENCODING, MB_ICONERROR);				
			}
			else
			{
				Encoding = ENCODING_ANSI;
			}

			if (NULL != lpHeader)
			{
				delete[]lpHeader;
			}

			if (NULL != hFile)
			{
				CloseHandle(hFile);
			}

			return Encoding;
		}

		if (NULL != hFile)
		{
			CloseHandle(hFile);
		}

	}

	if (NULL != lpHeader)
		delete[]lpHeader;

	return ERROR_INVALID_FUNCTION;
}

int AnaylsisMainDlg::Read_CSVFile(HWND hMainDlg, WCHAR* pCSVFilePath, std::vector<File_Info>* pdInfo)
{
	if (0 == hMainDlg || NULL == pdInfo)
	{
		return ERROR_INVALID_PARAMETER;
	}

	if (0 != _waccess_s(pCSVFilePath, 4))
	{
		return ERROR_INVALID_DATA;
	}

	DWORD Encoding = GetFileEncoding(hMainDlg, pCSVFilePath);

	if (ERROR_INVALID_FUNCTION == Encoding)
	{
		return ERROR_INVALID_DATA;
	}

	WCHAR szWinPath[MAX_PATH] = { 0, };
	int nRet = GetWindowsDirectory(szWinPath, MAX_PATH);

	if (0 == nRet)
	{
		return ERROR_INVALID_FUNCTION;
	}

	_wsetlocale(LC_ALL, L"Korean");

	BOOL nR = FALSE;
	nR = PathFileExists(pCSVFilePath);

	HANDLE hFile = NULL;
	hFile = CreateFile(pCSVFilePath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	int nG = ::GetLastError();

	if (INVALID_HANDLE_VALUE == hFile)
	{
		CloseHandle(hFile);
		return ERROR_TOO_MANY_OPEN_FILES;
	}

	int nFileSize = GetFileSize(hFile, NULL);

	if (INVALID_FILE_SIZE == nFileSize)
	{
		CloseHandle(hFile);
		return ERROR_TOO_MANY_OPEN_FILES;
	}

	DWORD dwRead = 0;

	LPSTR sz_ANSI_Content = new(std::nothrow)CHAR[nFileSize];
	if (NULL == sz_ANSI_Content)
		return ERROR_INVALID_FUNCTION;

	LPWSTR sz_UNICODE_Text = new(std::nothrow)WCHAR[nFileSize];

	if (NULL == sz_UNICODE_Text)
		return ERROR_INVALID_FUNCTION;

	memset(sz_ANSI_Content, 0, nFileSize * sizeof(CHAR));
	memset(sz_UNICODE_Text, 0, nFileSize * sizeof(WCHAR));

	if (ENCODING_UNICODE == Encoding)
	{
		BOOL bRetval = ReadFile(hFile, sz_UNICODE_Text, nFileSize, &dwRead, NULL);
		if (FALSE == bRetval)
		{
			return ERROR_TOO_MANY_OPEN_FILES;
		}

		WCHAR* pLongLine = NULL;
		WCHAR* pLongNextLine = NULL;
		vector<WCHAR*>szTok;
		File_Info File_Info = { 0, };

		pLongLine = wcstok_s(sz_UNICODE_Text, L"\n", &pLongNextLine);
		while (pLongLine)
		{
			if (ERROR_SUCCESS == FileDataCollect(pLongLine))
			{
				wcscpy_s(File_Info.File_Name, m_FileData.at(0).c_str());
				m_FileData.at(2)[0] = szWinPath[0];
				wcscpy_s(File_Info.CSVInfo_FilePath, m_FileData.at(2).c_str());
				if (L"O" == m_FileData.at(10))
					File_Info.FileVersion_Flag = BIT_64;
				else
					File_Info.FileVersion_Flag = BIT_32;

				pdInfo->push_back(File_Info);
			}
			pLongLine = wcstok_s(NULL, L"\n", &pLongNextLine);
		}
		CloseHandle(hFile);
		delete[]sz_ANSI_Content;
		delete[]sz_UNICODE_Text;
	}

	else if (ENCODING_ANSI == Encoding)
	{
		BOOL bRetval = ReadFile(hFile, (LPVOID)sz_ANSI_Content, nFileSize, &dwRead, NULL);
		if (TRUE == bRetval)
		{
			MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, sz_ANSI_Content, -1, sz_UNICODE_Text, nFileSize);

			WCHAR* pLongLine = NULL;
			WCHAR* pLongNextLine = NULL;
			File_Info File_Info = { 0, };

			pLongLine = wcstok_s(sz_UNICODE_Text, L"\n", &pLongNextLine);
			while (pLongLine)
			{
				if (ERROR_SUCCESS == FileDataCollect(pLongLine))
				{
					wcscpy_s(File_Info.File_Name, m_FileData.at(0).c_str());
					m_FileData.at(2)[0] = szWinPath[0];
					wcscpy_s(File_Info.CSVInfo_FilePath, m_FileData.at(2).c_str());
					if (L"O" == m_FileData.at(10))
						File_Info.FileVersion_Flag = BIT_64;
					else
						File_Info.FileVersion_Flag = BIT_32;

					pdInfo->push_back(File_Info);
				}
				pLongLine = wcstok_s(NULL, L"\n", &pLongNextLine);
			}

		}
		CloseHandle(hFile);
		delete[]sz_ANSI_Content;
		delete[]sz_UNICODE_Text;
	}

	return ERROR_SUCCESS;
}

int AnaylsisMainDlg::Read_SystemEnvironment(HWND hMainDlg, std::vector<wstring>* pSystemEnv)
{
	if (0 == hMainDlg || NULL == pSystemEnv)
		return ERROR_INVALID_PARAMETER;

	WCHAR SystemEnvName[4096] = { 0, };
	DWORD dwRet = 0x00;
	DWORD dwErr = 0x00;

	dwRet = GetEnvironmentVariable(L"PATH", SystemEnvName, _countof(SystemEnvName));

	if (0 == dwRet)
	{
		return ERROR_ENVVAR_NOT_FOUND;
	}
	else if (READ_BUFSIZE < dwRet)
	{
		return ERROR_NOT_ENOUGH_MEMORY;
	}
	else
	{
		WCHAR* szSysEnvTok = NULL;
		WCHAR* pNextLine = NULL;

		_wcsupr_s(SystemEnvName);
		szSysEnvTok = wcstok_s(SystemEnvName, L";", &pNextLine);

		while (szSysEnvTok)
		{
			if (NULL == StrStrIW(szSysEnvTok, L"softcamp"))
			{
				pSystemEnv->push_back(szSysEnvTok);
			}
			szSysEnvTok = wcstok_s(NULL, L";", &pNextLine);
		}
		if (0 == pSystemEnv->size())
		{
			return ERROR_INVALID_DATA;
		}
	}
	return ERROR_SUCCESS;
}

int AnaylsisMainDlg::Verification_CSVFile(HWND hMainDlg, File_Info* pdInfo)
{
	if (0 == hMainDlg || NULL == pdInfo)
	{
		return ERROR_INVALID_PARAMETER;
	}

	if (NULL == pdInfo->CSVInfo_FilePath && NULL == pdInfo->File_Name)
	{
		return ERROR_INVALID_DATA;
	}

	WCHAR szRealFile_Path[MAX_PATH] = { 0, };
	WCHAR szTok[MAX_PATH] = L"SYSTEM32";
	HANDLE hFileExists = NULL;
	PVOID OldValue = NULL;
	BOOL bIsWow64 = FALSE;

	PathCombine(szRealFile_Path, pdInfo->CSVInfo_FilePath, pdInfo->File_Name);
	if (0 == szRealFile_Path)
	{
		return ERROR_INVALID_DATA;
	}

	IsWow64Process(GetCurrentProcess(), &bIsWow64);

	if (TRUE == bIsWow64)
	{
		if (BIT_64 == pdInfo->FileVersion_Flag)
		{
			if (NULL != StrStrIW(pdInfo->CSVInfo_FilePath, L"syswow64"))
			{
				hFileExists = CreateFile(szRealFile_Path, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
				if (INVALID_HANDLE_VALUE != hFileExists)
				{
					pdInfo->File_State = INCORRECT_FILEPATH;
				}
				else
				{
					pdInfo->File_State = NON_STATIONARY_FILEPATH;
				}

				if (NULL != hFileExists)
				{
					CloseHandle(hFileExists);
					hFileExists = NULL;
				}

				return ERROR_SUCCESS;
			}
			else if (TRUE == Wow64DisableWow64FsRedirection(&OldValue))	
			{
				hFileExists = CreateFile(szRealFile_Path, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
				Wow64RevertWow64FsRedirection(OldValue);
				if (INVALID_HANDLE_VALUE != hFileExists)
				{
					pdInfo->File_State = CORRECT_FILEPATH;
				}
				else
				{
					pdInfo->File_State = NO_FILE;
				}

				if (NULL != hFileExists)
				{
					CloseHandle(hFileExists);
					hFileExists = NULL;
				}
				return ERROR_SUCCESS;
			}
		}
		else
		{
			if (NULL != StrStrIW(pdInfo->CSVInfo_FilePath, L"system32"))
			{
				if (TRUE == Wow64DisableWow64FsRedirection(&OldValue))
				{
					hFileExists = CreateFile(szRealFile_Path, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
					Wow64RevertWow64FsRedirection(OldValue);

					if (INVALID_HANDLE_VALUE != hFileExists)
					{
						pdInfo->File_State = INCORRECT_FILEPATH;
					}
					else
					{
						pdInfo->File_State = NON_STATIONARY_FILEPATH;
					}
				}
				if (NULL != hFileExists)
				{
					CloseHandle(hFileExists);
					hFileExists = NULL;
				}
				return ERROR_SUCCESS;
			}

			// syswow64 경로
			hFileExists = CreateFile(szRealFile_Path, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

			if (INVALID_HANDLE_VALUE != hFileExists)
			{
				pdInfo->File_State = CORRECT_FILEPATH;
			}
			else
			{
				pdInfo->File_State = NO_FILE;
			}

			if (NULL != hFileExists)
			{
				CloseHandle(hFileExists);
				hFileExists = NULL;
			}
			return ERROR_SUCCESS;
		}
	}
	else
	{
		if (BIT_32 == pdInfo->FileVersion_Flag)
		{
			if (NULL != StrStrIW(pdInfo->CSVInfo_FilePath, L"syswow64"))
			{
				WCHAR szSYSWOW64_Path[MAX_PATH] = { 0, };
				wcscpy_s(szSYSWOW64_Path, szRealFile_Path);

				int Num = wcslen(szTok);
				for (int j = 0; j < Num; j++)
					szSYSWOW64_Path[11 + j] = szTok[j];

				hFileExists = CreateFile(szSYSWOW64_Path, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

				if (INVALID_HANDLE_VALUE != hFileExists)
				{
					pdInfo->File_State = CORRECT_FILEPATH;
				}
				else
				{
					pdInfo->File_State = NO_FILE;
				}

				if (NULL != hFileExists)
				{
					CloseHandle(hFileExists);
					hFileExists = NULL;
				}
				return ERROR_SUCCESS;
			}
			else
			{
				hFileExists = CreateFile(szRealFile_Path, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

				if (INVALID_HANDLE_VALUE != hFileExists)
				{
					pdInfo->File_State = CORRECT_FILEPATH;
				}
				else
				{
					pdInfo->File_State = NO_FILE;
				}
				if (NULL != hFileExists)
				{
					CloseHandle(hFileExists);
					hFileExists = NULL;
				}
				return ERROR_SUCCESS;
			}
		}
	}
	return ERROR_INVALID_FUNCTION;
}

int AnaylsisMainDlg::Verification_SystemEnvironment(std::vector<wstring>* pSystemEnv, File_Info* pdInfo)
{
	if (NULL == pSystemEnv || NULL == pdInfo)
		return ERROR_INVALID_PARAMETER;

	if (0 == wcslen(pdInfo->File_Name))
		return ERROR_INVALID_DATA;

	int SysEnv_Size = pSystemEnv->size();
	BOOL bIsWow64 = FALSE;

	IsWow64Process(GetCurrentProcess(), &bIsWow64);

	if (TRUE == bIsWow64)
	{
		if (BIT_64 == pdInfo->FileVersion_Flag)
		{
			for (int i = 0; i < SysEnv_Size; i++)
			{
				WCHAR szNRealFile_Path[MAX_PATH] = { 0, };
				WCHAR szTok[MAX_PATH] = L"SYSWOW64";
				HANDLE hFileExists = NULL;
				PVOID OldValue = NULL;

				PathCombine(szNRealFile_Path, pSystemEnv->at(i).c_str(), pdInfo->File_Name);

				if (0 == szNRealFile_Path)
				{
					return ERROR_INVALID_DATA;
				}

				if (NULL != StrStrIW(szNRealFile_Path, L"system32"))
				{
					if (TRUE != Wow64DisableWow64FsRedirection(&OldValue))
					{
						return ERROR_INVALID_FUNCTION;
					}

					hFileExists = CreateFile(szNRealFile_Path, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
					Wow64RevertWow64FsRedirection(OldValue);

					if (INVALID_HANDLE_VALUE != hFileExists)
					{
						if (0 != _wcsicmp(pSystemEnv->at(i).c_str(), pdInfo->CSVInfo_FilePath))
						{
							if (INCORRECT_FILEPATH == pdInfo->File_State || NON_STATIONARY_FILEPATH == pdInfo->File_State)
							{
								pdInfo->FileVer_CollectSysEnv_FilePath.push_back(pSystemEnv->at(i).c_str());
								pdInfo->FileVer_CollectSysEnv_State = CORRECT_FILEPATH;
							}
							else
							{
								pdInfo->SysEnv_FilePath.push_back(pSystemEnv->at(i).c_str());
								if (INCORRECT_FILEPATH != pdInfo->SysEnv_State)
								{
									pdInfo->SysEnv_State = INCORRECT_FILEPATH;

								}
							}
						}

						if (NULL != hFileExists)
						{
							CloseHandle(hFileExists);
							hFileExists = NULL;
						}

					}

					WCHAR szSYSWOW64_Path[MAX_PATH] = { 0, };
					wcscpy_s(szSYSWOW64_Path, szNRealFile_Path);

					int Num = wcslen(szTok);
					for (int j = 0; j < Num; j++)
					{
						szSYSWOW64_Path[11 + j] = szTok[j];
					}

					hFileExists = CreateFile(szSYSWOW64_Path, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

					if (INVALID_HANDLE_VALUE != hFileExists)
					{
						int FileName_len = wcslen(szNRealFile_Path) - wcslen(pdInfo->File_Name) - 1;
						wcsncpy_s(szSYSWOW64_Path, szSYSWOW64_Path, FileName_len);
						if (0 != _wcsicmp(szSYSWOW64_Path, pdInfo->CSVInfo_FilePath))
						{
							if (INCORRECT_FILEPATH == pdInfo->File_State || NON_STATIONARY_FILEPATH == pdInfo->File_State)
							{
								pdInfo->FileVer_InCollectSysEnv_FilePath.push_back(szSYSWOW64_Path);
								pdInfo->FileVer_InCollectSysEnv_State = INCORRECT_FILEPATH;
							}
							else
							{
								if (INCORRECT_FILEPATH != pdInfo->SysEnv_State)
								{
									pdInfo->SysEnv_State = INCORRECT_FILEPATH;
								}
								pdInfo->SysEnv_FilePath.push_back(szSYSWOW64_Path);
							}
						}
					}
					if (NULL != hFileExists)
					{
						CloseHandle(hFileExists);
						hFileExists = NULL;
					}
				}

				else
				{
					hFileExists = CreateFile(szNRealFile_Path, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
					if (INVALID_HANDLE_VALUE != hFileExists)
					{
						if (0 != _wcsicmp(pSystemEnv->at(i).c_str(), pdInfo->CSVInfo_FilePath))
						{
							pdInfo->SysEnv_FilePath.push_back(pSystemEnv->at(i).c_str());

							if (INCORRECT_FILEPATH != pdInfo->SysEnv_State)
							{
								pdInfo->SysEnv_State = INCORRECT_FILEPATH;

							}
						}

						if (NULL != hFileExists)
						{
							CloseHandle(hFileExists);
							hFileExists = NULL;
						}
					}
				}
			}
			if (0 == pdInfo->SysEnv_State && 0 == pdInfo->FileVer_CollectSysEnv_State && 0 == pdInfo->FileVer_InCollectSysEnv_State)
			{
				pdInfo->SysEnv_State = NO_FILE;
			}
			return ERROR_SUCCESS;
		}

		else
		{
			for (int i = 0; i < SysEnv_Size; i++)
			{
				WCHAR szNRealDLLFile_Path[MAX_PATH] = { 0, };
				WCHAR szTok[MAX_PATH] = L"SYSWOW64";
				HANDLE hFileExists = NULL;
				PVOID OldValue = NULL;

				PathCombine(szNRealDLLFile_Path, pSystemEnv->at(i).c_str(), pdInfo->File_Name);

				if (0 == szNRealDLLFile_Path)
				{
					return ERROR_INVALID_DATA;
				}

				if (NULL != StrStrIW(szNRealDLLFile_Path, L"system32"))
				{
					if (TRUE != Wow64DisableWow64FsRedirection(&OldValue))
					{
						return ERROR_INVALID_FUNCTION;
					}

					hFileExists = CreateFile(szNRealDLLFile_Path, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
					Wow64RevertWow64FsRedirection(OldValue);

					if (INVALID_HANDLE_VALUE != hFileExists)
					{
						if (0 != _wcsicmp(pSystemEnv->at(i).c_str(), pdInfo->CSVInfo_FilePath))
						{
							if (INCORRECT_FILEPATH == pdInfo->File_State || NON_STATIONARY_FILEPATH == pdInfo->File_State)
							{
								pdInfo->FileVer_InCollectSysEnv_FilePath.push_back(pSystemEnv->at(i).c_str());
								pdInfo->FileVer_InCollectSysEnv_State = INCORRECT_FILEPATH;
							}
							else
							{
								pdInfo->SysEnv_FilePath.push_back(pSystemEnv->at(i).c_str());
								pdInfo->SysEnv_State = INCORRECT_FILEPATH;
							}
						}

						if (NULL != hFileExists)
						{
							CloseHandle(hFileExists);
							hFileExists = NULL;
						}


					}

					WCHAR szSYSWOW64_Path[MAX_PATH] = { 0, };
					wcscpy_s(szSYSWOW64_Path, szNRealDLLFile_Path);

					int Num = wcslen(szTok);
					for (int j = 0; j < Num; j++)
					{
						szSYSWOW64_Path[11 + j] = szTok[j];
					}

					hFileExists = CreateFile(szSYSWOW64_Path, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

					if (INVALID_HANDLE_VALUE != hFileExists)
					{
						int FileName_len = wcslen(szNRealDLLFile_Path) - wcslen(pdInfo->File_Name) - 1;
						wcsncpy_s(szSYSWOW64_Path, szSYSWOW64_Path, FileName_len);
						if (0 != _wcsicmp(szSYSWOW64_Path, pdInfo->CSVInfo_FilePath))
						{
							if (INCORRECT_FILEPATH == pdInfo->File_State || NON_STATIONARY_FILEPATH == pdInfo->File_State)
							{
								pdInfo->FileVer_CollectSysEnv_FilePath.push_back(szSYSWOW64_Path);
								pdInfo->FileVer_CollectSysEnv_State = CORRECT_FILEPATH;
							}
							else
							{
								pdInfo->SysEnv_State = INCORRECT_FILEPATH;
								pdInfo->SysEnv_FilePath.push_back(szSYSWOW64_Path);
							}
						}
						if (NULL != hFileExists)
						{
							CloseHandle(hFileExists);
							hFileExists = NULL;
						}


					}
				}

				else
				{
					hFileExists = CreateFile(szNRealDLLFile_Path, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
					if (INVALID_HANDLE_VALUE != hFileExists)
					{
						if (0 != _wcsicmp(pSystemEnv->at(i).c_str(), pdInfo->CSVInfo_FilePath))
						{
							pdInfo->SysEnv_FilePath.push_back(pSystemEnv->at(i).c_str());

							if (INCORRECT_FILEPATH != pdInfo->SysEnv_State)
							{
								pdInfo->SysEnv_State = INCORRECT_FILEPATH;
							}
						}

						if (hFileExists != hFileExists)
						{
							CloseHandle(hFileExists);
							hFileExists = NULL;
						}
					}
				}

			}if (0 == pdInfo->SysEnv_State && 0 == pdInfo->FileVer_CollectSysEnv_State && 0 == pdInfo->FileVer_InCollectSysEnv_State)
			{
				pdInfo->SysEnv_State = NO_FILE;
			}

			return ERROR_SUCCESS;
		}
	}

	else
	{
		if (BIT_32 == pdInfo->FileVersion_Flag)
		{
			for (int i = 0; i < SysEnv_Size; i++)
			{
				WCHAR szNRealDLLFile_Path[MAX_PATH] = { 0, };
				HANDLE hFileExists = NULL;

				PathCombine(szNRealDLLFile_Path, pSystemEnv->at(i).c_str(), pdInfo->File_Name);
				hFileExists = CreateFile(szNRealDLLFile_Path, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

				if (INVALID_HANDLE_VALUE != hFileExists)
				{
					if (0 != _wcsicmp(pSystemEnv->at(i).c_str(), pdInfo->CSVInfo_FilePath))
					{
						pdInfo->SysEnv_FilePath.push_back(pSystemEnv->at(i).c_str());
						pdInfo->SysEnv_State = INCORRECT_FILEPATH;
					}

					if (hFileExists != hFileExists)
					{
						CloseHandle(hFileExists);
						hFileExists = NULL;
					}

				}
			}

		}

		if (0 == pdInfo->SysEnv_State)
		{
			pdInfo->SysEnv_State = NO_FILE;
		}
		return ERROR_SUCCESS;
	}

	return ERROR_INVALID_FUNCTION;
}

int AnaylsisMainDlg::FileDataCollect(WCHAR* pFileLine)
{
	if (NULL == pFileLine)
		return ERROR_INVALID_PARAMETER;

	wstring szFileData = L"";
	wstring szNextData = L"";
	WCHAR szData[MAX_PATH] = { 0, };

	szFileData = std::wstring(pFileLine);
	if (L"" == szFileData)
	{
		return ERROR_INVALID_FUNCTION;
	}

	m_FileData.clear();

	szFileData.copy(szData, szFileData.find(L","));
	szNextData = szFileData.substr(szFileData.find(L","));

	if (L"" == szNextData)
	{
		return ERROR_INVALID_FUNCTION;
	}

	if (NULL != StrStrW(szData, L".dll"))
	{
		while (szData)
		{
			m_FileData.push_back(szData);
			memset(szData, 0, sizeof(szData));
			if (-1 == szNextData.find(L",", 1))
			{
				szNextData.copy(szData, szNextData.find(L"\n", 1) - 1, 1);
				wcsncpy_s(szData, szData, wcslen(szData) - 1);
				m_FileData.push_back(szData);
				break;
			}
			szNextData.copy(szData, szNextData.find(L",", 1) - 1, 1);
			szNextData = szNextData.substr(szNextData.find(L",", 1));
		}
		return ERROR_SUCCESS;
	}
	return ERROR_INVALID_FUNCTION;
}

int AnaylsisMainDlg::ListView_Print(HWND hListHWnd, vector<File_Info>* pdInfo)
{
	if (0 == hListHWnd || NULL == pdInfo)
		return ERROR_INVALID_PARAMETER;

	int File_Info_Size = pdInfo->size();
	WCHAR Number[10] = { 0, };
	WCHAR szExp[MAX_PATH] = { 0, };
	int nItemNum = 0;

	LVITEM LI;

	LI.mask = LVIF_TEXT;
	LI.iSubItem = 0;

	for (int i = 0; i < File_Info_Size; i++)
	{
		if (NON_STATIONARY_FILEPATH == pdInfo->at(i).File_State)
		{
			if (CORRECT_FILEPATH == pdInfo->at(i).FileVer_CollectSysEnv_State)
			{
				int InCorrect_FilePath_size = pdInfo->at(i).FileVer_CollectSysEnv_FilePath.size();
				for (int j = 0; j < InCorrect_FilePath_size; j++)
				{
					nItemNum = ListView_GetItemCount(hListHWnd);
					swprintf_s(Number, L"%d", nItemNum + 1);

					LI.iItem = nItemNum + 1;
					LI.pszText = Number;

					SecureZeroMemory(szExp, sizeof(szExp));
					Policy::getInstance()->GetPolicy(MAIN_SECTION, STRING_EXP1, szExp, MAX_PATH);

					int nNum = ListView_InsertItem(hListHWnd, &LI);
					ListView_SetItemText(hListHWnd, nNum, 1, pdInfo->at(i).File_Name);
					ListView_SetItemText(hListHWnd, nNum, 2, pdInfo->at(i).CSVInfo_FilePath);
					ListView_SetItemText(hListHWnd, nNum, 3, (LPWSTR)L"X");
					ListView_SetItemText(hListHWnd, nNum, 4, (LPWSTR)pdInfo->at(i).FileVer_CollectSysEnv_FilePath.at(j).c_str());
					ListView_SetItemText(hListHWnd, nNum, 5, (LPWSTR)L"O");
					ListView_SetItemText(hListHWnd, nNum, 6, szExp);
				}
			}
			else if (INCORRECT_FILEPATH == pdInfo->at(i).FileVer_InCollectSysEnv_State)
			{
				int InCorrect_FilePath_size = pdInfo->at(i).FileVer_InCollectSysEnv_FilePath.size();
				for (int j = 0; j < InCorrect_FilePath_size; j++)
				{
					nItemNum = ListView_GetItemCount(hListHWnd);
					swprintf_s(Number, L"%d", nItemNum + 1);

					LI.iItem = nItemNum + 1;
					LI.pszText = Number;

					SecureZeroMemory(szExp, sizeof(szExp));
					Policy::getInstance()->GetPolicy(MAIN_SECTION, STRING_EXP2, szExp, MAX_PATH);

					ListView_InsertItem(hListHWnd, &LI);
					ListView_SetItemText(hListHWnd, nItemNum, 1, pdInfo->at(i).File_Name);
					ListView_SetItemText(hListHWnd, nItemNum, 2, pdInfo->at(i).CSVInfo_FilePath);
					ListView_SetItemText(hListHWnd, nItemNum, 3, (LPWSTR)L"X");
					ListView_SetItemText(hListHWnd, nItemNum, 4, (LPWSTR)pdInfo->at(i).FileVer_InCollectSysEnv_FilePath.at(j).c_str());
					ListView_SetItemText(hListHWnd, nItemNum, 5, (LPWSTR)L"O");
					ListView_SetItemText(hListHWnd, nItemNum, 6, szExp);
				}
			}
			else if (NO_FILE == pdInfo->at(i).SysEnv_State)
			{
				nItemNum = ListView_GetItemCount(hListHWnd);
				swprintf_s(Number, L"%d", nItemNum + 1);

				LI.iItem = nItemNum + 1;
				LI.pszText = Number;

				SecureZeroMemory(szExp, sizeof(szExp));
				Policy::getInstance()->GetPolicy(MAIN_SECTION, STRING_EXP3, szExp, MAX_PATH);

				ListView_InsertItem(hListHWnd, &LI);
				ListView_SetItemText(hListHWnd, nItemNum, 1, pdInfo->at(i).File_Name);
				ListView_SetItemText(hListHWnd, nItemNum, 2, pdInfo->at(i).CSVInfo_FilePath);
				ListView_SetItemText(hListHWnd, nItemNum, 3, (LPWSTR)L"X");
				ListView_SetItemText(hListHWnd, nItemNum, 6, szExp);
			}
			else
			{
				int InCorrect_FilePath_size = pdInfo->at(i).SysEnv_FilePath.size();
				for (int j = 0; j < InCorrect_FilePath_size; j++)
				{
					nItemNum = ListView_GetItemCount(hListHWnd);
					swprintf_s(Number, L"%d", nItemNum + 1);

					LI.iItem = nItemNum + 1;
					LI.pszText = Number;

					SecureZeroMemory(szExp, sizeof(szExp));
					Policy::getInstance()->GetPolicy(MAIN_SECTION, STRING_EXP2, szExp, MAX_PATH);

					ListView_InsertItem(hListHWnd, &LI);
					ListView_SetItemText(hListHWnd, nItemNum, 1, pdInfo->at(i).File_Name);
					ListView_SetItemText(hListHWnd, nItemNum, 2, pdInfo->at(i).CSVInfo_FilePath);
					ListView_SetItemText(hListHWnd, nItemNum, 3, (LPWSTR)L"O");
					ListView_SetItemText(hListHWnd, nItemNum, 4, (LPWSTR)pdInfo->at(i).SysEnv_FilePath.at(j).c_str());
					ListView_SetItemText(hListHWnd, nItemNum, 5, (LPWSTR)L"O");
					ListView_SetItemText(hListHWnd, nItemNum, 6, szExp);
				}
			}
		}
		else if (INCORRECT_FILEPATH == pdInfo->at(i).File_State)
		{
			if (CORRECT_FILEPATH == pdInfo->at(i).FileVer_CollectSysEnv_State)
			{
				int InCorrect_FilePath_size = pdInfo->at(i).FileVer_CollectSysEnv_FilePath.size();
				for (int j = 0; j < InCorrect_FilePath_size; j++)
				{
					nItemNum = ListView_GetItemCount(hListHWnd);
					swprintf_s(Number, L"%d", nItemNum + 1);

					LI.iItem = nItemNum + 1;
					LI.pszText = Number;

					SecureZeroMemory(szExp, sizeof(szExp));
					Policy::getInstance()->GetPolicy(MAIN_SECTION, STRING_EXP1, szExp, MAX_PATH);

					ListView_InsertItem(hListHWnd, &LI);
					ListView_SetItemText(hListHWnd, nItemNum, 1, pdInfo->at(i).File_Name);
					ListView_SetItemText(hListHWnd, nItemNum, 2, pdInfo->at(i).CSVInfo_FilePath);
					ListView_SetItemText(hListHWnd, nItemNum, 3, (LPWSTR)L"O");
					ListView_SetItemText(hListHWnd, nItemNum, 4, (LPWSTR)pdInfo->at(i).FileVer_CollectSysEnv_FilePath.at(j).c_str());
					ListView_SetItemText(hListHWnd, nItemNum, 5, (LPWSTR)L"O");
					ListView_SetItemText(hListHWnd, nItemNum, 6, szExp);
				}
			}
			else if (INCORRECT_FILEPATH == pdInfo->at(i).FileVer_InCollectSysEnv_State)
			{
				int InCorrect_FilePath_size = pdInfo->at(i).FileVer_InCollectSysEnv_FilePath.size();
				for (int j = 0; j < InCorrect_FilePath_size; j++)
				{
					nItemNum = ListView_GetItemCount(hListHWnd);
					swprintf_s(Number, L"%d", nItemNum + 1);

					LI.iItem = nItemNum + 1;
					LI.pszText = Number;

					SecureZeroMemory(szExp, sizeof(szExp));
					Policy::getInstance()->GetPolicy(MAIN_SECTION, STRING_EXP2, szExp, MAX_PATH);

					ListView_InsertItem(hListHWnd, &LI);
					ListView_SetItemText(hListHWnd, nItemNum, 1, pdInfo->at(i).File_Name);
					ListView_SetItemText(hListHWnd, nItemNum, 2, pdInfo->at(i).CSVInfo_FilePath);
					ListView_SetItemText(hListHWnd, nItemNum, 3, (LPWSTR)L"O");
					ListView_SetItemText(hListHWnd, nItemNum, 4, (LPWSTR)pdInfo->at(i).FileVer_InCollectSysEnv_FilePath.at(j).c_str());
					ListView_SetItemText(hListHWnd, nItemNum, 5, (LPWSTR)L"O");
					ListView_SetItemText(hListHWnd, nItemNum, 6, szExp);
				}
			}
			else if (NO_FILE == pdInfo->at(i).SysEnv_State)
			{
				nItemNum = ListView_GetItemCount(hListHWnd);
				swprintf_s(Number, L"%d", nItemNum + 1);

				LI.iItem = nItemNum + 1;
				LI.pszText = Number;

				SecureZeroMemory(szExp, sizeof(szExp));
				Policy::getInstance()->GetPolicy(MAIN_SECTION, STRING_EXP3, szExp, MAX_PATH);

				ListView_InsertItem(hListHWnd, &LI);
				ListView_SetItemText(hListHWnd, nItemNum, 1, pdInfo->at(i).File_Name);
				ListView_SetItemText(hListHWnd, nItemNum, 2, pdInfo->at(i).CSVInfo_FilePath);
				ListView_SetItemText(hListHWnd, nItemNum, 3, (LPWSTR)L"O");
				ListView_SetItemText(hListHWnd, nItemNum, 6, szExp);
			}
			else
			{
				int InCorrect_FilePath_size = pdInfo->at(i).SysEnv_FilePath.size();
				for (int j = 0; j < InCorrect_FilePath_size; j++)
				{
					nItemNum = ListView_GetItemCount(hListHWnd);
					swprintf_s(Number, L"%d", nItemNum + 1);

					LI.iItem = nItemNum + 1;
					LI.pszText = Number;

					SecureZeroMemory(szExp, sizeof(szExp));
					Policy::getInstance()->GetPolicy(MAIN_SECTION, STRING_EXP2, szExp, MAX_PATH);

					ListView_InsertItem(hListHWnd, &LI);
					ListView_SetItemText(hListHWnd, nItemNum, 1, pdInfo->at(i).File_Name);
					ListView_SetItemText(hListHWnd, nItemNum, 2, pdInfo->at(i).CSVInfo_FilePath);
					ListView_SetItemText(hListHWnd, nItemNum, 3, (LPWSTR)L"O");
					ListView_SetItemText(hListHWnd, nItemNum, 4, (LPWSTR)pdInfo->at(i).SysEnv_FilePath.at(j).c_str());
					ListView_SetItemText(hListHWnd, nItemNum, 5, (LPWSTR)L"O");
					ListView_SetItemText(hListHWnd, nItemNum, 6, szExp);
				}
			}

		}
		else if (CORRECT_FILEPATH == pdInfo->at(i).File_State)
		{
			int InCorrect_FilePath_size = pdInfo->at(i).SysEnv_FilePath.size();
			for (int j = 0; j < InCorrect_FilePath_size; j++)
			{
				nItemNum = ListView_GetItemCount(hListHWnd);
				swprintf_s(Number, L"%d", nItemNum + 1);

				LI.iItem = nItemNum + 1;
				LI.pszText = Number;

				SecureZeroMemory(szExp, sizeof(szExp));
				Policy::getInstance()->GetPolicy(MAIN_SECTION, STRING_EXP4, szExp, MAX_PATH);

				ListView_InsertItem(hListHWnd, &LI);
				ListView_SetItemText(hListHWnd, nItemNum, 1, pdInfo->at(i).File_Name);
				ListView_SetItemText(hListHWnd, nItemNum, 2, pdInfo->at(i).CSVInfo_FilePath);
				ListView_SetItemText(hListHWnd, nItemNum, 3, (LPWSTR)L"O");
				ListView_SetItemText(hListHWnd, nItemNum, 4, (LPWSTR)pdInfo->at(i).SysEnv_FilePath.at(j).c_str());
				ListView_SetItemText(hListHWnd, nItemNum, 5, (LPWSTR)L"O");
				ListView_SetItemText(hListHWnd, nItemNum, 6, szExp);
			}
		}
		else if (NO_FILE == pdInfo->at(i).File_State)
		{
			if (NO_FILE == pdInfo->at(i).SysEnv_State)
			{
				nItemNum = ListView_GetItemCount(hListHWnd);
				swprintf_s(Number, L"%d", nItemNum + 1);

				LI.iItem = nItemNum + 1;
				LI.pszText = Number;

				SecureZeroMemory(szExp, sizeof(szExp));
				Policy::getInstance()->GetPolicy(MAIN_SECTION, STRING_EXP5, szExp, MAX_PATH);

				ListView_InsertItem(hListHWnd, &LI);
				ListView_SetItemText(hListHWnd, nItemNum, 1, pdInfo->at(i).File_Name);
				ListView_SetItemText(hListHWnd, nItemNum, 2, pdInfo->at(i).CSVInfo_FilePath);
				ListView_SetItemText(hListHWnd, nItemNum, 3, (LPWSTR)L"X");
				ListView_SetItemText(hListHWnd, nItemNum, 5, (LPWSTR)L"X");
				ListView_SetItemText(hListHWnd, nItemNum, 6, szExp);
			}
			else
			{
				int InCorrect_FilePath_size = pdInfo->at(i).SysEnv_FilePath.size();
				for (int j = 0; j < InCorrect_FilePath_size; j++)
				{
					nItemNum = ListView_GetItemCount(hListHWnd);
					swprintf_s(Number, L"%d", nItemNum + 1);

					LI.iItem = nItemNum + 1;
					LI.pszText = Number;

					SecureZeroMemory(szExp, sizeof(szExp));
					Policy::getInstance()->GetPolicy(MAIN_SECTION, STRING_EXP4, szExp, MAX_PATH);

					ListView_InsertItem(hListHWnd, &LI);
					ListView_SetItemText(hListHWnd, nItemNum, 1, pdInfo->at(i).File_Name);
					ListView_SetItemText(hListHWnd, nItemNum, 2, pdInfo->at(i).CSVInfo_FilePath);
					ListView_SetItemText(hListHWnd, nItemNum, 3, (LPWSTR)L"X");
					ListView_SetItemText(hListHWnd, nItemNum, 4, (LPWSTR)pdInfo->at(i).SysEnv_FilePath.at(j).c_str());
					ListView_SetItemText(hListHWnd, nItemNum, 5, (LPWSTR)L"O");
					ListView_SetItemText(hListHWnd, nItemNum, 6, szExp);
				}
			}
		}
	}
	return ERROR_SUCCESS;
}

int AnaylsisMainDlg::OnFileSave(HWND hMainDlg, HWND hListView)
{
	if (0 == hMainDlg || 0 == hListView)
		return ERROR_INVALID_PARAMETER;

	if (0 == m_vecFile_Info.size())
	{
		MsgBox(hMainDlg, MSG_SELECT_FILE_FIRST, MB_ICONERROR);
		return ERROR_INVALID_FUNCTION;
	}

	OPENFILENAME OFN = { 0, };

	OFN.lStructSize = sizeof(OFN);
	OFN.hwndOwner = hMainDlg;
	OFN.lpstrFile = m_szSave_CSVFile;
	OFN.nMaxFile = sizeof(m_szSave_CSVFile);
	OFN.lpstrFilter = L"CSV\0*.csv\0";
	OFN.nFilterIndex = 1;
	OFN.lpstrFileTitle = NULL;
	OFN.nMaxFileTitle = 0;
	OFN.lpstrInitialDir = NULL;
	OFN.lpstrDefExt = L"csv";
	OFN.Flags = OFN_CREATEPROMPT | OFN_OVERWRITEPROMPT | OFN_EXTENSIONDIFFERENT;

	if (TRUE == GetSaveFileNameW(&OFN))
	{
		int nRetval = FileSave(hListView, m_szSave_CSVFile);

		if (ERROR_SUCCESS != nRetval) {

			MsgBox(hMainDlg, MSG_FILE_SAVE_FAIL, MB_ICONERROR);
			return ERROR_INVALID_FUNCTION;
		}

	}

	return ERROR_INVALID_FUNCTION;
}

int UnicodeToUtf8(WCHAR* pUnicode, char** pUtf8)
{
	int len = ::WideCharToMultiByte(CP_UTF8, 0, (LPCWSTR)pUnicode, -1, NULL, 0, NULL, NULL);
	*pUtf8 = new char[len];
	::WideCharToMultiByte(CP_UTF8, 0, (LPCWSTR)pUnicode, -1, *pUtf8, len, NULL, NULL);

	return len;
}

int AnaylsisMainDlg::FileSave(HWND hListHWnd, WCHAR* pCSVFilePath)
{
	if (NULL == hListHWnd)
		return ERROR_INVALID_PARAMETER;

	if (NULL == pCSVFilePath)
		return ERROR_INVALID_PARAMETER;


	DWORD result = 0;
	WORD wBOM = 0xBBEF;
	BYTE wwROM = 0xBF;

	HANDLE hFile = ::CreateFile(pCSVFilePath, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		CloseHandle(hFile);

		hFile = NULL;
		return ERROR_INVALID_FUNCTION;
	}

	if (TRUE != ::WriteFile(hFile, &wBOM, sizeof(WORD), &result, NULL))
	{
		CloseHandle(hFile);
		return ERROR_INVALID_FUNCTION;
	}

	if (TRUE != WriteFile(hFile, &wwROM, sizeof(BYTE), &result, NULL))
	{
		CloseHandle(hFile);
		return ERROR_INVALID_FUNCTION;
	}

	int colCount = Header_GetItemCount(m_hHeader);
	int rowCount = ListView_GetItemCount(hListHWnd);
	int indexCount = 0;
	WCHAR szTemp[MAX_PATH] = { 0, };
	WCHAR sztext[BUFSIZ * 2] = { 0, };

	wcscpy_s(sztext, L"ModuleFileName,ModuleFile path,Exist,EnvironmentVariable Path,Exist,Exp\n");

	char* pTextHeader = NULL;
	UnicodeToUtf8(sztext, &pTextHeader);

	if (0 == pTextHeader)
	{
		delete[] pTextHeader;
		CloseHandle(hFile);
		return ERROR_INVALID_FUNCTION;
	}

	if (TRUE != ::WriteFile(hFile, pTextHeader, strlen(pTextHeader), &result, NULL))
	{
		CloseHandle(hFile);
		return ERROR_INVALID_FUNCTION;
	}

	delete[] pTextHeader;
	memset(sztext, 0, sizeof(sztext));

	for (int i = 0; i < rowCount; i++)
	{
		for (int j = 1; j < colCount; j++)
		{
			ListView_GetItemText(hListHWnd, i, j, szTemp, sizeof(szTemp));
			wcscat_s(sztext, szTemp);
			if (j != colCount - 1)
				wcscat_s(sztext, L",");
		}
		if (i != rowCount - 1)
		{
			wcscat_s(sztext, L"\n");
		}

		char* pText = NULL;
		UnicodeToUtf8(sztext, &pText);

		if (0 == pText)
		{
			delete[] pText;
			CloseHandle(hFile);
			return ERROR_INVALID_FUNCTION;
		}

		if (TRUE != ::WriteFile(hFile, pText, strlen(pText), &result, NULL))
		{
			return ERROR_INVALID_FUNCTION;
		}

		delete[] pText;
		memset(sztext, 0x00, sizeof(sztext));
	}

	CloseHandle(hFile);
	return ERROR_SUCCESS;
}

INT_PTR CALLBACK AnaylsisMainDlg::KeybdProc(int code, WPARAM wParam, LPARAM lParam)
{
	if (code < 0)
		return CallNextHookEx(AnaylsisMainDlg::getInstance()->m_Hok, code, wParam, lParam);

	switch (wParam)
	{
	case VK_ESCAPE:
	{
		return TRUE;
	}
	break;
	}

	return ERROR_SUCCESS;
}

int AnaylsisMainDlg::OnDblclkListToFolder(NMHEADER* pNMHDR)
{
	if (NULL == pNMHDR)
	{
		return ERROR_INVALID_PARAMETER;
	}

	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	if (NULL == pNMItemActivate)
	{
		return ERROR_INVALID_FUNCTION;
	}

	int Rowindex = pNMItemActivate->iItem;
	int Colindex = pNMItemActivate->iSubItem;

	WCHAR szFilePath[MAX_PATH] = { 0, };

	if (2 == Colindex && Colindex < ListView_GetItemCount(m_hListHWnd))
	{
		ListView_GetItemText(m_hListHWnd, Rowindex, Colindex, szFilePath, sizeof(szFilePath));
		if (0 < wcslen(szFilePath))
		{
			ShellExecute(NULL, L"open", szFilePath, NULL, NULL, SW_SHOW);
			return ERROR_SUCCESS;
		}
	}

	if (4 == Colindex && Colindex < ListView_GetItemCount(AnaylsisMainDlg::getInstance()->m_hListHWnd))
	{
		ListView_GetItemText(m_hListHWnd, Rowindex, Colindex, szFilePath, sizeof(szFilePath));
		if (0 < wcslen(szFilePath))
		{
			ShellExecute(NULL, L"open", szFilePath, NULL, NULL, SW_SHOW);
			return ERROR_SUCCESS;
		}
	}

	return ERROR_INVALID_FUNCTION;
}

int AnaylsisMainDlg::OnHdnItemSortingList(NMHEADER* pNMHDR)
{
	if (NULL == pNMHDR)
	{
		return ERROR_INVALID_PARAMETER;
	}

	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	if (NULL == pNMLV)
	{
		return ERROR_INVALID_FUNCTION;
	}

	int nColumn = pNMLV->iItem;

	if (nColumn == 0)
	{
		return ERROR_INVALID_FUNCTION;
	}

	for (int i = 0; i < ListView_GetItemCount(m_hListHWnd); i++)
	{
		LV_ITEM li;
		memset(&li, 0, sizeof(li));
		li.mask = LVIF_PARAM;
		li.lParam = i;
		li.iItem = i;
		ListView_SetItem(m_hListHWnd, &li);
	}

	m_bAscending = !m_bAscending;

	SORTPARAM sortparams;
	sortparams.hListView_HWnd = m_hListHWnd;
	sortparams.iSortColumn = nColumn;
	sortparams.bSortDirect = (0 != m_bAscending);

	ListView_SortItems(m_hListHWnd, &CompareItem, (LPARAM)&sortparams);

	for (int i = 0; i < ListView_GetItemCount(m_hListHWnd); i++)
	{
		WCHAR szTemp[10] = { 0, };
		swprintf_s(szTemp, L"%d", i + 1);
		ListView_SetItemText(m_hListHWnd, i, 0, szTemp);
	}

	return ERROR_SUCCESS;
}

int CALLBACK AnaylsisMainDlg::CompareItem(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	if (0 == lParam1 || 0 == lParam2 || 0 == lParamSort)
	{
		return ERROR_INVALID_PARAMETER;
	}

	HWND hList_SortHWnd = ((SORTPARAM*)lParamSort)->hListView_HWnd;
	int iSortColumn = ((SORTPARAM*)lParamSort)->iSortColumn;
	bool bSortDirect = ((SORTPARAM*)lParamSort)->bSortDirect;

	LVFINDINFO info1, info2;		////LVFINDINFO : 목록보기 항목을 검색 할 때 사용되는 정보를 포함
	info1.flags = LVFI_PARAM;		////LVFI_PARAM : 목록보기 항목의 속성을 지정하거나받습니다
	info1.lParam = lParam1;			////IParam을 이용해서 row를 찾기 위한 부분
	info2.flags = LVFI_PARAM;
	info2.lParam = lParam2;

	int irow1 = ListView_FindItem(hList_SortHWnd, -1, &info1);
	int irow2 = ListView_FindItem(hList_SortHWnd, -1, &info2);

	WCHAR szItem1[MAX_PATH] = { 0, };
	WCHAR szItem2[MAX_PATH] = { 0, };

	ListView_GetItemText(hList_SortHWnd, irow1, iSortColumn, szItem1, sizeof(szItem1));
	ListView_GetItemText(hList_SortHWnd, irow2, iSortColumn, szItem2, sizeof(szItem2));

	return bSortDirect ? wcscmp(szItem1, szItem2) : -wcscmp(szItem1, szItem2);
}

int AnaylsisMainDlg::OnFindText()
{
	if (0 == m_vecFile_Info.size())
	{
		MsgBox(m_hMainDlg, MSG_SELECT_FILE_FIRST, MB_ICONERROR);
		return ERROR_INVALID_FUNCTION;
	}

	FindTextDlg::getInstance()->FindTextDialog(m_hListHWnd);

	WCHAR szFindText[MAX_PATH] = { 0, };
	int nRetVal = ERROR_INVALID_FUNCTION;

	nRetVal = FindTextDlg::getInstance()->GetData(szFindText, _countof(szFindText));

	if (ERROR_INVALID_FUNCTION == nRetVal)
	{
		return ERROR_INVALID_FUNCTION;
	}

	int colCount = Header_GetItemCount(m_hHeader);
	int rowCount = ListView_GetItemCount(m_hListHWnd);
	WCHAR sztext[MAX_PATH] = { 0, };

	BOOL bFind = FALSE;

	ListView_SetItemState(m_hListHWnd, -1, 0, LVIS_FOCUSED | LVIS_SELECTED);

	for (int rowindex = 0; rowindex < rowCount; rowindex++)
	{
		for (int colindex = 1; colindex < colCount; colindex++)
		{
			ListView_GetItemText(m_hListHWnd, rowindex, colindex, sztext, sizeof(sztext));
			if (NULL != StrStrIW(sztext, szFindText))
			{
				bFind = TRUE;
				ListView_SetItemState(m_hListHWnd, rowindex, LVIS_FOCUSED | LVIS_SELECTED, LVIS_FOCUSED | LVIS_SELECTED);
			}
		}
	}

	if (FALSE == bFind)
	{
		MsgBox(m_hMainDlg, MSG_NO_STRING, MB_ICONERROR);
		return ERROR_INVALID_FUNCTION;
	}

	return ERROR_SUCCESS;
}

void AnaylsisMainDlg::MsgBox(HWND hDlg, WCHAR* szMsgText, UINT nType)
{
	if (NULL == hDlg || NULL == szMsgText)
	{
		return;
	}

	WCHAR szText[MAX_PATH] = { 0, };

	Policy::getInstance()->GetPolicy(THIRD_SECTION, szMsgText, szText, MAX_PATH);

	MessageBox(hDlg, szText, m_szMsgTitle, nType);
}