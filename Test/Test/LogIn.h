
#include "Common.h"
#include "MySQL.h"
class LogInWnd :public WindowImplBase {
protected:
	//��¼����ͬ���̳�  �������������, Ҳ��Ҫʵ�����������麯��
	CDuiString GetSkinFolder();
	CDuiString GetSkinFile();
	LPCTSTR GetWindowClassName(void) const ;
	void LogIn();
public:
	MySQL* p_mysql;
	//�̳��ڻ������Ӧ��Ϣ�ĺ���
	void Notify(TNotifyUI& msg);
};