#include "pch.h"
#include "AnaylsisMainDlg.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(nCmdShow);

	return AnaylsisMainDlg::getInstance()->ConfigManagementFile_Analysis_Dialog(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
}