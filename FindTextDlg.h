#include "pch.h"
#pragma once

using namespace std;

/*!
\brief		검색창을 위한 클래스
*/
class FindTextDlg
{
private:
	/*!
	\brief		CCISFindText 클래스 생성자
	*/
	FindTextDlg(void);

public:

	/*!
	\brief		한 개의 CCISFindText 클래스 인스턴스 생성
	\return
	-inst		객체
	*/
	static FindTextDlg* getInstance();

	/*!
	\brief		CCISFindText 클래스 소멸자
	*/
	~FindTextDlg(void);

	WCHAR m_szFindText[MAX_PATH];			///<검색 다이얼로그에서 입력받은 문자 또는 문자열
	HWND m_hEdit;							///<검색 다이얼로그에서 Edit에 대한 핸들값
	HWND m_hMainDlg;						///<검색창에 대한 핸들값
	HICON m_hIcon;							///<아이콘에 대한 핸들값
	HHOOK m_Hok;							///<키보드 입력값을 위한 Hook

	/*!
	\brief		검색 다이얼로그 생성
	\param[in] hparentDlg		부모 다이얼로그 핸들
	\return
	\b 성공 @n
	-TRUE
	*/
	int FindTextDialog(HWND hparentDlg);

	/*!
	\brief		검색 다이얼로그 초기화
	\param[in] hDlg		본인 다이얼로그 핸들
	\return
	\b 성공 @n
	-ERROR_SUCCESS : 0 (성공) @n
	\b 실패 @n
	-ERROR_INVALID_FUNCTION : 1L (Incorrect function)
	*/
	int InitDialog(HWND hDlg);

	/*!
	\brief		사용자가 입력한 문자 또는 문자열을 저장
	\param[in] hparentDlg		본인 다이얼로그 핸들
	\return
	\b 성공 @n
	-ERROR_SUCCESS : 0 (성공) @n
	\b 실패 @n
	-ERROR_INVALID_FUNCTION : 1L (Incorrect function)
	*/
	int OnButtonFind(HWND hDlg);

	/*!
	\brief		사용자가 입력한 문자 또는 문자열 전달
	\param[in] pStr				전달받는 문자 또는 문자열
	\param[in] nBufferSize		전달받는 문자 또는 문자열의 크기
	\return
	\b 성공 @n
	-ERROR_SUCCESS : 0 (성공) @n
	\b 실패 @n
	-ERROR_INVALID_PARAMETER : 87L (The parameter is incorrect)
	*/
	int GetData(WCHAR* pStr, int nBufferSize);

	/*!
	\brief		검색창 다이얼로그에 전송된 메시지 처리
	\param[in] hDlg		다이얼로그에 대한 핸들
	\param[in] message	메시지
	\param[in] wParam	추가 메시지 별 정보
	\param[in] IParam	추가 메시지 별 정보
	\return
	\b 성공 @n
	-TRUE@n
	\b 실패 @n
	-FALSE
	*/
	static INT_PTR CALLBACK DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	/*!
	\brief		키보드 입력을 위한 함수
	\details
	-hook을 이용해 키보드 입력을 인식하고 함수에서 작성한 코드 수행@n
	-엔터키 누를 시, OnButtonFind() 함수 호출하여 검색 기능 수행@n
	-ESC키 누를 시, 검색창 종료
	\param[in] code		키보드 hook 타입
	\param[in] wParam	추가 메시지 별 정보
	\param[in] IParam	추가 메시지 별 정보
	\return
	\b 성공 @n
	-ERROR_SUCCESS : 0 (성공)@n
	\b 실패 @n
	-CallNextHookEx(CCISFindText::getInstance()->m_Hok, code, wParam, lParam)
	*/
	static INT_PTR CALLBACK KeybdProc(int code, WPARAM wParam, LPARAM lParam);

};
