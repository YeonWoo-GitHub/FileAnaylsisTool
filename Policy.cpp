#include "pch.h"
#include "Policy.h"

Policy* Policy::getInstance()
{
	static Policy inst;
	return &inst;
}

Policy::Policy(void)
{
}

Policy::~Policy(void)
{
}

BOOL Policy::GetPolicy(LPWSTR lpszSection, LPWSTR lpszIdString, LPWSTR lpReturnedString, DWORD nSize)
{
	if (NULL == lpszSection || NULL == lpszIdString)
	{
		return FALSE;
	}

	WCHAR sziniPath[MAX_PATH] = { 0, };
	WCHAR szModulePath[MAX_PATH] = { 0, };
	WCHAR szData[MAX_PATH] = { 0, };

	GetCurrentDirectory(MAX_PATH, szModulePath);
	swprintf_s(sziniPath, L"%s\\%s", szModulePath, POLICY_FILE);

	GetPrivateProfileString(lpszSection, lpszIdString, L"", szData, nSize, sziniPath);

	if (0 == wcslen(szData))
	{
		return FALSE;
	}

	wcsncpy_s(lpReturnedString, nSize, szData, nSize - 1);

	return TRUE;
}