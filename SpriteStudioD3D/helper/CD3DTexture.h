/*
 * @�T�v		�e�N�X�`���N���X
 *
 * $Id: c43225ac2d71f606cc95fba991b281cdcaed30ca $
 *
 * @author		��
 * @since		2003/10/25
 */

#pragma once

#include "Direct3D.h"

// ======================================================================
//  �N���X��`
// ======================================================================
class CD3DTexture : public CCS::ICCSInterface
{
public:
	// �R���X�g���N�^�E�f�X�g���N�^
	CD3DTexture( CDirect3DHelper* pD3DHelper ):
		ICCSInterface(),
		m_nRefCount( 1 ),
		m_pD3DHelper( pD3DHelper ),
		m_bSetParam( false ),
		m_bCreateTex( false ),
		m_bLocked( false ),
		m_bRenderTarget( false ),
		m_bCompress( false ),
		m_bKeepSrcTexture( false ),
		m_nWidth( 0 ),
		m_nHeight( 0 ),
		m_nTWidth( 0 ),
		m_nTHeight( 0 )
	{
	}

	virtual ~CD3DTexture( void )
	{
	}

public:
	virtual ULONG STDCALLMETHOD AddRef( void )
	{
		return ++m_nRefCount;
	}
	virtual ULONG STDCALLMETHOD Release( void )
	{
		if( --m_nRefCount == 0 ){
			delete this;
			return 0;
		}
		return m_nRefCount;
	}
	virtual ULONG STDCALLMETHOD GetRefCount( void )
	{
		return m_nRefCount;
	}

public:
	// �摜�̕��擾
	virtual UINT STDCALLMETHOD GetWidth( void )const
	{
		return m_nWidth;
	}

	// �摜�̍����擾
	virtual UINT STDCALLMETHOD GetHeight( void )const
	{
		return m_nHeight;
	}

	// �e�N�X�`���̕��擾
	virtual UINT STDCALLMETHOD GetTWidth( void )const 
	{
		return m_nTWidth;
	}

	// �e�N�X�`���̍����擾
	virtual UINT STDCALLMETHOD GetTHeight( void )const
	{
		return m_nTHeight;
	}

	// �����_�����O�^�[�Q�b�g���ǂ���
	virtual bool STDCALLMETHOD IsRenderTarget( void )const
	{
		return m_bRenderTarget;
	}

	// ���k�e�N�X�`�����ǂ���
	virtual bool STDCALLMETHOD IsCompressTexture( void )const
	{
		return m_bCompress;
	}

	virtual bool STDCALLMETHOD	CreateTextureFromFile( const char* pszFileName );
	virtual bool STDCALLMETHOD	CreateTexture( UINT nWidth, UINT nHeight, bool bRenderTarget = false, bool bCompress = false );
	virtual void STDCALLMETHOD	DestroyTexture( void );
	virtual bool STDCALLMETHOD	LockTexture( D3DLOCKED_RECT* pLockRect, bool bKeepSrcTexture = false );
	virtual void STDCALLMETHOD	UnlockTexture( void );

public:
	// ����ɏ��������ꂽ���擾
	bool IsInited( void )const
	{
		return m_bSetParam;
	}

	// �\���e�N�X�`�����g�p�ł����Ԃ�
	bool IsVRAMInited( void )const
	{
		return m_bCreateTex;
	}

	// �X�e���V���T�[�t�F�X�擾
	IDirect3DSurface9* GetDepthSurface( void )
	{
		return m_pDepthSurface;
	}

	// �e�N�X�`���擾
	IDirect3DTexture9* GetTexture( void )
	{
		return m_pDestTexture;
	}

	virtual void Delete( void );
	virtual bool Recreate( void );

	virtual D3DOBJTYPE GetType( void )
	{
		return D3DOBJ_TEXTURE;
	}

private:
	UINT										m_nRefCount;
	bool										m_bSetParam, m_bCreateTex, m_bLocked;
	bool										m_bRenderTarget, m_bCompress;
	bool										m_bKeepSrcTexture;
	const CComPtr< CDirect3DHelper >			m_pD3DHelper;
	CComPtr< IDirect3DTexture9 >				m_pDestTexture, m_pSrcTexture;
	CComPtr< IDirect3DSurface9 >				m_pDepthSurface;
	UINT										m_nWidth, m_nHeight;
	UINT										m_nTWidth, m_nTHeight;
};
