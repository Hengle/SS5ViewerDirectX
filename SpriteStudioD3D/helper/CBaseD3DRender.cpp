/*
 * @�T�v		�����_�����O�N���X
 *
 * $Id: 79dad66ddacd304b9c13286923e243a47d44eb59 $
 *
 * @author		��
 * @since		2002/05/05
 */

#include "Stdafx.h"
#include <D3D9.h>
#include <DDraw.h>
#include "CBaseD3DRender.h"

using namespace std;

// �����_�����O���[�h�̎w��
void CBaseD3DRender::SetRendering( void )
{
	// �F�̎Z�o���@�̐ݒ�
	switch( m_RenderType ){
	case D3DRENDER_MUL:
		// ��Z
		m_pD3DHelper->SetTexColorOp( D3DTOP_MODULATE );
		m_pD3DHelper->SetTexColorArg1( D3DTA_TEXTURE );
		m_pD3DHelper->SetTexColorArg2( D3DTA_DIFFUSE );
		// ���̎Z�o���@�̐ݒ�
		m_pD3DHelper->SetTexAlphaOp( D3DTOP_MODULATE );
		m_pD3DHelper->SetTexAlphaArg1( D3DTA_TEXTURE );
		m_pD3DHelper->SetTexAlphaArg2( D3DTA_DIFFUSE );
		break;
	case D3DRENDER_ADD:
		// ���Z
		m_pD3DHelper->SetTexColorOp( D3DTOP_ADD );
		m_pD3DHelper->SetTexColorArg1( D3DTA_TEXTURE );
		m_pD3DHelper->SetTexColorArg2( D3DTA_DIFFUSE );
		// ���̎Z�o���@�̐ݒ�
		m_pD3DHelper->SetTexAlphaOp( D3DTOP_ADD );
		m_pD3DHelper->SetTexAlphaArg1( D3DTA_TEXTURE );
		m_pD3DHelper->SetTexAlphaArg2( D3DTA_DIFFUSE );
		break;
	case D3DRENDER_DIF:
		// �f�B�t���[�Y�F�g�p
		m_pD3DHelper->SetTexColorOp( D3DTOP_SELECTARG1 );
		m_pD3DHelper->SetTexColorArg1( D3DTA_DIFFUSE );
		m_pD3DHelper->SetTexColorArg2( D3DTA_DIFFUSE );
		// ���̎Z�o���@�̐ݒ�
		m_pD3DHelper->SetTexAlphaOp( D3DTOP_SELECTARG1 );
		m_pD3DHelper->SetTexAlphaArg1( D3DTA_DIFFUSE );
		m_pD3DHelper->SetTexAlphaArg2( D3DTA_DIFFUSE );
		break;
	}

	// ���u�����h�̕��@�ݒ�
	if( m_bAlphaBlend ){
		// ������̐F�̎Z�o���@�̐ݒ�
		switch( m_BlendType ){
		case D3DBLEND_NML:
			// �ʏ�u�����h
			m_pD3DHelper->SetBlendOp( D3DBLENDOP_ADD );
			m_pD3DHelper->SetSrcBlend( D3DBLEND_SRCALPHA );
			m_pD3DHelper->SetDstBlend( D3DBLEND_INVSRCALPHA );
			// ������̃��l�̎Z�o���@�̐ݒ�
			m_pD3DHelper->SetBlendOpAlpha( D3DBLENDOP_MAX );
			m_pD3DHelper->SetSrcBlendAlpha( D3DBLEND_ONE );
			m_pD3DHelper->SetDstBlendAlpha( D3DBLEND_ONE );
			break;
		case D3DBLEND_ADD:
			// ���Z
			m_pD3DHelper->SetBlendOp( D3DBLENDOP_ADD );
			m_pD3DHelper->SetSrcBlend( D3DBLEND_SRCALPHA );
			m_pD3DHelper->SetDstBlend( D3DBLEND_ONE );
			// ������̃��l�̎Z�o���@�̐ݒ�
			m_pD3DHelper->SetBlendOpAlpha( D3DBLENDOP_ADD );
			m_pD3DHelper->SetSrcBlendAlpha( D3DBLEND_ONE );
			m_pD3DHelper->SetDstBlendAlpha( D3DBLEND_ONE );
			break;
		case D3DBLEND_SUB:
			// ���Z
			m_pD3DHelper->SetBlendOp( D3DBLENDOP_REVSUBTRACT );
			m_pD3DHelper->SetSrcBlend( D3DBLEND_SRCALPHA );
			m_pD3DHelper->SetDstBlend( D3DBLEND_ONE );
			// ������̃��l�̎Z�o���@�̐ݒ�
			m_pD3DHelper->SetBlendOpAlpha( D3DBLENDOP_REVSUBTRACT );
			m_pD3DHelper->SetSrcBlendAlpha( D3DBLEND_ONE );
			m_pD3DHelper->SetDstBlendAlpha( D3DBLEND_ONE );
			break;
		case D3DBLEND_MUL:
			// ��Z
			m_pD3DHelper->SetBlendOp( D3DBLENDOP_ADD );
			m_pD3DHelper->SetSrcBlend( D3DBLEND_ZERO );
			m_pD3DHelper->SetDstBlend( D3DBLEND_SRCCOLOR );
			// ������̃��l�̎Z�o���@�̐ݒ�
			m_pD3DHelper->SetBlendOpAlpha( D3DBLENDOP_ADD );
			m_pD3DHelper->SetSrcBlendAlpha( D3DBLEND_ZERO );
			m_pD3DHelper->SetDstBlendAlpha( D3DBLEND_SRCALPHA );
			break;
		case D3DBLEND_OVR:
			// �㏑���]��
			m_pD3DHelper->SetBlendOp( D3DBLENDOP_ADD );
			m_pD3DHelper->SetSrcBlend( D3DBLEND_ONE );
			m_pD3DHelper->SetDstBlend( D3DBLEND_ZERO );
			// ������̃��l�̎Z�o���@�̐ݒ�
			m_pD3DHelper->SetBlendOpAlpha( D3DBLENDOP_ADD );
			m_pD3DHelper->SetSrcBlendAlpha( D3DBLEND_ONE );
			m_pD3DHelper->SetDstBlendAlpha( D3DBLEND_ZERO );
			break;
		case D3DBLEND_CHI:
			// �F�̂݉��Z
			m_pD3DHelper->SetBlendOp( D3DBLENDOP_ADD );
			m_pD3DHelper->SetSrcBlend( D3DBLEND_ONE );
			m_pD3DHelper->SetDstBlend( D3DBLEND_ONE );
			// ������̃��l�̎Z�o���@�̐ݒ�
			m_pD3DHelper->SetBlendOpAlpha( D3DBLENDOP_ADD );
			m_pD3DHelper->SetSrcBlendAlpha( D3DBLEND_ZERO );
			m_pD3DHelper->SetDstBlendAlpha( D3DBLEND_ONE );
			break;
		case D3DBLEND_CHD:
			// �F�̂݌��Z
			m_pD3DHelper->SetBlendOp( D3DBLENDOP_REVSUBTRACT );
			m_pD3DHelper->SetSrcBlend( D3DBLEND_ONE );
			m_pD3DHelper->SetDstBlend( D3DBLEND_ONE );
			// ������̃��l�̎Z�o���@�̐ݒ�
			m_pD3DHelper->SetBlendOpAlpha( D3DBLENDOP_ADD );
			m_pD3DHelper->SetSrcBlendAlpha( D3DBLEND_ZERO );
			m_pD3DHelper->SetDstBlendAlpha( D3DBLEND_ONE );
			break;
		case D3DBLEND_CHM:
			// �F�̂ݏ�Z
			m_pD3DHelper->SetBlendOp( D3DBLENDOP_ADD );
			m_pD3DHelper->SetSrcBlend( D3DBLEND_ZERO );
			m_pD3DHelper->SetDstBlend( D3DBLEND_SRCCOLOR );
			// ������̃��l�̎Z�o���@�̐ݒ�
			m_pD3DHelper->SetBlendOpAlpha( D3DBLENDOP_ADD );
			m_pD3DHelper->SetSrcBlendAlpha( D3DBLEND_ZERO );
			m_pD3DHelper->SetDstBlendAlpha( D3DBLEND_ONE );
			break;
		case D3DBLEND_ACI:
			// ���̂݉��Z
			m_pD3DHelper->SetBlendOp( D3DBLENDOP_ADD );
			m_pD3DHelper->SetSrcBlend( D3DBLEND_ZERO );
			m_pD3DHelper->SetDstBlend( D3DBLEND_ONE );
			// ������̃��l�̎Z�o���@�̐ݒ�
			m_pD3DHelper->SetBlendOpAlpha( D3DBLENDOP_ADD );
			m_pD3DHelper->SetSrcBlendAlpha( D3DBLEND_ONE );
			m_pD3DHelper->SetDstBlendAlpha( D3DBLEND_ONE );
			break;
		case D3DBLEND_ACD:
			// ���̂݌��Z
			m_pD3DHelper->SetBlendOp( D3DBLENDOP_ADD );
			m_pD3DHelper->SetSrcBlend( D3DBLEND_ZERO );
			m_pD3DHelper->SetDstBlend( D3DBLEND_ONE );
			// ������̃��l�̎Z�o���@�̐ݒ�
			m_pD3DHelper->SetBlendOpAlpha( D3DBLENDOP_REVSUBTRACT );
			m_pD3DHelper->SetSrcBlendAlpha( D3DBLEND_ONE );
			m_pD3DHelper->SetDstBlendAlpha( D3DBLEND_ONE );
		case D3DBLEND_ACM:
			// ���̂ݏ�Z
			m_pD3DHelper->SetBlendOp( D3DBLENDOP_ADD );
			m_pD3DHelper->SetSrcBlend( D3DBLEND_ZERO );
			m_pD3DHelper->SetDstBlend( D3DBLEND_ONE );
			// ������̃��l�̎Z�o���@�̐ݒ�
			m_pD3DHelper->SetBlendOpAlpha( D3DBLENDOP_ADD );
			m_pD3DHelper->SetSrcBlendAlpha( D3DBLEND_ZERO );
			m_pD3DHelper->SetDstBlendAlpha( D3DBLEND_SRCALPHA );
		}
		// ���u�����h��L���ɂ���
		m_pD3DHelper->SetAlphaBlendEnable( TRUE );
		m_pD3DHelper->SetSeparateAlphaBlendEnable( TRUE );
	}else{
		// ���u�����h�͖���
		m_pD3DHelper->SetAlphaBlendEnable( FALSE );
		m_pD3DHelper->SetSeparateAlphaBlendEnable( FALSE );
	}
	if( m_bAlphaTest ){
		// ���e�X�g��L���ɂ���
		m_pD3DHelper->SetAlphaTestEnable( TRUE );
		m_pD3DHelper->SetAlphaRef( 0x01 );
		m_pD3DHelper->SetAlphaFunc( D3DCMP_GREATEREQUAL );
	}else{
		// ���e�X�g�͖���
		m_pD3DHelper->SetAlphaTestEnable( FALSE );
	}
	if( m_bLinearFilter ){
		// ���j�A�t�B���^�[��L���ɂ���
		m_pD3DHelper->SetScaleState( D3DTEXF_LINEAR );
	}else{
		// ���j�A�t�B���^�[�͖���
		m_pD3DHelper->SetScaleState( D3DTEXF_POINT );
	}
}

// �e�N�X�`����ύX
bool CBaseD3DRender::SetTexture( CD3DTexture* pTexture )
{
	if( pTexture != NULL && pTexture->IsInited() ){
		// �A�X�y�N�g����擾
		m_fAspectX = 1 / static_cast< float >( pTexture->GetTWidth() );
		m_fAspectY = 1 / static_cast< float >( pTexture->GetTHeight() );
		// �e�N�X�`�����蓖��
		m_pTexture = pTexture;
		return true;
	}
	return false;
}
