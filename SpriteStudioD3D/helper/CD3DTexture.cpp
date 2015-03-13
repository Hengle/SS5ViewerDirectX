/*
 * @�T�v		�e�N�X�`���N���X
 *
 * $Id: 22c2f276b6880e5e9cc437d6457439b6bcf3bab3 $
 *
 * @author		��
 * @since		2003/10/25
 */

#include "Stdafx.h"
#include <D3D9.h>
#include <D3DX9.h>
#include <DDraw.h>
#include "CD3DTexture.h"
#include "Direct3D.h"

using namespace std;

bool STDCALLMETHOD CD3DTexture::CreateTextureFromFile( const char* pszFileName )
{
	// �����A�����b�N^^;
	UnlockTexture();
	if( !m_bCreateTex && !m_bLocked ){
		IDirect3DDevice9* pD3DDevice;
		pD3DDevice = static_cast< IDirect3DDevice9* >( m_pD3DHelper->GetDevice() );
		if( pD3DDevice == NULL ){ return false; }
		IDirect3DTexture9* p;
		if( FAILED( ::D3DXCreateTextureFromFile( pD3DDevice, pszFileName, &p ) ) ){
			return false;
		}
		m_pDestTexture.Attach( p );
		m_pSrcTexture = NULL;
		// ����������
		m_bSetParam = true;
		m_bCreateTex = true;
		D3DSURFACE_DESC Desc;
		m_pDestTexture->GetLevelDesc( 0, &Desc );
		m_nWidth		= Desc.Width;
		m_nHeight		= Desc.Height;
		m_nTWidth		= Desc.Width;
		m_nTHeight		= Desc.Height;
		m_bRenderTarget	= false;
		m_bCompress		= Desc.Format == D3DFMT_DXT5;
	}
	return true;
}

// �e�N�X�`�����쐬
bool STDCALLMETHOD CD3DTexture::CreateTexture( UINT nWidth, UINT nHeight, bool bRenderTarget, bool bCompress )
{
	// �����A�����b�N^^;
	UnlockTexture();
	// ��Ԃ��m�F���āA������������Ȃɂ����Ȃ�
	if( nWidth != 0 && nHeight != 0 ){		
		IDirect3DDevice9* pD3DDevice;
		pD3DDevice = static_cast< IDirect3DDevice9* >( m_pD3DHelper->GetDevice() );
		if( pD3DDevice == NULL ){ return false; }
		// �e��p�����[�^
		m_nWidth		= nWidth;
		m_nHeight		= nHeight;
		m_bRenderTarget	= bRenderTarget;
		m_bCompress		= bCompress;
		// �e�N�X�`���̃t�H�[�}�b�g�ݒ�
		D3DFORMAT ColorDepth;
		DWORD dwUsage;
		if( bRenderTarget ){
			ColorDepth = D3DFMT_A8R8G8B8;
			dwUsage = D3DUSAGE_RENDERTARGET;
			m_bCompress = false;
		}else if( !bCompress ){
			ColorDepth = D3DFMT_A8R8G8B8;
			dwUsage = 0;
		}else{
			ColorDepth = D3DFMT_DXT5;
			dwUsage = 0;
		}
		const D3DCONSTPARAMETER& ConstParameter = *m_pD3DHelper->GetConstParameter();
		if( ConstParameter.bTextureSizePow2 ){
			// �e�N�X�`���T�C�Y��2�̓��{�ł����쐬�ł��Ȃ��O���t�B�b�N�J�[�h�ł́A
			// ���{�ň�ԋ߂��T�C�Y�ō쐬���A������̗p����悤�ɂ���
			// ��DXT5��4�̔{���̃e�N�X�`���ɂȂ�̂ŊJ�n�l��4�ɂ��Ă���
			for( m_nTWidth = 4 ; m_nTWidth < nWidth ; m_nTWidth <<= 1 ){}
			for( m_nTHeight = 4 ; m_nTHeight < nHeight ; m_nTHeight <<= 1 ){}
		}else{
			// ����ȊO�̃e�N�X�`����DXT5�ׂ̈�4�̔{���ō쐬���s���悤�ɂ���
			for( m_nTWidth = 4 ; m_nTWidth < nWidth ; m_nTWidth += 4 ){}
			for( m_nTHeight = 4 ; m_nTHeight < nHeight ; m_nTHeight += 4 ){}
		}
		// ������̃T�C�Y���쐬�ł���ő�e�N�X�`���T�C�Y�𒴂��Ă����ꍇ
		// �쐬�ł��Ȃ��̂ŃG���[�I��������
		if( ConstParameter.nMaxTextureWidth < m_nTWidth || ConstParameter.nMaxTextureHeight < m_nTHeight ){
			return false;
		}
		// �e�N�X�`���̐���
		{
			IDirect3DTexture9* p;
			if( FAILED( pD3DDevice->CreateTexture( m_nTWidth, m_nTHeight, 1, dwUsage, ColorDepth, D3DPOOL_DEFAULT, &p, NULL ) ) ){
				return false;
			}
			m_pDestTexture.Attach( p );
		}
		// �����_�����O�^�[�Q�b�g�̏ꍇ
		// �X�e���V���T�[�t�F�X���쐬����
		if( m_bRenderTarget ){
			IDirect3DSurface9* p;
			if( FAILED( pD3DDevice->CreateDepthStencilSurface(
				m_nTWidth,
				m_nTHeight,
				D3DFMT_D16,
				D3DMULTISAMPLE_NONE,
				0,
				true,
				&p,
				NULL
				) ) ){
				return false;
			}
			m_pDepthSurface.Attach( p );
		}else{
			m_pDepthSurface = NULL;
		}
		// �Â��V�X�e���e�N�X�`���̓e�N�X�`���̏�񂪕ς�����ۂ�
		// �ė��p�ł��Ȃ��̂ō쐬���Ȃ���
		m_pSrcTexture = NULL;
		// ����������
		m_bSetParam = true;
		m_bCreateTex = true;
		// �S�~���\�������̂�h�~���邽��
		// �e�N�X�`���������h��Ԃ�^^;
		{
			D3DLOCKED_RECT LockRect;
			if( LockTexture( &LockRect, false ) ){
				if( !m_bCompress ){
					::ZeroMemory( LockRect.pBits, LockRect.Pitch * m_nTHeight );
				}else{
					::ZeroMemory( LockRect.pBits, LockRect.Pitch * ( m_nTHeight / 4 ) );
				}
				UnlockTexture();
			}
		}
	}
	return true;
}

// �e�N�X�`����j������
void STDCALLMETHOD CD3DTexture::DestroyTexture( void )
{
	// �����A�����b�N^^;
	UnlockTexture();
	if( m_bSetParam ){
		m_bSetParam			= false;
		m_bCreateTex		= false;
		m_pDepthSurface		= NULL;
		m_pDestTexture		= NULL;
		m_pSrcTexture		= NULL;
		m_bRenderTarget		= false;
		m_bCompress			= false;
		m_bKeepSrcTexture	= false;
		m_nWidth			= 0;
		m_nHeight			= 0;
		m_nTWidth			= 0;
		m_nTHeight			= 0;
	}
}

// �e�N�X�`�������b�N���āA�|�C���^�𓾂�
bool STDCALLMETHOD CD3DTexture::LockTexture( D3DLOCKED_RECT* pLockRect, bool bKeepSrcTexture )
{
	if( m_bCreateTex && !m_bLocked ){
		// VideoRAM�ɑ��݂���e�N�X�`���͒��ڃ��b�N���邱�Ƃ��ł��Ȃ��̂�
		// �V�X�e���������Ƀe�N�X�`�����쐬���A��������b�N���ēn��
		//
		// m_bKeepSrcTexture�t���O�ɂ���đO��쐬���ꂽ�V�X�e���e�N�X�`�����ێ�����Ă���ꍇ��
		// ���̏������΂��č�������}��
		if( m_pSrcTexture == NULL ){
			IDirect3DDevice9* pD3DDevice;
			pD3DDevice = static_cast< IDirect3DDevice9* >( m_pD3DHelper->GetDevice() );
			if( pD3DDevice == NULL ){ return false; }
			// �e�N�X�`���Ŏg�p����t�H�[�}�b�g�𔻕�
			D3DFORMAT ColorDepth;
			if( m_bRenderTarget || !m_bCompress ){
				ColorDepth = D3DFMT_A8R8G8B8;
			}else{
				ColorDepth = D3DFMT_DXT5;
			}
			// CreateOffscreenPlainSurface�ŃV�X�e���������T�[�t�F�X���쐬��
			// ������g�p���邱�ƂŃ��b�N���������邱�Ƃ��\����
			// �I�t�X�N���[���v���[���T�[�t�F�X�ŁuD3DFMT_DXT5�v�t�H�[�}�b�g���T�|�[�g���Ȃ��h���C�o�����݂��邽��
			// (��F�Â��I���{�[�h�O���t�B�b�N��)
			// �����ł̓V�X�e���������e�N�X�`�����쐬���A���b�N�������s��
			IDirect3DTexture9* p;
			if( FAILED( pD3DDevice->CreateTexture( m_nTWidth, m_nTHeight, 1, 0, ColorDepth, D3DPOOL_SYSTEMMEM, &p, NULL ) ) ){
				return false;
			}
			m_pSrcTexture.Attach( p );
		}
		// GetRenderTargetData���\�b�h��p����
		// VideoRAM�̓��e���V�X�e���e�N�X�`���[�ɃR�s�[����
		// ��GetRenderTargetData���\�b�h�̓o�b�N�o�b�t�@��A�uD3DUSAGE_RENDERTARGET�v�t���O���e�N�X�`���Ƃ�����
		// �@�����郌���_�����O�^�[�Q�b�g�����]�����Ƃ��Ď󂯕t���Ȃ��̂�
		// �@�e�N�X�`���쐬���͕K���AdwUsage�ɁuD3DUSAGE_RENDERTARGET�v�t���O���Z�b�g���邱��
		// �@(�܂背���_�[�^�[�Q�b�g�Ɏw��ł��Ȃ�D3DFMT_DXT5�ɂ̓R�s�[�ł��Ȃ�)
		// �@�܂��A�]����̓������Ǘ��I�v�V������D3DPOOL_SYSTEMMEM���w�肵�V�X�e���������Ɋm�ۂ����T�[�t�F�C�X��
		// �@�t�H�[�}�b�g�ƃT�C�Y����v���Ă���K�v������
		//
		// �������ݏ��������s��Ȃ��ꍇ��(�����_�����O�^�[�Q�b�g�ł͂Ȃ�)
		// ���̏������΂����Ƃō��������s��
		if( m_bRenderTarget ){
			IDirect3DDevice9* pD3DDevice;
			pD3DDevice = static_cast< IDirect3DDevice9* >( m_pD3DHelper->GetDevice() );
			if( pD3DDevice == NULL ){ return false; }
			IDirect3DSurface9* p;
			m_pDestTexture->GetSurfaceLevel( 0, &p );
			CComPtr< IDirect3DSurface9 > pDestSurface;
			pDestSurface.Attach( p );
			m_pSrcTexture->GetSurfaceLevel( 0, &p );
			CComPtr< IDirect3DSurface9 > pSourceSurface;
			pSourceSurface.Attach( p );
			if( FAILED( pD3DDevice->GetRenderTargetData( pDestSurface, pSourceSurface ) ) ){
				return false;
			}
		}
		if( FAILED( m_pSrcTexture->LockRect( 0, reinterpret_cast< ::D3DLOCKED_RECT* >( pLockRect ), NULL, 0 ) ) ){
			return false;
		}
		// �V�X�e���e�N�X�`���ێ��t���O���Z�b�g
		m_bKeepSrcTexture = bKeepSrcTexture;
		m_bLocked = true;
		return true;
	}
	return false;
}

// ���b�N���ꂽ�e�N�X�`������������
void STDCALLMETHOD CD3DTexture::UnlockTexture( void )
{
	if( m_bCreateTex && m_bLocked ){
		// �e�N�X�`�����A�����b�N
		// �A�����b�N��ɓ��e��]������
		// �������擾���[�h�̏ꍇ�͑ΏۊO
		m_pSrcTexture->UnlockRect( 0 );
		{
			IDirect3DDevice9* pD3DDevice;
			pD3DDevice = static_cast< IDirect3DDevice9* >( m_pD3DHelper->GetDevice() );
			if( pD3DDevice == NULL ){ return; }
			//
			IDirect3DSurface9* p;
			m_pDestTexture->GetSurfaceLevel( 0, &p );
			CComPtr< IDirect3DSurface9 > pDestSurface;
			pDestSurface.Attach( p );
			m_pSrcTexture->GetSurfaceLevel( 0, &p );
			CComPtr< IDirect3DSurface9 > pSrcSurface;
			pSrcSurface.Attach( p );
			pD3DDevice->UpdateSurface( pSrcSurface, NULL, pDestSurface, NULL );
		}
		// �T�[�t�F�X���c���Ȃ��I�v�V�����̏ꍇ��
		// �s�v�ɂȂ����T�[�t�F�X�����
		if( !m_bKeepSrcTexture ){
			m_pSrcTexture = NULL;
		}
		m_bLocked = false;
	}
}

// �e�N�X�`�����ꎞ�I�ɍ폜
void CD3DTexture::Delete( void )
{
	if( m_bSetParam ){
		m_bCreateTex = false;
		m_pDepthSurface = NULL;
		m_pDestTexture = NULL;
	}
}

// �e�N�X�`������蒼��
bool CD3DTexture::Recreate( void )
{
	if( m_bSetParam ){
		IDirect3DDevice9* pD3DDevice;
		pD3DDevice = static_cast< IDirect3DDevice9* >( m_pD3DHelper->GetDevice() );
		if( pD3DDevice == NULL ){ return false; }
		// �e�N�X�`���̃t�H�[�}�b�g�ݒ�
		D3DFORMAT ColorDepth;
		DWORD dwUsage;
		if( m_bRenderTarget ){
			ColorDepth = D3DFMT_A8R8G8B8;
			dwUsage = D3DUSAGE_RENDERTARGET;
		}else if( !m_bCompress ){
			ColorDepth = D3DFMT_A8R8G8B8;
			dwUsage = 0;
		}else{
			ColorDepth = D3DFMT_DXT5;
			dwUsage = 0;
		}
		// �e�N�X�`���̍Đ���
		{
			IDirect3DTexture9* p;
			if( FAILED( pD3DDevice->CreateTexture( m_nTWidth, m_nTHeight, 1, dwUsage, ColorDepth, D3DPOOL_DEFAULT, &p, NULL ) ) ){
				return false;
			}
			m_pDestTexture.Attach( p );
		}
		// �����_�����O�^�[�Q�b�g�̏ꍇ
		// �X�e���V���T�[�t�F�X���쐬����
		if( m_bRenderTarget ){
			IDirect3DSurface9* p;
			if( FAILED( pD3DDevice->CreateDepthStencilSurface(
				m_nTWidth,
				m_nTHeight,
				D3DFMT_D16,
				D3DMULTISAMPLE_NONE,
				0,
				true,
				&p,
				NULL
				) ) ){
				return false;
			}
			m_pDepthSurface.Attach( p );
		}
		// �ď���������
		m_bCreateTex = true;
		// �S�~���\�������̂�h�~���邽��
		// �e�N�X�`���������h��Ԃ�^^;
		{
			D3DLOCKED_RECT LockRect;
			if( LockTexture( &LockRect, false ) ){
				if( !m_bCompress ){
					::ZeroMemory( LockRect.pBits, LockRect.Pitch * m_nTHeight );
				}else{
					::ZeroMemory( LockRect.pBits, LockRect.Pitch * ( m_nTHeight / 4 ) );
				}
				UnlockTexture();
			}
		}
	}
	return true;
}
