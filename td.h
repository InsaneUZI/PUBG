#pragma once
#include <windows.h>
#define ��֤����_��¼�ɹ�����     1
#define ��֤����_������           2
#define ��֤����_ʣ��ʱ��         3
#define ��֤����_����汾         4
#define ��֤����_��¼�ɹ��������� 5
#define ��֤����_��֤�ɹ�����     6
#define ��֤����_������           7
#define ��֤����_��֤�ɹ��Ļ����� 8
#define ��֤����_��ע             9
#define ��֤����_�û�����Ϣ     10
#define ��֤����_����IP��ַ       11
#define ��֤����_����IP����λ��   12
#define ��֤����_��¼ʱ��         13
#define ��֤����_�û�ע��ʱ��     14
#define ��֤����_��¼���˺�       15
#define ��֤����_QQ               16
#define ��֤����_�ƹ�Ա           17
#define ��֤����_����ʱ��         18
#define ��֤����_���õ���         19
#define ��֤����_�������         20
#define ��֤����_����Ա           21

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

//���_��ʼ��
void TD_Init(const char* szIp, WORD wPort, const char* szSoftVer, const char* szSoftName);
//���_ȡΨһ������
char* TD_GetMacCode();
//���_����¼
bool TD_CardLogin(const char* szCard, char* szResult);
//���_����֤��������
char* TD_GetRetData(int nIdx);
//���_ȡ����������
char* TD_GetStaticData();
//���_���Ž��
char* TD_UnBindCard(const char* szCard);
//���_JS����
char* TD_RunJs(const char* szJs);
//���_�Ƽ���
char* TD_Cloud(const char* szBuf, UINT nLen);
//���_Ƶ����֤
bool TD_PingLv();
//���_�⿨
char* TD_BanCard(const char* szCard, const char* szReason);
//���_��ѯ����
int TD_QueryPoint();
//���_�۵�
bool TD_DedPoint(UINT nCnt);
//���_���ط������ļ�
char* TD_DownFile(const char* szFileName, UINT* nRecvLen);
//���_������������汾
char* TD_GetSoftVersion();
//���_�û���¼
bool TD_UserLogin(const char* szUser, const char* szPass, char* szResult);
//���_�û�ע��
char* TD_UserReg(const char* szUser, const char* szPass, const char* szCard);
//���_�û���ֵ
char* TD_UserRecharge(const char* szUser, const char* szCard);
//���_�û����
char* TD_UserUnbind(const char* szUser, const char* szPass);
//���_�û�����
char* TD_UserEditPass(const char* szUser, const char* szOldPass, const char* szNewPass);
//����_�û�����
bool TD_SetMacCode(const char* SetCode);