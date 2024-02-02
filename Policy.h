#pragma once

// 리소스 파일 명
#define POLICY_FILE			L"FileAnaylsisTool.ini"

// 리소스
#define  MAIN_SECTION		L"FILE_ANALYSIS_TOOL_DLG"
#define  SECOND_SECTION		L"FIND_TEXT_DLG"
#define  THIRD_SECTION		L"MSG"

#define  DLG_TITLE			L"DLG_TITLE"
#define  MSG_TITLE			L"MSG_TITLE"

#define  NUM								L"NUM"
#define  MODULEFILE_NAME					L"MODULEFILE_NAME"
#define  MODULEFILE_PATH					L"MODULEFILE_PATH"
#define  EXIST								L"EXIST"
#define  ENVIRONMENT_VARIABLE_PATH			L"ENVIRONMENT_VARIABLE_PATH"
#define  EXP								L"EXP"

#define  MSG_SELECT_ONE_FILE				L"MSG_SELECT_ONE_FILE"
#define  MSG_ANALYSIS_FILE_FAIL				L"MSG_ANALYSIS_FILE_FAIL"
#define  MSG_SELECT_FILE					L"MSG_SELECT_FILE"
#define  MSG_SELECT_FILE_WITH_ENCODING		L"MSG_SELECT_FILE_WITH_ENCODING"
#define  MSG_SELECT_FILE_FIRST				L"MSG_SELECT_FILE_FIRST"
#define  MSG_FILE_SAVE_FAIL					L"MSG_FILE_SAVE_FAIL"
#define  MSG_NO_STRING						L"MSG_NO_STRING"

#define  TITLE_STATE_STRING					L"TITLE_STATE_STRING"
#define  STRING_EXP1						L"STRING_EXP1"
#define  STRING_EXP2						L"STRING_EXP2"
#define  STRING_EXP3						L"STRING_EXP3"
#define  STRING_EXP4						L"STRING_EXP4"
#define  STRING_EXP5						L"STRING_EXP5"


/*!
\brief		리소스 관리를 위한 클래스
*/
class Policy
{
	/*!
	\brief		Policy 클래스 생성자
	*/
	Policy(void);
public:

	/*!
	\brief		Policy 클래스 소멸자
	*/
	~Policy(void);

	/*!
	\brief		한 개의 Policy 클래스 인스턴스 생성
	\return
	-inst		객체
	*/
	static Policy* getInstance();

	/*!
	\brief		ini 파일에서 리소스 가져옴
	\param[in] lpszSection			ini 파일에서의 섹션 부분
	\param[in] lpszIdString			ini 파일에서의 key 값
	\param[out] lpReturnedString	ini 파일에서의 value 값
	\param[in] nSize				lpReturnedString 사이즈
	\return
	\b 성공 @n
	-TRUE
	\b 실패 @n
	-FALSE
	*/
	BOOL GetPolicy(LPWSTR lpszSection, LPWSTR lpszIdString, LPWSTR lpReturnedString, DWORD nSize);
};

