#pragma once

#include "pch.h"
/**
	@mainpage	������� ���� �м� ��� ����

	@section Intro 1. �Ұ�
		- ������� ���Ͽ� �����ִ� DLL���� �� ��ְ� �ְų� ���Ϲ������� ���� ��ġ��ΰ� �߸��� ��츦 �м��Ͽ� ����Ѵ�.
		- WinApi�� ������� ���ߵǾ���.

	@section Functin_Sec 2. ���
		- ��� ����@n
			a. ������� ���Ͽ� �����ִ� DLL������ ���� ��ġ��� �Ӹ� �ƴ϶� ȯ�溯�� Path�� �����ִ� ��ο��� ������ ��� ��ַ� �Ǵ��Ͽ� ����Ѵ�.@n
			b. ������� ���Ͽ� �����ִ� DLL������ ���� ��ġ��ο� �������� �ʰ� ȯ�溯�� Path�� �����ִ� ��ο��� ������ ��� ��ַ� �Ǵ��Ͽ� ����Ѵ�.@n
			c. ������� ���Ͽ� �����ִ� DLL������ �����ο��� �������� ���� ��� ��ַ� �Ǵ��Ͽ� ����Ѵ�.@n
			d. ������� ���Ͽ� �����ִ� DLL������ ����(x64, x86)�� ���� ���� ��ġ��ΰ� �߸��ư� ȯ�溯�� Path�� �����ִ� ��ΰ� �ùٸ� ��� ����Ѵ�.@n
			e. ������� ���Ͽ� �����ִ� DLL������ ����(x64, x86)�� ���� ���� ��ġ��ο� ȯ�溯�� Path�� �����ִ� ��� �� �� �߸��� ���@n
		- ������� ����(CSV����)�� �м� ��� �� �˻�â�� ���� ���ϴ� ���� �Ǵ� ���ڿ��� �˻��Ͽ� �� �� �ִ�.
		- ������� ����(CSV����)�� �м� ����� CSV���� �������� ������ �� �ִ�.
 */



#pragma once

using namespace std;

#define NO_FILE						0x00000001			///<���Ͼ���
#define CORRECT_FILEPATH			0x00000002			///<�ùٸ� ���ϰ��
#define NON_STATIONARY_FILEPATH		0x00000004			///<���������� ���ϰ��
#define INCORRECT_FILEPATH			0x00000008			///<�ùٸ��� ���� ���ϰ��

#define BIT_64						0x00000010
#define BIT_32						0x00000020

#define READ_BUFSIZE				32767
#define ENCODING_UNICODE				0x00000040
#define ENCODING_ANSI					0x00000080

#define FILEOPEN				1
#define FILESAVE				2
#define QUIT					3
#define FINDTEXT				4

/*!
\brief		������ ������� ���� �����͸� �����ϴ� ����ü
*/
typedef struct File_Info
{
	WCHAR File_Name[MAX_PATH];								///<��������̸�
	WCHAR CSVInfo_FilePath[MAX_PATH];						///<������� ��ġ���
	DWORD File_State;										///<������� ����(������� ���ϳ����� ��������)
	DWORD SysEnv_State;										///<ȯ�溯�� ��� ����
	DWORD FileVer_CollectSysEnv_State;						///<������� ������ ���� �ùٸ� ȯ�溯�� ��� ����
	DWORD FileVer_InCollectSysEnv_State;					///<������� ������ ���� �ùٸ��� ���� ȯ�溯�� ��� ����
	DWORD FileVersion_Flag;									///<������� ����
	vector<wstring>SysEnv_FilePath;							///<ȯ�溯�� Path���� ������ ȯ�溯�� ��� ������
	vector<wstring>FileVer_CollectSysEnv_FilePath;			///<������� ������ ���� �ùٸ� ȯ�溯�� ��� ������
	vector<wstring>FileVer_InCollectSysEnv_FilePath;		///<������� ������ ���� �ùٸ��� ���� ȯ�溯�� ��� ������
	WCHAR Exp[MAX_PATH];									///<���
} File_Info, * pFile_Info;

/*!
\brief		����Ʈ �� ���� �� ����ϴ� ����ü
*/
struct SORTPARAM
{
	int iSortColumn;				///<���� �� Ȯ���� ����Ʈ �� �׸�
	bool bSortDirect;				///<���� ���(�������� �Ǵ� ��������)
	HWND hListView_HWnd;			///<������ ����Ʈ �� �ڵ� ����
};

/*!
\brief		������� ������ �м��� ���� Ŭ���� ����
*/
class AnaylsisMainDlg
{
private:

	WCHAR m_szCSVFilePath[MAX_PATH];				///<������� ������ ��ġ ���					
	WCHAR m_szSave_CSVFile[MAX_PATH];				///<������ ������ ���
	WCHAR m_szMsgTitle[MAX_PATH];					///<�޼��� �ڽ� Ÿ��Ʋ
	vector<wstring> m_vecSystemEnvPath_Info;		///<ȯ�溯�� Path ������ ����
	vector<File_Info> m_vecFile_Info;				///<������� ���� ������ ����

	vector<wstring> m_FileData;						///<������� ���Ͽ��� �� �� ������ ����				

	/*!
	\brief		CCISFileDlg Ŭ���� ������
	*/
	AnaylsisMainDlg(void);
public:

	/*!
	\brief		CCISFileDlg Ŭ���� �Ҹ���
	*/
	~AnaylsisMainDlg(void);

	/*!
	\brief		�� ���� CCISFileDlg Ŭ���� �ν��Ͻ� ����
	\return
	-inst		��ü
	*/
	static AnaylsisMainDlg* getInstance();

	/*!
	\brief		������ ��ǥ��
	\details	������ ��ǥ������ ������ �ּ� ũ��� ������� �����Ѵ�
	*/
	RECT m_Wndrect;

	HWND m_hListHWnd;			///<����Ʈ �� �ڵ鰪
	HWND m_hHeader;				///<����Ʈ �� ��� �ڵ鰪
	HWND m_hMainDlg;			///<������� ���� �м� ���̾�α׿� ���� �ڵ鰪
	HDROP m_hDropInfo;			///<�巡�� �� ����� ���� ���� �ڵ�

	/*!
	\brief		���� ���(�������� �Ǵ� ��������)
	\details	����Ʈ �� ���� �� �������� �Ǵ� ������������ �����ϴ� ���
	*/
	BOOL m_bAscending;
	HHOOK m_Hok;				///<Ű���� �Է°��� ���� Hook
	HICON m_hIcon;				///<�����ܿ� ���� �ڵ鰪
	int m_Sizecx;				///<������ �ּ� ������� ���α���
	int m_Sizecy;				///<������ �ּ� ������� ���α���

	/*!
	\brief		������� ���� �м� ���̾�α� ����
	\param[in] hDlg		������� ���� �м� ���̾�α׿� ���� �ڵ�
	\return
	\b ���� @n
	-ERROR_SUCCESS : 0 (����)
	*/
	int ConfigManagementFile_Analysis_Dialog(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow);

	/*!
	\brief		������� ���� �м� ���̾�α׿� ���۵� �޽��� ó��
	\param[in] hDlg		������� ���� �м� ���̾�α׿� ���� �ڵ�
	\param[in] message	�޽���
	\param[in] wParam	�߰� �޽��� �� ����
	\param[in] IParam	�߰� �޽��� �� ����
	\return
	\b ���� @n
	-TRUE@n
	\b ���� @n
	-FALSE
	*/
	static INT_PTR CALLBACK DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	/*!
	\brief		����Ʈ ���� ����ʱ�ȭ
	\param[in]	�ʱ�ȭ �� �Ѱܹ޴� ����Ʈ �� �ڵ�
	\return
	\b ���� @n
	-ERROR_SUCCESS : 0 (����) @n
	\b ���� @n
	-ERROR_INVALID_PARAMETER : 87L (The parameter is incorrect)
	*/
	int ListView_Init(HWND hListView);

	/*!
	\brief		���� �巡�� �� ���
	\param[in] hMainDlg			������� ���� �м� ���̾�α� �ڵ�
	\param[in] hDropInfo		�巡�� �� ����� ���� ���� �ڵ�
	\param[in] hListView		����Ʈ �� �ڵ�
	\return
	\b ���� @n
	-ERROR_SUCCESS : 0 (����) @n
	\b ���� @n
	-ERROR_INVALID_FUNCTION : 1L (Incorrect function)@n
	-ERROR_INVALID_PARAMETER : 87L (The parameter is incorrect)
	*/
	int OnDropFiles(HWND hMainDlg, HDROP hDropInfo, HWND hListView);

	/*!
	\brief		���Ͽ��� ��ȭ����
	\details	���Ͽ��� ��ȭ���ڸ� ���� ������� ������ �Ѱܹ���
	\param[in] hMainDlg			������� ���� �м� ���̾�α� �ڵ�
	\param[in] hListView		����Ʈ �� �ڵ�
	\return
	\b ���� @n
	-ERROR_SUCCESS : 0 (����) @n
	\b ���� @n
	-ERROR_INVALID_FUNCTION : 1L (Incorrect function)@n
	-ERROR_INVALID_PARAMETER : 87L (The parameter is incorrect)
	*/
	int OnOpenCSVFile(HWND hMainDlg, HWND hListView);

	/*!
	\brief		������� ���� �м��� ���η����� �帧�� ����
	\details	������� ���ϰ� ȯ�溯�� Path ������ ������ �����۾� �� ����� ����Ʈ ��� ���
	\param[in] hMainDlg			������� ���� �м� ���̾�α� �ڵ�
	\param[in] hListView		����Ʈ �� �ڵ�
	\return
	\b ���� @n
	-ERROR_SUCCESS : 0 (����)@n
	\b ���� @n
	-ERROR_INVALID_PARAMETER : 87L (The parameter is incorrect)@n
	-ERROR_INVALID_FUNCTION : 1L (Incorrect function)
	*/
	int InteriorLogicFunc(HWND hMainDlg, HWND hListView);

	/*!
	\brief			������� ������ ������ ����
	\details
	-������� ���� ������ DLL������ �����͸� ����@n
	-������ DLL������ ������ �� ������ '���ϸ�, ��ġ��ġ, ����' ������ ����
	\param[in] hMainDlg		������� ���� �м� ���̾�α� �ڵ�
	\param[in] pCSVFilePath	������� ������ ���
	\param[out] pdInfo		������ DLL������ ������ �� ������ '���ϸ�, ��ġ��ġ, ����' ���� ����
	\return
	\b ���� @n
	-ERROR_SUCCESS : 0 (����)@n
	\b ���� @n
	-ERROR_INVALID_PARAMETER : 87L (The parameter is incorrect)@n
	-ERROR_INVALID_DATA : 13L (The data is invalid)@n
	-ERROR_TOO_MANY_OPEN_FILES : 4L (The system cannot open the file.)@n
	-ERROR_INVALID_FUNCTION : 1L (Incorrect function)
	*/
	int Read_CSVFile(HWND hMainDlg, WCHAR* pCSVFilePath, std::vector<File_Info>* pdInfo);

	/*!
	\brief			ȯ�溯�� Path ������ ����
	\details		ȯ�溯�� Path ������ ���� �� 'softcmap' ���ڿ��� �ִ� ���� ����
	\param[in] hMainDlg		������� ���� �м� ���̾�α� �ڵ�
	\param[out] pSystemEnv	ȯ�溯�� Path ������ ����
	\return
	\b ���� @n
	-ERROR_SUCCESS : 0 (����)@n
	\b ���� @n
	-ERROR_INVALID_PARAMETER : 87L (The parameter is incorrect)@n
	-ERROR_ENVVAR_NOT_FOUND : 203L (The system could not find the environment option that was entered)@n
	-ERROR_NOT_ENOUGH_MEMORY : 8L (Not enough memory resources are available to process this command)@n
	-ERROR_INVALID_DATA : 13L (The data is invalid)
	*/
	int Read_SystemEnvironment(HWND hMainDlg, std::vector<wstring>* pSystemEnv);

	/*!
	\brief			������ ���ڵ� Ȯ��
	\details
	-������ ���ڵ��� Unicode �Ǵ� Ansi���� Ȯ���Ͽ� �� �� �ϳ��� ������@n
	-������ ���ڵ��� UTF-8�� ��� ������� ���� �м� �Ұ���( ���ڵ��� Unicode �Ǵ� Ansi�� �ٲ�� �� )
	\param[in] hMainDlg		������� ���� �м� ���̾�α� �ڵ�
	\param[in] pCSVFilePath	������� ������ ���
	\return
	\b ���� @n
	-Encoding : Unicode �Ǵ� Ansi ��ȯ@n
	\b ���� @n
	-ERROR_INVALID_PARAMETER : 87L (The parameter is incorrect)@n
	-ERROR_INVALID_FUNCTION : 1L (Incorrect function)
	*/
	int GetFileEncoding(HWND hMainDlg, WCHAR* pCSVFilePath);

	/*!
	\brief			������� ���� ������ �Ľ�
	\details		������� ���� �����Ϳ��� ��ǥ�� �����ڷ� ���ڿ� �Ľ�
	\param[in] pFileLine		������� ���Ͽ��� �� ��
	\return
	\b ���� @n
	-ERROR_SUCCESS : 0 (����)@n
	\b ���� @n
	-ERROR_INVALID_PARAMETER : 87L (The parameter is incorrect)@n
	-ERROR_INVALID_DATA : 13L (The data is invalid)
	*/
	int FileDataCollect(WCHAR* pFileLine);

	/*!
	\brief			������� ���Ͽ��� ������ ������ ����
	\details		������ ������ �� ��� ������ '��ġ��ġ'�� ������ ���翩�� Ȯ�� �� ����
	\param[in] hMainDlg		������� ���� �м� ���̾�α� �ڵ�
	\param[out] pdInfo		'��ġ��ġ'�� ���� ������ ���翩�� ����
	\return
	\b ���� @n
	-ERROR_SUCCESS : 0 (����)@n
	\b ���� @n
	-ERROR_INVALID_PARAMETER : 87L (The parameter is incorrect)@n
	-ERROR_INVALID_DATA : 13L (The data is invalid)@n
	-ERROR_INVALID_FUNCTION : 1L (Incorrect function)
	*/
	int Verification_CSVFile(HWND hMainDlg, File_Info* pdInfo);

	/*!
	\brief			ȯ�溯�� Path���� ������ ������ ����
	\details		������ ������ �� ��ο� ��� ������ ���翩�� Ȯ�� �� ����
	\param[in] pSystemEnv		ȯ�溯�� Path ������
	\param[in] pdInfo			������ ���ϸ� �������� ������ �����ϴ� ȯ�溯�� Path ������ ����
	\return
	\b ���� @n
	-ERROR_SUCCESS : 0 (����)@n
	\b ���� @n
	-ERROR_INVALID_PARAMETER : 87L (The parameter is incorrect)@n
	-ERROR_INVALID_DATA : 13L (The data is invalid)@n
	-ERROR_INVALID_FUNCTION : 1L (Incorrect function)
	*/
	int Verification_SystemEnvironment(std::vector<wstring>* pSystemEnv, File_Info* pdInfo);

	/*!
	\brief			������ �����͵��� ����Ʈ ��� ��� ���
	\param[in] hListHWnd		������� ���� �м� ���̾�α� �ڵ�
	\param[in] pdInfo			����Ʈ ��� ����� �����͵� ����
	\return
	\b ���� @n
	-ERROR_SUCCESS : 0 (����)@n
	\b ���� @n
	-ERROR_INVALID_PARAMETER : 87L (The parameter is incorrect)
	*/
	int ListView_Print(HWND hListHWnd, vector<File_Info>* pdInfo);

	/*!
	\brief			�������� ���̾�α�
	\param[in] hListHWnd		������� ���� �м� ���̾�α� �ڵ�
	\param[in] hListView		����Ʈ �� �ڵ�
	\return
	\b ���� @n
	-ERROR_SUCCESS : 0 (����)@n
	\b ���� @n
	-ERROR_INVALID_PARAMETER : 87L (The parameter is incorrect)@n
	-ERROR_INVALID_FUNCTION : 1L (Incorrect function)
	*/
	int OnFileSave(HWND hMainDlg, HWND hListView);

	/*!
	\brief			�������� ���η���
	\details		����Ʈ ��� ��µ� ������ CSV���� �������� �����Ѵ�
	\param[in] hListHWnd		����Ʈ �� �ڵ�
	\param[in] pCSVFilePath		������� ���� ���
	\return
	\b ���� @n
	-ERROR_SUCCESS : 0 (����)@n
	\b ���� @n
	-ERROR_INVALID_PARAMETER : 87L (The parameter is incorrect)@n
	-ERROR_INVALID_FUNCTION : 1L (Incorrect function)
	*/
	int FileSave(HWND hListHWnd, WCHAR* pCSVFilePath);

	/*!
	\brief		Ű���� �Է��� ���� �Լ�
	\details
	-hook�� �̿��� Ű���� �Է��� �ν��ϰ� �Լ��� �ۼ��� �ڵ� ����@n
	-ESCŰ ���� ��, ���� ����
	\param[in] code		Ű���� hook Ÿ��
	\param[in] wParam	�߰� �޽��� �� ����
	\param[in] IParam	�߰� �޽��� �� ����
	\return
	\b ���� @n
	-ERROR_SUCCESS : 0 (����)@n
	-TRUE@n
	\b ���� @n
	-CallNextHookEx(CCISFileDlg::getInstance()->m_Hok, code, wParam, lParam)
	*/
	static INT_PTR CALLBACK KeybdProc(int code, WPARAM wParam, LPARAM lParam);

	/*!
	\brief			����Ʈ �信�� ���콺 ���� ��ư���� ����Ŭ�� �� ��Ÿ���� �̺�Ʈ
	\details		'���'�� ����Ŭ�� �� �ش� ����� ���� Ž���� ���
	\param[in] pNMHDR	Ŭ�� �� ����Ʈ �� �� ����
	\return
	\b ���� @n
	-ERROR_SUCCESS : 0 (����)@n
	\b ���� @n
	-ERROR_INVALID_PARAMETER : 87L (The parameter is incorrect)@n
	-ERROR_INVALID_FUNCTION : 1L (Incorrect function)
	*/
	int OnDblclkListToFolder(NMHEADER* pNMHDR);

	/*!
	\brief			����Ʈ �� ��� Ŭ�� �� ���� �̺�Ʈ
	\param[in] pNMHDR	Ŭ�� �� ����Ʈ �� ��� ����
	\return
	\b ���� @n
	-ERROR_SUCCESS : 0 (����)@n
	\b ���� @n
	-ERROR_INVALID_PARAMETER : 87L (The parameter is incorrect)@n
	-ERROR_INVALID_FUNCTION : 1L (Incorrect function)
	*/
	int OnHdnItemSortingList(NMHEADER* pNMHDR);

	/*!
	\brief		����Ʈ �� ��� Ŭ�� �� ���� ���
	\param[in] lParam1		������ row�� ã�� ���� ����
	\param[in] lParam2		������ row�� ã�� ���� ����
	\param[in] lParamSort	���� ���õ� ����ü
	\return
	\b ���� @n
	-bSortDirect ? wcscmp(szItem1, szItem2) : -wcscmp(szItem1, szItem2)		����Ʈ ���� �׸񳢸� ���ڿ��� ���� ���@n
	\b ���� @n
	-ERROR_INVALID_PARAMETER : 87L (The parameter is incorrect)
	*/
	static int CALLBACK CompareItem(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);

	/*!
	\brief		��µ� ����Ʈ �信�� ���ϴ� ���� �Ǵ� ���ڿ� �˻�
	\details	���ϴ� ���� �Ǵ� ���ڿ��� �κ� ���� �Ǵ� �κ� ���ڿ��̿��� �˻��� �� ������ ����Ʈ �信�� ��Ŀ�̵ȴ�.( ��ü ���� �Ǵ� ��ü ���ڿ��� �˻� ���� )
	\return
	\b ���� @n
	-ERROR_SUCCESS : 0 (����)@n
	\b ���� @n
	-ERROR_INVALID_PARAMETER : 87L (The parameter is incorrect)@n
	-ERROR_INVALID_FUNCTION : 1L (Incorrect function)
	\see		FileText_Dialog.cpp
	*/
	int OnFindText();

	/*!
	\brief		������� ���� �м� ���̾�α� �ʱ�ȭ
	\param[in] hDlg		������� ���� �м� ���̾�α� �ڵ�
	\return
	\b ���� @n
	-ERROR_SUCCESS : 0 (����)@n
	\b ���� @n
	-ERROR_INVALID_PARAMETER : 87L (The parameter is incorrect)@n
	-ERROR_INVALID_FUNCTION : 1L (Incorrect function)
	*/
	int InitDialog(HWND hDlg);

	/*!
	\brief		�޼��� �ڽ� ���
	\param[in] hDlg			������� ���� �м� ���̾�α� �ڵ�
	\param[in] szMsgText	�޼��� �ڽ��� ��� �� �ؽ�Ʈ
	\param[in] nType		�޼��� �ڽ� Ÿ��
	*/
	void MsgBox(HWND hDlg, WCHAR* szMsgText, UINT nType);
};
