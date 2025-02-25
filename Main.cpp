#include <winsock2.h>
#include <windows.h>
#include <Overlay/Overlay.h>
#include <Common/Data.h>
#include <Utils/Utils.h>
#include <Hack/Hack.h>
#include <Common/Offset.h>
#include <thread>
#include <DMALibrary/Memory/Memory.h>
#include <cstdint>
#include <Common/Config.h>
#include <Auth/Auth.h>
#include <Utils/MachineCodeGenerator.h>
#include <Utils/w3c/w3c.h>
#include "td.h"

#ifdef _WIN64
#pragma comment(lib,"tdx64.lib")
#else
#pragma comment(lib,"Tdx86.lib")
#endif

FGameData GameData;

using namespace std;

void saveCardKey(const std::string& cardKey) {
	std::ofstream outFile("cardKey.txt");
	if (outFile.is_open()) {
		outFile << cardKey;
		outFile.close();
	}
	/*else {
		std::cerr << "Unable to open file for saving card key.\n";
	}*/
}

std::string loadCardKey() {
	std::ifstream inFile("cardKey.txt");
	std::string cardKey;
	if (inFile.is_open()) {
		inFile >> cardKey;
		inFile.close();
	}/*
	else {
		std::cerr << "No saved card key found.\n";
	}*/
	return cardKey;
}

void deleteCardKey() {
	remove("cardKey.txt");
}

// SingleLogin | 单码用户登录
bool SingleLogin() {
	std::string HWID = MachineCodeGenerator::GenerateMachineCode();
	W3Client w3;

	W3Client client;

	bool isLoggedIn = false;
	std::string cardKey;
	TD_Init("103.143.11.35", 6666, "1", "凹凸曼");
	TD_SetMacCode(TD_GetMacCode());//置机器码，如果有自己的机器码方案可用此命令替换验证自带的取机器码方案
	char szBuf[256] = { 0 };
	cardKey = loadCardKey();
	if (!cardKey.empty()) {
		
		isLoggedIn = TD_CardLogin(cardKey.c_str(), szBuf);
		//std::cout << "登录结果: ";
		//std::cout << szBuf << std::endl;
		if (isLoggedIn) {
			char buf[1024] = "\0";
			std::cout << "Login Success" << std::endl;
			saveCardKey(cardKey);
			strncpy(GameData.Config.eyou.globalBuf, buf, 1024);
			memset(buf, 0x00, 1024);
			return TRUE;

		}
		else {
			std::cout << "Login Faild, Tips :" << szBuf << std::endl;
			deleteCardKey();  // 删除保存的卡密文件
			return FALSE;
		}
	}
	else
	{
		std::cout << "Please enter your card key: ";
		std::cin >> cardKey;
		isLoggedIn = TD_CardLogin(cardKey.c_str(), szBuf);
		std::cout << "登录结果: ";
		std::cout << szBuf << std::endl;
		if (isLoggedIn) {
			char buf[1024] = "\0";
			std::cout << "Login Success" << std::endl;
			saveCardKey(cardKey);
			strncpy(GameData.Config.eyou.globalBuf, buf, 1024);
			memset(buf, 0x00, 1024);
			return TRUE;

		}
		else {
			std::cout << "Login Faild, Tips :" << szBuf << std::endl;
			deleteCardKey();  // 删除保存的卡密文件
			return FALSE;
		}
	}

}

void SetConsoleStyle()
{
	SetConsoleOutputCP(CP_UTF8);

	SetConsoleTitle("菜鸟基地QQ群：964504044     最全特征码解密，QQ:1342674682");

	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof(cfi);
	cfi.nFont = 0;
	cfi.dwFontSize.X = 0;
	cfi.dwFontSize.Y = 14;

	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);

	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

	DWORD dwMode = 0;
	GetConsoleMode(hOut, &dwMode);
	dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	SetConsoleMode(hOut, dwMode);

	auto hStdin = ::GetStdHandle(STD_INPUT_HANDLE);
	DWORD mode;
	::GetConsoleMode(hStdin, &mode);
	mode &= ~ENABLE_QUICK_EDIT_MODE;
	::SetConsoleMode(hStdin, mode);
}

int Refresh()
{
	while (true)
	{
		mem.RefreshAll();
		Sleep(1000 * 60 * 15);
	}
}


void ConvertGBKToUTF8(const char* gbkStr, std::string& utf8Str) {
	int wideStrSize = MultiByteToWideChar(CP_ACP, 0, gbkStr, -1, NULL, 0);
	if (wideStrSize == 0) {
		cerr << "Failed to convert GBK to wide string with error: " << GetLastError() << endl;
		return;
	}
	wchar_t* wideStr = new wchar_t[wideStrSize];
	MultiByteToWideChar(CP_ACP, 0, gbkStr, -1, wideStr, wideStrSize);
	int utf8StrSize = WideCharToMultiByte(CP_UTF8, 0, wideStr, -1, NULL, 0, NULL, NULL);
	if (utf8StrSize == 0) {
		cerr << "Failed to convert wide string to UTF-8 with error: " << GetLastError() << endl;
		delete[] wideStr;
		return;
	}
	char* utf8StrC = new char[utf8StrSize];
	WideCharToMultiByte(CP_UTF8, 0, wideStr, -1, utf8StrC, utf8StrSize, NULL, NULL);

	utf8Str = std::string(utf8StrC);

	delete[] wideStr;
	delete[] utf8StrC;
}

int main() {

	if (SingleLogin() == FALSE)
	{
		return 0;
	}

	SetConsoleStyle();

	Auth::Init();

	std::thread HackThread(Hack::Init);

	std::thread OverlayThread(Overlay::Init);


	while (true)
	{
		if (GameData.Config.Window.Main) {
			Sleep(500);
			continue;
		}
		break;
	}


	return 0;

}

