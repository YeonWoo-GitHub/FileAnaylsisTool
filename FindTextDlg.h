#include "pch.h"
#pragma once

using namespace std;

/*!
\brief		�˻�â�� ���� Ŭ����
*/
class FindTextDlg
{
private:
	/*!
	\brief		CCISFindText Ŭ���� ������
	*/
	FindTextDlg(void);

public:

	/*!
	\brief		�� ���� CCISFindText Ŭ���� �ν��Ͻ� ����
	\return
	-inst		��ü
	*/
	static FindTextDlg* getInstance();

	/*!
	\brief		CCISFindText Ŭ���� �Ҹ���
	*/
	~FindTextDlg(void);

	WCHAR m_szFindText[MAX_PATH];			///<�˻� ���̾�α׿��� �Է¹��� ���� �Ǵ� ���ڿ�
	HWND m_hEdit;							///<�˻� ���̾�α׿��� Edit�� ���� �ڵ鰪
	HWND m_hMainDlg;						///<�˻�â�� ���� �ڵ鰪
	HICON m_hIcon;							///<�����ܿ� ���� �ڵ鰪
	HHOOK m_Hok;							///<Ű���� �Է°��� ���� Hook

	/*!
	\brief		�˻� ���̾�α� ����
	\param[in] hparentDlg		�θ� ���̾�α� �ڵ�
	\return
	\b ���� @n
	-TRUE
	*/
	int FindTextDialog(HWND hparentDlg);

	/*!
	\brief		�˻� ���̾�α� �ʱ�ȭ
	\param[in] hDlg		���� ���̾�α� �ڵ�
	\return
	\b ���� @n
	-ERROR_SUCCESS : 0 (����) @n
	\b ���� @n
	-ERROR_INVALID_FUNCTION : 1L (Incorrect function)
	*/
	int InitDialog(HWND hDlg);

	/*!
	\brief		����ڰ� �Է��� ���� �Ǵ� ���ڿ��� ����
	\param[in] hparentDlg		���� ���̾�α� �ڵ�
	\return
	\b ���� @n
	-ERROR_SUCCESS : 0 (����) @n
	\b ���� @n
	-ERROR_INVALID_FUNCTION : 1L (Incorrect function)
	*/
	int OnButtonFind(HWND hDlg);

	/*!
	\brief		����ڰ� �Է��� ���� �Ǵ� ���ڿ� ����
	\param[in] pStr				���޹޴� ���� �Ǵ� ���ڿ�
	\param[in] nBufferSize		���޹޴� ���� �Ǵ� ���ڿ��� ũ��
	\return
	\b ���� @n
	-ERROR_SUCCESS : 0 (����) @n
	\b ���� @n
	-ERROR_INVALID_PARAMETER : 87L (The parameter is incorrect)
	*/
	int GetData(WCHAR* pStr, int nBufferSize);

	/*!
	\brief		�˻�â ���̾�α׿� ���۵� �޽��� ó��
	\param[in] hDlg		���̾�α׿� ���� �ڵ�
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
	\brief		Ű���� �Է��� ���� �Լ�
	\details
	-hook�� �̿��� Ű���� �Է��� �ν��ϰ� �Լ����� �ۼ��� �ڵ� ����@n
	-����Ű ���� ��, OnButtonFind() �Լ� ȣ���Ͽ� �˻� ��� ����@n
	-ESCŰ ���� ��, �˻�â ����
	\param[in] code		Ű���� hook Ÿ��
	\param[in] wParam	�߰� �޽��� �� ����
	\param[in] IParam	�߰� �޽��� �� ����
	\return
	\b ���� @n
	-ERROR_SUCCESS : 0 (����)@n
	\b ���� @n
	-CallNextHookEx(CCISFindText::getInstance()->m_Hok, code, wParam, lParam)
	*/
	static INT_PTR CALLBACK KeybdProc(int code, WPARAM wParam, LPARAM lParam);

};
