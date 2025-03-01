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
	inline INT CurrentPage = 0;							// 当前页面

	// 登录
	namespace LogIn
	{
		inline CHAR Account[MAX_PATH]{ 0 };				// 用户名
		inline CHAR Password[MAX_PATH]{ 0 };			// 密码
		inline bool IsRemember = true;					// 记住
	}
	
	// 注册
	namespace Register
	{
		inline CHAR Account[MAX_PATH]{ 0 };				// 用户名
		inline CHAR Password[MAX_PATH]{ 0 };			// 密码
		//inline CHAR SafetyCode[MAX_PATH]{ 0 };
		inline CHAR Key[MAX_PATH]{ 0 };					// 卡密
	}

	// 充值
	namespace Pay
	{
		inline CHAR Account[MAX_PATH]{ 0 };				// 用户名
		inline CHAR Key[MAX_PATH]{ 0 };					// 充值卡密
	}

	// 改密
	namespace ChangePassword
	{
		inline CHAR Account[MAX_PATH]{ 0 };				// 用户名
		inline CHAR OrigPassword[MAX_PATH]{ 0 };		// 原密码
		inline CHAR NewPassword[MAX_PATH]{ 0 };			// 新密码
		//inline CHAR SafetyCode[MAX_PATH]{ 0 };			// 安全码
	}

	// 解绑
	namespace Unlink
	{
		inline CHAR Account[MAX_PATH]{ 0 };				// 用户名
		inline CHAR Password[MAX_PATH]{ 0 };			// 密码
	}
}


