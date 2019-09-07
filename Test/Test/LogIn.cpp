#include "LogIn.h"
#include "MainWnd.h"
#include "Cashier.h"
CDuiString LogInWnd::GetSkinFolder() {
	return _T("");
}
CDuiString LogInWnd::GetSkinFile() {
	return _T("LogInWnd.xml");
}
LPCTSTR LogInWnd::GetWindowClassName(void) const {
	return _T("LogInWnd");
}

//��Ϣ������
void LogInWnd::Notify(TNotifyUI& msg) {



	//tagTNotifyUI�ṹ�������װ��һ�� CControlUI* pSender���͵�ָ�� CControlUI�������װ�˻�ȡ�ռ����Ƶķ���
	CDuiString StrName = msg.pSender->GetName();

	//����Ϣ��������  ,msg.sType ����Ϣ������   ,���ص��ǵ����Ϣ
	if (msg.sType == _T("click")) {
		if (StrName == _T("Btn_MIN"))
			//MessageBox(NULL, _T("MIN"), _T("Login"), IDOK); //һ�������ı�,MessageBox��ʾһ���Ի���
			SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
		else if (StrName == _T("Btn_CLOSE"))
			Close();
		else if (StrName == _T("Btn_LOGIN"))
			LogIn();
	}
}

void LogInWnd::LogIn() {
	//�ӱ༭����ȡ���û���������,��ʱ���������ݿ��е����ݽ��жԱ�
	CEditUI* pUserName = (CEditUI*)m_PaintManager.FindControl(_T("Edit_USER_NAME"));//���û����Ŀռ��������û�������û���
	CDuiString StrUserName = pUserName->GetText();	//�õ��û�������û���

	//��ȡ�û�����
	CEditUI* pUserPassword = (CEditUI*)m_PaintManager.FindControl(_T("Edit_USER_PASSWORD"));
	CDuiString StrUserPassword = pUserPassword->GetText();

	//����û��Ƿ��������û���������,���û����,�����û�����
	if (StrUserName.IsEmpty()) {
		MessageBox(NULL, _T("�������û���"), _T("����"), IDOK);
		return;
	}
	if (StrUserPassword.IsEmpty()) {
		MessageBox(NULL, _T("����������"), _T("����"), IDOK);
		return;
	}

	MySQL mysql;
	p_mysql = &mysql;
	p_mysql->ConnectionMySql("127.0.0.1", "root", "", "weihang");

	//�����û���������,�������ݿ�,�������ݿ��е������Ƿ�ƥ��
	string StrSQL("select * from employee where name='");
	//AscToUnicode
	StrSQL += UnicodeToAsc(StrUserName);
	StrSQL += "';";
	vector<vector<string>> vRet = p_mysql->Select(StrSQL);
	if (vRet.empty()) {
		MessageBox(m_hWnd, _T("�û�������"), _T("����"), IDOK);
		return;
	}

	string password = UnicodeToAsc(StrUserPassword);
	if (password != vRet[0][4]) {
		MessageBox(m_hWnd, _T("�û����������"), _T("����"), IDOK);
		return;
	}


	//���ƥ��ɹ�
	//���ص�¼����
	ShowWindow(false);
	if (vRet[0][5] == "����Ա") {
		//����������
		MainWnd mainWnd;
		mainWnd.Create(NULL, _T("MainWnd"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
		mainWnd.p_mysql = p_mysql;//��MySQL�ĸ�ֵ
		mainWnd.CenterWindow();
		mainWnd.ShowModal();
	}
	else {
		//��������Ա�ܴ���
		
		CashierWnd CashierWnd;
		CashierWnd.Create(NULL, _T("CashierWnd"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
		CashierWnd.p_mysql = p_mysql;
		CashierWnd.CenterWindow();
		CashierWnd.ShowModal();
	}
	//tablayoue  
}