#pragma once
#include <windows.h>
#define 验证返回_登录成功次数     1
#define 验证返回_附属性           2
#define 验证返回_剩余时间         3
#define 验证返回_软件版本         4
#define 验证返回_登录成功返回数据 5
#define 验证返回_验证成功卡号     6
#define 验证返回_卡类型           7
#define 验证返回_验证成功的机器码 8
#define 验证返回_备注             9
#define 验证返回_用户绑定信息     10
#define 验证返回_本机IP地址       11
#define 验证返回_本机IP地理位置   12
#define 验证返回_登录时间         13
#define 验证返回_用户注册时间     14
#define 验证返回_登录的账号       15
#define 验证返回_QQ               16
#define 验证返回_推广员           17
#define 验证返回_到期时间         18
#define 验证返回_可用点数         19
#define 验证返回_绑机数量         20
#define 验证返回_销售员           21

#ifdef _WIN64
#define VMPBegin VMProtectBeginVirtualization(__FUNCTION__);
#define VMPEnd VMProtectEnd();
#else
#define VMPBegin __asm _emit 0xEB\
	__asm _emit 0x10\
	__asm _emit 0x56\
	__asm _emit 0x4D\
	__asm _emit 0x50\
	__asm _emit 0x72\
	__asm _emit 0x6F\
	__asm _emit 0x74\
	__asm _emit 0x65\
	__asm _emit 0x63\
	__asm _emit 0x74\
	__asm _emit 0x20\
	__asm _emit 0x62\
	__asm _emit 0x65\
	__asm _emit 0x67\
	__asm _emit 0x69\
	__asm _emit 0x6E\
	__asm _emit 0x00;
#define VMPEnd __asm _emit 0xEB\
	__asm _emit 0x0E\
	__asm _emit 0x56\
	__asm _emit 0x4D\
	__asm _emit 0x50\
	__asm _emit 0x72\
	__asm _emit 0x6F\
	__asm _emit 0x74\
	__asm _emit 0x65\
	__asm _emit 0x63\
	__asm _emit 0x74\
	__asm _emit 0x20\
	__asm _emit 0x65\
	__asm _emit 0x6E\
	__asm _emit 0x64\
	__asm _emit 0x00;
#endif

void OutputDebugStringV(LPCTSTR lpFormat, ...);

//天盾_初始化
void TD_Init(const char* szIp, WORD wPort, const char* szSoftVer, const char* szSoftName);
//天盾_取唯一机器码
char* TD_GetMacCode();
//天盾_卡登录
bool TD_CardLogin(const char* szCard, char* szResult);
//天盾_读验证返回数据
char* TD_GetRetData(int nIdx);
//天盾_取服务器数据
char* TD_GetStaticData();
//天盾_卡号解绑
char* TD_UnBindCard(const char* szCard);
//天盾_JS运算
char* TD_RunJs(const char* szJs);
//天盾_云计算
char* TD_Cloud(const char* szBuf, UINT nLen);
//天盾_频率验证
bool TD_PingLv();
//天盾_封卡
char* TD_BanCard(const char* szCard, const char* szReason);
//天盾_查询点数
int TD_QueryPoint();
//天盾_扣点
bool TD_DedPoint(UINT nCnt);
//天盾_下载服务器文件
char* TD_DownFile(const char* szFileName, UINT* nRecvLen);
//天盾_读服务器软件版本
char* TD_GetSoftVersion();
//天盾_用户登录
bool TD_UserLogin(const char* szUser, const char* szPass, char* szResult);
//天盾_用户注册
char* TD_UserReg(const char* szUser, const char* szPass, const char* szCard);
//天盾_用户充值
char* TD_UserRecharge(const char* szUser, const char* szCard);
//天盾_用户解绑
char* TD_UserUnbind(const char* szUser, const char* szPass);
//天盾_用户改密
char* TD_UserEditPass(const char* szUser, const char* szOldPass, const char* szNewPass);
//辅助_置机器码
bool TD_SetMacCode(const char* SetCode);