
// CalculatorDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "Calculator.h"


// CCalculatorDlg dialog
class CCalculatorDlg : public CDialogEx
{
// Construction
public:
	CCalculatorDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_CALCULATOR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()


	
public:
	afx_msg void OnBnClickedButton0();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton8();
	afx_msg void OnBnClickedButton9();
	afx_msg void OnBnClickedButtonPlus();
	afx_msg void OnBnClickedButtonMinus();
	afx_msg void OnBnClickedButtonMultiply();
	afx_msg void OnBnClickedButtonDivide();
	afx_msg void OnBnClickedButtonClear();
	afx_msg void OnBnClickedButtonEqual();
private:
	bool m_errorInput;
	const CString m_outputResetString;
	void resetOutput();
	void reset();
	void addDigit(char digit);
	void doOperation(Calculator::ActionType operation, bool handleNumber=true);
	void createHistoryText();

	Calculator m_calculator;	
	BOOL m_firstDigitEntered;
	CString m_historyText;
	CString m_output;
	CButton m_button0;
	CButton m_button1;
	CButton m_button2;
	CButton m_button3;
	CButton m_button4;
	CButton m_button5;
	CButton m_button6;
	CButton m_button7;
	CButton m_button8;
	CButton m_button9;
	CEdit m_editHistory;
	CEdit m_editResult;
	CButton m_buttonPlus;
	CButton m_buttonEquals;
	CButton m_buttonC;
	CButton m_buttonMinus;
	CButton m_buttonMultiply;
	CButton m_buttonDivide;
};
