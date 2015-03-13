/*
 * @�T�v		�����_�����O�N���X
 *
 * $Id: 79307f789932a541ca1b8fb7e06dec4dea49c23f $
 *
 * @author		��
 * @since		2002/05/05
 */

#pragma once

#include "CD3DTexture.h"
#include "CBaseD3DRender.h"
//#define _USE_DRAW_PRIMITIVE_

// ======================================================================
//  �N���X��`
// ======================================================================
class CD3DRender:public CCS::ICCSInterface
{
	enum{
		// �X�v���C�g�̍ő吔
		MAX_SPRITE = 16384,
	};

public:
	CD3DRender::CD3DRender( CDirect3DHelper* pD3DHelper ):
		ICCSInterface(),
		m_nRefCount( 1 ),
		m_BaseRender( pD3DHelper ),
		m_bSetTexture( false ),
		m_bSetParam( false ),
		m_bCreateVertex( false ),
		m_nNumPolygon( 0 ),
		m_nMaxPolygon( 0 )
	{
	}
	virtual ~CD3DRender( void )
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

private:
	// �J�X�^�����_�̒�`
	struct D3DVERTEX{
		// x, y, z, RHW, Color, Alpha Red Green Blue, fTU, fTV
		float		x, y, z;		// �|���S���̍��W
		float		fRHW;			// ���_�ϊ��l
		D3DCOLOR	Color;			// ���_�̐F
		float		fTU, fTV;		// �e�N�X�`�����W
	};

public:
	virtual CD3DTexture* STDCALLMETHOD GetTexture( void )
	{
		return m_BaseRender.GetTexture();
	}
	virtual UINT STDCALLMETHOD GetDrawNum( void )
	{
		return m_nMaxPolygon / 2;
	}
	virtual void STDCALLMETHOD GetRenderParameter( D3DRENDERTYPE* pRenderType, D3DBLENDTYPE* pBlendType = NULL, bool* pbAlphaBlend = NULL, bool* pbAlphaTest = NULL, bool* pbLinearFilter = NULL )const
	{
		m_BaseRender.GetRenderParameter( pRenderType, pBlendType, pbAlphaBlend, pbAlphaTest, pbLinearFilter );
	}
	// Draw�L���[�̒��O�����������
	virtual void STDCALLMETHOD DrawBack( void )
	{
		if( m_nNumPolygon > 0 ){
			m_nNumPolygon -= 2;
		}
	}
	// Draw�L���[������������
	virtual void STDCALLMETHOD DrawClear( void )
	{
		m_nNumPolygon = 0;
	}
	virtual bool STDCALLMETHOD SetTexture( CD3DTexture* pTexture )
	{
		m_bSetTexture = m_BaseRender.SetTexture( pTexture );
		return m_bSetTexture;
	}
	virtual void STDCALLMETHOD SetRenderParameter( D3DRENDERTYPE RenderType = D3DRENDER_MUL, D3DBLENDTYPE BlendType = D3DBLEND_NML,  bool bAlphaBlend = true, bool bAlphaTest = true, bool bLinearFilter = false )
	{
		m_BaseRender.SetRenderParameter( RenderType, BlendType, bAlphaBlend, bAlphaTest, bLinearFilter );
	}

	virtual bool STDCALLMETHOD	SetDrawNum( UINT nNum );
	virtual void STDCALLMETHOD	Draw( const D3DFOURPARAM* pFourParam, float fAngleX = 0, float fAngleY = 0, float fAngleZ = 0, UINT r = 6553 );
	virtual void STDCALLMETHOD	Rendering( void );

#ifdef _USE_DRAW_PRIMITIVE_
	virtual void Delete( void );
	virtual bool Recreate( void );

	virtual D3DOBJTYPE GetType( void )
	{
		return D3DOBJ_RENDER;
	}
#endif

private:
	CBaseD3DRender									m_BaseRender;
	UINT											m_nRefCount;
	bool											m_bSetTexture, m_bSetParam, m_bCreateVertex;
#ifdef _USE_DRAW_PRIMITIVE_
	boost::intrusive_ptr< IDirect3DVertexBuffer9 >	m_pVertexBuffer;
	boost::intrusive_ptr< IDirect3DIndexBuffer9 >	m_pIndexBuffer;
#else
	std::vector< D3DVERTEX >						m_aVertex;
	std::vector< WORD >								m_aIndex16;
	std::vector< DWORD >							m_aIndex32;
	void*											m_pIndex;
	D3DFORMAT										m_IndexFormat;
#endif
	D3DVERTEX*										m_pCurrentVertex;
	UINT											m_nNumPolygon, m_nMaxPolygon;
};
