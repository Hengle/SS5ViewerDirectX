/*
 * @�T�v		�E�C���h�E�x�[�X�N���X
 *
 * $Id: CGWindowBase.h 1090 2008-01-08 03:56:49Z ryoh-srs $
 *
 * @author		��
 * @since		2003/08/23
 */

#pragma once
#undef CreateWindow
#undef DestroyWindow
#undef GetWindow

// ======================================================================
//  �N���X�錾
// ======================================================================
class DLL_EXPORT CGWindowBase:public CCS::ICCSInterface{
public:
	// �R���X�g���N�^�E�f�X�g���N�^
	CGWindowBase::CGWindowBase( void ):
		m_hWnd( NULL ),
		m_hMenu( NULL ),
		m_bIdle( true ){}
	virtual ~CGWindowBase( void ){}

public:
	// �E�C���h�E�n���h�����擾
	virtual HWND GetWindow( void )const
	{
		return m_hWnd;
	}

	virtual HMENU GetMenu( void )const
	{
		return m_hMenu;
	}

	virtual bool	CreateWindow( const std::string& sClassName, const std::string& sWindowTitle, DWORD dwStyle, int x, int y, int w, int h, HICON hIcon, HCURSOR hCursor, HBRUSH hBrush, HMENU hMenu, int nShow );
	virtual void	Run( void );
	virtual void	DestroyWindow( void );

protected:
	// ����������
	virtual bool Init( void ) = 0;
	// �A�C�h�����ɍs������
	virtual bool Idle( void ) = 0;
	// �I������
	virtual void Term( void ) = 0;
	// ���b�Z�[�W����
	virtual LRESULT	WinProc( HWND hWnd, UINT msg, WPARAM WParam, LPARAM LParam );

private:
	static LRESULT CALLBACK WinProc_Inner( HWND hWnd, UINT msg, WPARAM WParam, LPARAM LParam );

protected:
	HWND	m_hWnd;
	HMENU	m_hMenu;
	bool	m_bIdle;
};
