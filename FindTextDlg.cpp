#include "pch.h"
#include "FindTextDlg.h"
#include "AnaylsisMainDlg.h"
#include "resource.h"
#include "Policy.h"

FindTextDlg* FindTextDlg::getInstance()
{
	static FindTextDlg inst;
	return &inst;
}

FindTextDlg::FindTextDlg(void)
{
	memset(m_szFindText, 0, sizeof(m_szFindText));

	m_hMainDlg = 0;
	m_hEdit = 0;
	m_Hok = 0;
	m_hIcon = 0;
}

FindTextDlg::~FindTextDlg(void)
{
}

int FindTextDlg::FindTextDialog(HWND hparentDlg)
{
	DialogBoxW(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG_CCIS_FIND), hparentDlg, DlgProc);

	return TRUE;
}

INT_PTR CALLBACK FindTextDlg::DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
	{
		FindTextDlg::getInstance()->InitDialog(hDlg);
	}
	break;

	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDCANCEL:
		{
			UnhookWindowsHookEx(FindTextDlg::getInstance()->m_Hok);
			EndDialog(hDlg, 0);
			return TRUE;
		}
		break;

		case IDC_BUTTON_CCIS_FIND:
			UnhookWindowsHookEx(FindTextDlg::getInstance()->m_Hok);
			FindTextDlg::getInstance()->OnButtonFind(FindTextDlg::getInstance()->m_hMainDlg);
			EndDialog(hDlg, 0);
			return TRUE;
		}
		break;
	}
	break;

	default:
		return FALSE;
	}
	return FALSE;
}

int FindTextDlg::InitDialog(HWND hDlg)
{
	int nRetval = 0;
	WCHAR szTitle[MAX_PATH] = { 0, };

	Policy::getInstance()->GetPolicy(SECOND_SECTION, DLG_TITLE, szTitle, MAX_PATH);

	nRetval = SetWindowText(hDlg, szTitle);

	if (0 == nRetval)
	{
		return ERROR_INVALID_FUNCTION;
	}

	/*m_hIcon = (HICON)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDR_MAINFRAME), IMAGE_ICON, 16, 16, LR_DEFAULTSIZE);
	SendMessage(hDlg, WM_SETICON, ICON_SMALL, (LPARAM)m_hIcon);*/

	m_hMainDlg = hDlg;
	m_hEdit = ::GetDlgItem(hDlg, IDC_EDIT_CCIS_FIND);
	m_Hok = SetWindowsHookEx(WH_KEYBOARD, (HOOKPROC)KeybdProc, NULL, GetCurrentThreadId());

	return ERROR_SUCCESS;
}

int FindTextDlg::OnButtonFind(HWND hDlg)
{
	int nRetval = 0;

	nRetval = GetWindowTextW(m_hEdit, m_szFindText, MAX_PATH);

	if (0 == nRetval)
	{
		return ERROR_INVALID_FUNCTION;
	}

	return ERROR_SUCCESS;
}

int FindTextDlg::GetData(WCHAR* pStr, int nBufferSize)
{
	if (NULL == pStr || 0 == nBufferSize)
	{
		return ERROR_INVALID_PARAMETER;
	}

	wcsncpy_s(pStr, nBufferSize, m_szFindText, nBufferSize - 1);

	return ERROR_SUCCESS;
}

INT_PTR CALLBACK FindTextDlg::KeybdProc(int code, WPARAM wParam, LPARAM lParam)
{
	if (code < 0)
		return CallNextHookEx(FindTextDlg::getInstance()->m_Hok, code, wParam, lParam);

	switch (wParam)
	{
	case VK_RETURN:
	{
		FindTextDlg::getInstance()->OnButtonFind(FindTextDlg::getInstance()->m_hMainDlg);
		UnhookWindowsHookEx(FindTextDlg::getInstance()->m_Hok);
		EndDialog(FindTextDlg::getInstance()->m_hMainDlg, 0);
	}
	break;
	case VK_ESCAPE:
	{
		UnhookWindowsHookEx(FindTextDlg::getInstance()->m_Hok);
		EndDialog(FindTextDlg::getInstance()->m_hMainDlg, 0);
	}
	break;

	default:
		return FALSE;
	}

	return ERROR_SUCCESS;
}