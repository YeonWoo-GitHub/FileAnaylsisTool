#pragma once

#include "pch.h"
/**
	@mainpage	형상관리 파일 분석 기능 설명

	@section Intro 1. 소개
		- 형상관리 파일에 속해있는 DLL파일 중 장애가 있거나 파일버전으로 인해 설치경로가 잘못된 경우를 분석하여 출력한다.
		- WinApi를 기반으로 개발되었다.

	@section Functin_Sec 2. 기능
		- 출력 기준@n
			a. 형상관리 파일에 속해있는 DLL파일이 기존 설치경로 뿐만 아니라 환경변수 Path에 적혀있는 경로에도 존재할 경우 장애로 판단하여 출력한다.@n
			b. 형상관리 파일에 속해있는 DLL파일이 기존 설치경로에 존재하지 않고 환경변수 Path에 적혀있는 경로에만 존재할 경우 장애로 판단하여 출력한다.@n
			c. 형상관리 파일에 속해있는 DLL파일이 어느경로에도 존재하지 않을 경우 장애로 판단하여 출력한다.@n
			d. 형상관리 파일에 속해있는 DLL파일의 버전(x64, x86)에 따라 기존 설치경로가 잘못됐고 환경변수 Path에 적혀있는 경로가 올바른 경우 출력한다.@n
			e. 형상관리 파일에 속해있는 DLL파일의 버전(x64, x86)에 따라 기존 설치경로와 환경변수 Path에 적혀있는 경로 둘 다 잘못된 경우@n
		- 형상관리 파일(CSV파일)의 분석 결과 중 검색창을 통해 원하는 문자 또는 문자열을 검색하여 볼 수 있다.
		- 형상관리 파일(CSV파일)의 분석 결과를 CSV파일 형식으로 저장할 수 있다.
 */



#pragma once

using namespace std;

#define NO_FILE						0x00000001			///<파일없음
#define CORRECT_FILEPATH			0x00000002			///<올바른 파일경로
#define NON_STATIONARY_FILEPATH		0x00000004			///<비정상적인 파일경로
#define INCORRECT_FILEPATH			0x00000008			///<올바르지 않은 파일경로

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
\brief		수집한 형상관리 파일 데이터를 저장하는 구조체
*/
typedef struct File_Info
{
	WCHAR File_Name[MAX_PATH];								///<모듈파일이름
	WCHAR CSVInfo_FilePath[MAX_PATH];						///<모듈파일 설치경로
	DWORD File_State;										///<모듈파일 상태(형상관리 파일내에서 오류여부)
	DWORD SysEnv_State;										///<환경변수 경로 상태
	DWORD FileVer_CollectSysEnv_State;						///<모듈파일 버전에 의한 올바른 환경변수 경로 상태
	DWORD FileVer_InCollectSysEnv_State;					///<모듈파일 버전에 의한 올바르지 못한 환경변수 경로 상태
	DWORD FileVersion_Flag;									///<모듈파일 버전
	vector<wstring>SysEnv_FilePath;							///<환경변수 Path에서 수집한 환경변수 경로 데이터
	vector<wstring>FileVer_CollectSysEnv_FilePath;			///<모듈파일 버전에 의한 올바른 환경변수 경로 데이터
	vector<wstring>FileVer_InCollectSysEnv_FilePath;		///<모듈파일 버전에 의한 올바르지 못한 환경변수 경로 데이터
	WCHAR Exp[MAX_PATH];									///<비고
} File_Info, * pFile_Info;

/*!
\brief		리스트 뷰 정렬 시 사용하는 구조체
*/
struct SORTPARAM
{
	int iSortColumn;				///<정렬 시 확인할 리스트 뷰 항목
	bool bSortDirect;				///<정렬 방식(오름차순 또는 내림차순)
	HWND hListView_HWnd;			///<정렬할 리스트 뷰 핸들 정보
};

/*!
\brief		형상관리 파일의 분석을 위한 클래스 정의
*/
class AnaylsisMainDlg
{
private:

	WCHAR m_szCSVFilePath[MAX_PATH];				///<형상관리 파일의 설치 경로					
	WCHAR m_szSave_CSVFile[MAX_PATH];				///<저장할 파일의 경로
	WCHAR m_szMsgTitle[MAX_PATH];					///<메세지 박스 타이틀
	vector<wstring> m_vecSystemEnvPath_Info;		///<환경변수 Path 데이터 정보
	vector<File_Info> m_vecFile_Info;				///<형상관리 파일 데이터 정보

	vector<wstring> m_FileData;						///<형상관리 파일에서 한 줄 데이터 정보				

	/*!
	\brief		CCISFileDlg 클래스 생성자
	*/
	AnaylsisMainDlg(void);
public:

	/*!
	\brief		CCISFileDlg 클래스 소멸자
	*/
	~AnaylsisMainDlg(void);

	/*!
	\brief		한 개의 CCISFileDlg 클래스 인스턴스 생성
	\return
	-inst		객체
	*/
	static AnaylsisMainDlg* getInstance();

	/*!
	\brief		윈도우 좌표값
	\details	윈도우 좌표값으로 윈도우 최소 크기와 리사이즈를 조절한다
	*/
	RECT m_Wndrect;

	HWND m_hListHWnd;			///<리스트 뷰 핸들값
	HWND m_hHeader;				///<리스트 뷰 헤더 핸들값
	HWND m_hMainDlg;			///<형상관리 파일 분석 다이얼로그에 대한 핸들값
	HDROP m_hDropInfo;			///<드래그 앤 드롭한 파일 정보 핸들

	/*!
	\brief		정렬 방식(오름차순 또는 내림차순)
	\details	리스트 뷰 정렬 시 오름차순 또는 내림차순으로 정렬하는 방식
	*/
	BOOL m_bAscending;
	HHOOK m_Hok;				///<키보드 입력값을 위한 Hook
	HICON m_hIcon;				///<아이콘에 대한 핸들값
	int m_Sizecx;				///<윈도우 최소 사이즈에서 세로길이
	int m_Sizecy;				///<윈도우 최소 사이즈에서 가로길이

	/*!
	\brief		형상관리 파일 분석 다이얼로그 생성
	\param[in] hDlg		형상관리 파일 분석 다이얼로그에 대한 핸들
	\return
	\b 성공 @n
	-ERROR_SUCCESS : 0 (성공)
	*/
	int ConfigManagementFile_Analysis_Dialog(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow);

	/*!
	\brief		형상관리 파일 분석 다이얼로그에 전송된 메시지 처리
	\param[in] hDlg		형상관리 파일 분석 다이얼로그에 대한 핸들
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
	\brief		리스트 뷰의 헤더초기화
	\param[in]	초기화 시 넘겨받는 리스트 뷰 핸들
	\return
	\b 성공 @n
	-ERROR_SUCCESS : 0 (성공) @n
	\b 실패 @n
	-ERROR_INVALID_PARAMETER : 87L (The parameter is incorrect)
	*/
	int ListView_Init(HWND hListView);

	/*!
	\brief		파일 드래그 앤 드롭
	\param[in] hMainDlg			형상관리 파일 분석 다이얼로그 핸들
	\param[in] hDropInfo		드래그 앤 드롭한 파일 정보 핸들
	\param[in] hListView		리스트 뷰 핸들
	\return
	\b 성공 @n
	-ERROR_SUCCESS : 0 (성공) @n
	\b 실패 @n
	-ERROR_INVALID_FUNCTION : 1L (Incorrect function)@n
	-ERROR_INVALID_PARAMETER : 87L (The parameter is incorrect)
	*/
	int OnDropFiles(HWND hMainDlg, HDROP hDropInfo, HWND hListView);

	/*!
	\brief		파일열기 대화상자
	\details	파일열기 대화상자를 통해 형상관리 파일을 넘겨받음
	\param[in] hMainDlg			형상관리 파일 분석 다이얼로그 핸들
	\param[in] hListView		리스트 뷰 핸들
	\return
	\b 성공 @n
	-ERROR_SUCCESS : 0 (성공) @n
	\b 실패 @n
	-ERROR_INVALID_FUNCTION : 1L (Incorrect function)@n
	-ERROR_INVALID_PARAMETER : 87L (The parameter is incorrect)
	*/
	int OnOpenCSVFile(HWND hMainDlg, HWND hListView);

	/*!
	\brief		형상관리 파일 분석의 내부로직의 흐름을 제어
	\details	형상관리 파일과 환경변수 Path 정보의 수집과 검증작업 후 결과를 리스트 뷰로 출력
	\param[in] hMainDlg			형상관리 파일 분석 다이얼로그 핸들
	\param[in] hListView		리스트 뷰 핸들
	\return
	\b 성공 @n
	-ERROR_SUCCESS : 0 (성공)@n
	\b 실패 @n
	-ERROR_INVALID_PARAMETER : 87L (The parameter is incorrect)@n
	-ERROR_INVALID_FUNCTION : 1L (Incorrect function)
	*/
	int InteriorLogicFunc(HWND hMainDlg, HWND hListView);

	/*!
	\brief			형상관리 파일의 데이터 수집
	\details
	-형상관리 파일 내에서 DLL파일의 데이터만 수집@n
	-수집한 DLL파일의 데이터 중 파일의 '파일명, 설치위치, 버전' 정보만 저장
	\param[in] hMainDlg		형상관리 파일 분석 다이얼로그 핸들
	\param[in] pCSVFilePath	형상관리 파일의 경로
	\param[out] pdInfo		수집한 DLL파일의 데이터 중 파일의 '파일명, 설치위치, 버전' 정보 저장
	\return
	\b 성공 @n
	-ERROR_SUCCESS : 0 (성공)@n
	\b 실패 @n
	-ERROR_INVALID_PARAMETER : 87L (The parameter is incorrect)@n
	-ERROR_INVALID_DATA : 13L (The data is invalid)@n
	-ERROR_TOO_MANY_OPEN_FILES : 4L (The system cannot open the file.)@n
	-ERROR_INVALID_FUNCTION : 1L (Incorrect function)
	*/
	int Read_CSVFile(HWND hMainDlg, WCHAR* pCSVFilePath, std::vector<File_Info>* pdInfo);

	/*!
	\brief			환경변수 Path 데이터 수집
	\details		환경변수 Path 데이터 수집 중 'softcmap' 문자열이 있는 경우는 제외
	\param[in] hMainDlg		형상관리 파일 분석 다이얼로그 핸들
	\param[out] pSystemEnv	환경변수 Path 데이터 저장
	\return
	\b 성공 @n
	-ERROR_SUCCESS : 0 (성공)@n
	\b 실패 @n
	-ERROR_INVALID_PARAMETER : 87L (The parameter is incorrect)@n
	-ERROR_ENVVAR_NOT_FOUND : 203L (The system could not find the environment option that was entered)@n
	-ERROR_NOT_ENOUGH_MEMORY : 8L (Not enough memory resources are available to process this command)@n
	-ERROR_INVALID_DATA : 13L (The data is invalid)
	*/
	int Read_SystemEnvironment(HWND hMainDlg, std::vector<wstring>* pSystemEnv);

	/*!
	\brief			파일의 인코딩 확인
	\details
	-파일의 인코딩이 Unicode 또는 Ansi인지 확인하여 둘 중 하나를 리턴함@n
	-파일의 인코딩이 UTF-8인 경우 형상관리 파일 분석 불가능( 인코딩을 Unicode 또는 Ansi로 바꿔야 함 )
	\param[in] hMainDlg		형상관리 파일 분석 다이얼로그 핸들
	\param[in] pCSVFilePath	형상관리 파일의 경로
	\return
	\b 성공 @n
	-Encoding : Unicode 또는 Ansi 반환@n
	\b 실패 @n
	-ERROR_INVALID_PARAMETER : 87L (The parameter is incorrect)@n
	-ERROR_INVALID_FUNCTION : 1L (Incorrect function)
	*/
	int GetFileEncoding(HWND hMainDlg, WCHAR* pCSVFilePath);

	/*!
	\brief			형상관리 파일 데이터 파싱
	\details		형상관리 파일 데이터에서 쉼표를 구분자로 문자열 파싱
	\param[in] pFileLine		형상관리 파일에서 한 줄
	\return
	\b 성공 @n
	-ERROR_SUCCESS : 0 (성공)@n
	\b 실패 @n
	-ERROR_INVALID_PARAMETER : 87L (The parameter is incorrect)@n
	-ERROR_INVALID_DATA : 13L (The data is invalid)
	*/
	int FileDataCollect(WCHAR* pFileLine);

	/*!
	\brief			형상관리 파일에서 수집한 데이터 검증
	\details		수집한 데이터 중 모듈 파일의 '설치위치'에 파일의 존재여부 확인 후 저장
	\param[in] hMainDlg		형상관리 파일 분석 다이얼로그 핸들
	\param[out] pdInfo		'설치위치'에 따른 파일의 존재여부 저장
	\return
	\b 성공 @n
	-ERROR_SUCCESS : 0 (성공)@n
	\b 실패 @n
	-ERROR_INVALID_PARAMETER : 87L (The parameter is incorrect)@n
	-ERROR_INVALID_DATA : 13L (The data is invalid)@n
	-ERROR_INVALID_FUNCTION : 1L (Incorrect function)
	*/
	int Verification_CSVFile(HWND hMainDlg, File_Info* pdInfo);

	/*!
	\brief			환경변수 Path에서 수집한 데이터 검증
	\details		수집한 데이터 중 경로에 모듈 파일의 존재여부 확인 후 저장
	\param[in] pSystemEnv		환경변수 Path 데이터
	\param[in] pdInfo			검증할 파일명 가져오고 파일이 존재하는 환경변수 Path 데이터 저장
	\return
	\b 성공 @n
	-ERROR_SUCCESS : 0 (성공)@n
	\b 실패 @n
	-ERROR_INVALID_PARAMETER : 87L (The parameter is incorrect)@n
	-ERROR_INVALID_DATA : 13L (The data is invalid)@n
	-ERROR_INVALID_FUNCTION : 1L (Incorrect function)
	*/
	int Verification_SystemEnvironment(std::vector<wstring>* pSystemEnv, File_Info* pdInfo);

	/*!
	\brief			검증한 데이터들을 리스트 뷰로 결과 출력
	\param[in] hListHWnd		형상관리 파일 분석 다이얼로그 핸들
	\param[in] pdInfo			리스트 뷰로 출력할 데이터들 선택
	\return
	\b 성공 @n
	-ERROR_SUCCESS : 0 (성공)@n
	\b 실패 @n
	-ERROR_INVALID_PARAMETER : 87L (The parameter is incorrect)
	*/
	int ListView_Print(HWND hListHWnd, vector<File_Info>* pdInfo);

	/*!
	\brief			파일저장 다이얼로그
	\param[in] hListHWnd		형상관리 파일 분석 다이얼로그 핸들
	\param[in] hListView		리스트 뷰 핸들
	\return
	\b 성공 @n
	-ERROR_SUCCESS : 0 (성공)@n
	\b 실패 @n
	-ERROR_INVALID_PARAMETER : 87L (The parameter is incorrect)@n
	-ERROR_INVALID_FUNCTION : 1L (Incorrect function)
	*/
	int OnFileSave(HWND hMainDlg, HWND hListView);

	/*!
	\brief			파일저장 내부로직
	\details		리스트 뷰로 출력된 내용을 CSV파일 형식으로 저장한다
	\param[in] hListHWnd		리스트 뷰 핸들
	\param[in] pCSVFilePath		형상관리 파일 경로
	\return
	\b 성공 @n
	-ERROR_SUCCESS : 0 (성공)@n
	\b 실패 @n
	-ERROR_INVALID_PARAMETER : 87L (The parameter is incorrect)@n
	-ERROR_INVALID_FUNCTION : 1L (Incorrect function)
	*/
	int FileSave(HWND hListHWnd, WCHAR* pCSVFilePath);

	/*!
	\brief		키보드 입력을 위한 함수
	\details
	-hook을 이용해 키보드 입력을 인식하고 함수에 작성한 코드 수행@n
	-ESC키 누를 시, 종료 방지
	\param[in] code		키보드 hook 타입
	\param[in] wParam	추가 메시지 별 정보
	\param[in] IParam	추가 메시지 별 정보
	\return
	\b 성공 @n
	-ERROR_SUCCESS : 0 (성공)@n
	-TRUE@n
	\b 실패 @n
	-CallNextHookEx(CCISFileDlg::getInstance()->m_Hok, code, wParam, lParam)
	*/
	static INT_PTR CALLBACK KeybdProc(int code, WPARAM wParam, LPARAM lParam);

	/*!
	\brief			리스트 뷰에서 마우스 왼쪽 버튼으로 더블클릭 시 나타나는 이벤트
	\details		'경로'를 더블클릭 시 해당 경로의 파일 탐색기 출력
	\param[in] pNMHDR	클릭 된 리스트 뷰 행 정보
	\return
	\b 성공 @n
	-ERROR_SUCCESS : 0 (성공)@n
	\b 실패 @n
	-ERROR_INVALID_PARAMETER : 87L (The parameter is incorrect)@n
	-ERROR_INVALID_FUNCTION : 1L (Incorrect function)
	*/
	int OnDblclkListToFolder(NMHEADER* pNMHDR);

	/*!
	\brief			리스트 뷰 헤더 클릭 시 정렬 이벤트
	\param[in] pNMHDR	클릭 된 리스트 뷰 헤더 정보
	\return
	\b 성공 @n
	-ERROR_SUCCESS : 0 (성공)@n
	\b 실패 @n
	-ERROR_INVALID_PARAMETER : 87L (The parameter is incorrect)@n
	-ERROR_INVALID_FUNCTION : 1L (Incorrect function)
	*/
	int OnHdnItemSortingList(NMHEADER* pNMHDR);

	/*!
	\brief		리스트 뷰 헤더 클릭 시 정렬 기능
	\param[in] lParam1		정렬할 row를 찾기 위한 변수
	\param[in] lParam2		정렬할 row를 찾기 위한 변수
	\param[in] lParamSort	정렬 관련된 구조체
	\return
	\b 성공 @n
	-bSortDirect ? wcscmp(szItem1, szItem2) : -wcscmp(szItem1, szItem2)		리스트 뷰의 항목끼리 문자열을 비교한 결과@n
	\b 실패 @n
	-ERROR_INVALID_PARAMETER : 87L (The parameter is incorrect)
	*/
	static int CALLBACK CompareItem(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);

	/*!
	\brief		출력된 리스트 뷰에서 원하는 문자 또는 문자열 검색
	\details	원하는 문자 또는 문자열이 부분 문자 또는 부분 문자열이여도 검색할 수 있으며 리스트 뷰에서 포커싱된다.( 전체 문자 또는 전체 문자열도 검색 가능 )
	\return
	\b 성공 @n
	-ERROR_SUCCESS : 0 (성공)@n
	\b 실패 @n
	-ERROR_INVALID_PARAMETER : 87L (The parameter is incorrect)@n
	-ERROR_INVALID_FUNCTION : 1L (Incorrect function)
	\see		FileText_Dialog.cpp
	*/
	int OnFindText();

	/*!
	\brief		형상관리 파일 분석 다이얼로그 초기화
	\param[in] hDlg		형상관리 파일 분석 다이얼로그 핸들
	\return
	\b 성공 @n
	-ERROR_SUCCESS : 0 (성공)@n
	\b 실패 @n
	-ERROR_INVALID_PARAMETER : 87L (The parameter is incorrect)@n
	-ERROR_INVALID_FUNCTION : 1L (Incorrect function)
	*/
	int InitDialog(HWND hDlg);

	/*!
	\brief		메세지 박스 출력
	\param[in] hDlg			형상관리 파일 분석 다이얼로그 핸들
	\param[in] szMsgText	메세지 박스에 출력 할 텍스트
	\param[in] nType		메세지 박스 타입
	*/
	void MsgBox(HWND hDlg, WCHAR* szMsgText, UINT nType);
};
