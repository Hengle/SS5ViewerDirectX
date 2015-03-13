/*
 * @�T�v		�E�C���h�E�x�[�X�N���X
 *
 * $Id: CGWindowBase.cpp 1094 2008-01-09 07:19:24Z ryoh-srs $
 *
 * @author		��
 * @since		2003/08/23
 */

#include "Stdafx.h"
#include "ICCSInterface.h"
#include "CGWindowBase.h"
#include "CGlobalObject.h"

using namespace std;

// �E�C���h�E�𐶐�����
bool CGWindowBase::CreateWindow( const string& sClassName, const string& sWindowTitle, DWORD dwStyle, int x, int y, int w, int h, HICON hIcon, HCURSOR hCursor, HBRUSH hBrush, HMENU hMenu, int nShow )
{
	// ��d���s�h�~
	if( m_hWnd != NULL ){
		return false;
	}

	// �E�B���h�E�N���X�̓o�^
	WNDCLASSEX WindowClass = { 0 };
	WindowClass.cbSize			= sizeof( WNDCLASSEX );
	WindowClass.style			= CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	WindowClass.lpfnWndProc		= WinProc_Inner;
	WindowClass.hInstance		= CGlobalObject::GetThis()->GetInstance();
	WindowClass.hIcon			= hIcon;
	WindowClass.hCursor			= hCursor;
	WindowClass.hbrBackground	= hBrush;
	WindowClass.lpszClassName	= sClassName.c_str();
	if( !::RegisterClassEx( &WindowClass ) ){
		return false;
	}

	// �E�B���h�E����
	m_hWnd = ::CreateWindowEx(
		WS_EX_APPWINDOW,
		sClassName.c_str(),
		sWindowTitle.c_str(),
		dwStyle,
		x, y, w, h,
		NULL,
		NULL,
		CGlobalObject::GetThis()->GetInstance(),
		NULL );
	if( m_hWnd == NULL ){
		return false;
	}

	// ���j���[�̊��蓖��
	m_hMenu = hMenu;
	if( m_hMenu != NULL ){
		::SetMenu( m_hWnd, m_hMenu );
	}

	// �E�C���h�E�I�u�W�F�N�g�̓o�^
	::SetProp( m_hWnd, "WindowObject", this );

	// �E�C���h�E��\������
	::ShowWindow( m_hWnd, nShow );
	::UpdateWindow( m_hWnd );

	// ������WM_CREATE�𔭍s����
	::SendMessage( m_hWnd, WM_CREATE, 0, 0 );

	return true;
}

// �E�C���h�E�̃��C�����[�v
void CGWindowBase::Run( void )
{
	// ����������
	if( !Init() ){ return; }
	// ���C�����[�v
	for( ; ; ){
		MSG	msg;
		if( ::PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) ){
			if( msg.message == WM_QUIT ){ break; }
			// ���b�Z�[�W������
			m_bIdle = true;
			::TranslateMessage( &msg );
			::DispatchMessage( &msg );
		}else if( m_bIdle ){
			// �A�C�h�����̏���
			if( !Idle() ){
				m_bIdle = false;
			}
		}else{
			// ���b�Z�[�W��҂�
			::WaitMessage();
		}
	}
	// �I������
	Term();
}

// �E�C���h�E��j������
void CGWindowBase::DestroyWindow( void )
{
	if( m_hWnd != NULL ){
		// �E�C���h�E�ݒ�������l�ɖ߂�
		::ShowWindow( m_hWnd, SW_HIDE );
		::SetWindowLong( m_hWnd, GWL_STYLE, WS_POPUP );
		::SetWindowPos( m_hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );
		// �E�C���h�E�I�u�W�F�N�g��j��
		::RemoveProp( m_hWnd, "WindowObject" );
		// ���j���[�̔j��
		if( m_hMenu != NULL ){
			::SetMenu( m_hWnd, NULL );
			::DestroyMenu( m_hMenu );
			m_hMenu = NULL;
		}
		// �E�C���h�E��j��
		::DestroyWindow( m_hWnd );
		m_hWnd = NULL;
	}
}

LRESULT CGWindowBase::WinProc( HWND hWnd, UINT msg, WPARAM WParam, LPARAM LParam )
{
	if( msg == WM_CLOSE ){
		// �I�����b�Z�[�W�𑗐M
		::PostQuitMessage( 0 );
		return 0;
	}
	return ::DefWindowProc( hWnd, msg, WParam, LParam );
}

// �R�[���o�b�N����
LRESULT CALLBACK CGWindowBase::WinProc_Inner( HWND hWnd, UINT msg, WPARAM WParam, LPARAM LParam )
{
	// �E�C���h�E�I�u�W�F�N�g�̎擾
	CGWindowBase* pMWindow = static_cast< CGWindowBase* >( ::GetProp( hWnd, "WindowObject" ) );
	// �擾�ł���΃I�u�W�F�N�g���s�A�o���Ȃ���΃f�t�H���g�̏���
	if( pMWindow != NULL ){
		return pMWindow->WinProc( hWnd, msg, WParam, LParam );
	}
	return ::DefWindowProc( hWnd, msg, WParam, LParam );
}
