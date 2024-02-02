#pragma once

// ���ҽ� ���� ��
#define POLICY_FILE			L"FileAnaylsisTool.ini"

// ���ҽ�
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
\brief		���ҽ� ������ ���� Ŭ����
*/
class Policy
{
	/*!
	\brief		Policy Ŭ���� ������
	*/
	Policy(void);
public:

	/*!
	\brief		Policy Ŭ���� �Ҹ���
	*/
	~Policy(void);

	/*!
	\brief		�� ���� Policy Ŭ���� �ν��Ͻ� ����
	\return
	-inst		��ü
	*/
	static Policy* getInstance();

	/*!
	\brief		ini ���Ͽ��� ���ҽ� ������
	\param[in] lpszSection			ini ���Ͽ����� ���� �κ�
	\param[in] lpszIdString			ini ���Ͽ����� key ��
	\param[out] lpReturnedString	ini ���Ͽ����� value ��
	\param[in] nSize				lpReturnedString ������
	\return
	\b ���� @n
	-TRUE
	\b ���� @n
	-FALSE
	*/
	BOOL GetPolicy(LPWSTR lpszSection, LPWSTR lpszIdString, LPWSTR lpReturnedString, DWORD nSize);
};

