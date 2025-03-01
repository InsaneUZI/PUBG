#pragma once
#include <Windows.h>
#include <string>
#include <fstream>  
#include <iostream>  
#include <map>  
#include <sstream> 


using namespace std;

namespace UiParam
{	
	inline INT CurrentPage = 0;							// ��ǰҳ��

	// ��¼
	namespace LogIn
	{
		inline CHAR Account[MAX_PATH]{ 0 };				// �û���
		inline CHAR Password[MAX_PATH]{ 0 };			// ����
		inline bool IsRemember = true;					// ��ס
	}
	
	// ע��
	namespace Register
	{
		inline CHAR Account[MAX_PATH]{ 0 };				// �û���
		inline CHAR Password[MAX_PATH]{ 0 };			// ����
		//inline CHAR SafetyCode[MAX_PATH]{ 0 };
		inline CHAR Key[MAX_PATH]{ 0 };					// ����
	}

	// ��ֵ
	namespace Pay
	{
		inline CHAR Account[MAX_PATH]{ 0 };				// �û���
		inline CHAR Key[MAX_PATH]{ 0 };					// ��ֵ����
	}

	// ����
	namespace ChangePassword
	{
		inline CHAR Account[MAX_PATH]{ 0 };				// �û���
		inline CHAR OrigPassword[MAX_PATH]{ 0 };		// ԭ����
		inline CHAR NewPassword[MAX_PATH]{ 0 };			// ������
		//inline CHAR SafetyCode[MAX_PATH]{ 0 };			// ��ȫ��
	}

	// ���
	namespace Unlink
	{
		inline CHAR Account[MAX_PATH]{ 0 };				// �û���
		inline CHAR Password[MAX_PATH]{ 0 };			// ����
	}
}


