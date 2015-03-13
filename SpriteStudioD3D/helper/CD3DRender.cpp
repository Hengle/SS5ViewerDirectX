/*
 * @�T�v		�����_�����O�N���X
 *
 * $Id: 49e469bfd6499e2cd70e9601e6d8222ea46eb434 $
 *
 * @author		��
 * @since		2002/05/05
 */

#include "Stdafx.h"
#include <D3D9.h>
#include <DDraw.h>
#include <math.h>
#include "CD3DRender.h"

using namespace std;

// �`�搔�ύX
bool STDCALLMETHOD CD3DRender::SetDrawNum( UINT nNum )
{
	const D3DCONSTPARAMETER& ConstParameter = *m_BaseRender.GetD3DHelper()->GetConstParameter();
	UINT nMaxPrimitive = ConstParameter.nMaxPrimitiveCount / 2;
	UINT nMaxIndex = ConstParameter.nMaxVertexIndex / 6;
	UINT nMaxNum;
	if( nMaxPrimitive < nMaxIndex ){
		nMaxNum = nMaxPrimitive;
	}else{
		nMaxNum = nMaxIndex;
	}
	if( nNum != 0 && nNum <= nMaxNum ){
		// ���݋L�����ꂽ�`��|�C���g�����ׂď�����
		m_nNumPolygon = 0;
		// �ő�|���S�����ݒ�
		m_nMaxPolygon = nNum * 2;
#ifdef _USE_DRAW_PRIMITIVE_
		// Direct3D�f�o�C�X�̎擾
		IDirect3DDevice9* pD3DDevice;
		pD3DDevice = static_cast< IDirect3DDevice9* >( m_BaseRender.GetD3D()->CreateChildObject( IID_ID3DDevice ) );
		if( pD3DDevice == NULL ){ return false; }
		//���_�o�b�t�@�̍쐬
		if( m_pVertexBuffer != NULL ){
			// ���łɂ������ꍇ�͍폜
			m_pVertexBuffer->Unlock();
			m_pCurrentVertex = NULL;
			m_pVertexBuffer = NULL;
		}
		// ���_�o�b�t�@�̍쐬
		IDirect3DVertexBuffer9* p;
		if( FAILED( pD3DDevice->CreateVertexBuffer( m_nMaxPolygon * 2 * sizeof( D3DVERTEX ), D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, VERTEXFVF, D3DPOOL_DEFAULT, &p, NULL ) ) ){
			return false;
		}
		m_pVertexBuffer = intrusive_ptr< IDirect3DVertexBuffer9 >( p, false );
#else
		m_aVertex.resize( m_nMaxPolygon * 2 );
#endif
		// �C���f�b�N�X�o�b�t�@�̍쐬
		if( ConstParameter.nMaxVertexIndex > 65535 ){
#ifdef _USE_DRAW_PRIMITIVE_
			IDirect3DIndexBuffer9* p2;
			// 32Bit�C���f�b�N�X
			pD3DDevice->CreateIndexBuffer( m_nMaxPolygon * 3 * sizeof( DWORD ), D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, D3DFMT_INDEX32, D3DPOOL_DEFAULT, &p2, NULL );
			m_pIndexBuffer = intrusive_ptr< IDirect3DIndexBuffer9 >( p2, false );
			DWORD* pdwIndex;
			m_pIndexBuffer->Lock( 0, 0, reinterpret_cast< void** >( &pdwIndex ), 0 );
#else
			m_aIndex32.resize( m_nMaxPolygon * 3 );
			vector< DWORD >& pdwIndex = m_aIndex32;
			m_pIndex = &m_aIndex32[ 0 ];
			m_IndexFormat = D3DFMT_INDEX32;
#endif
			DWORD dwCnt = 0;
			DWORD dwBlockSize = m_nMaxPolygon * 3;
			for( UINT i = 0 ; i < dwBlockSize ; i += 6 ){
				pdwIndex[ i ] = dwCnt;
				pdwIndex[ i + 1 ] = dwCnt + 1;
				pdwIndex[ i + 2 ] = dwCnt + 3;

				pdwIndex[ i + 3 ] = dwCnt;
				pdwIndex[ i + 4 ] = dwCnt + 3;
				pdwIndex[ i + 5 ] = dwCnt + 2;

				dwCnt += 4;
			}
		}else{
			// 16Bit�C���f�b�N�X
#ifdef _USE_DRAW_PRIMITIVE_
			IDirect3DIndexBuffer9* p2;
			pD3DDevice->CreateIndexBuffer( m_nMaxPolygon * 3 * sizeof( WORD ), D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &p2, NULL );
			m_pIndexBuffer = intrusive_ptr< IDirect3DIndexBuffer9 >( p2, false );
			WORD* pwIndex;
			m_pIndexBuffer->Lock( 0, 0, reinterpret_cast< void** >( &pwIndex ), 0 );
#else
			m_aIndex16.resize( m_nMaxPolygon * 3 );
			vector< WORD >& pwIndex = m_aIndex16;
			m_pIndex = &pwIndex[ 0 ];
			m_IndexFormat = D3DFMT_INDEX16;
#endif
			WORD wCnt = 0;
			DWORD dwBlockSize = m_nMaxPolygon * 3;
			for( UINT i = 0 ; i < dwBlockSize ; i += 6 ){
				pwIndex[ i ] = wCnt;
				pwIndex[ i + 1 ] = wCnt + 1;
				pwIndex[ i + 2 ] = wCnt + 3;

				pwIndex[ i + 3 ] = wCnt;
				pwIndex[ i + 4 ] = wCnt + 3;
				pwIndex[ i + 5 ] = wCnt + 2;

				wCnt += 4;
			}
		}
#ifndef _USE_DRAW_PRIMITIVE_
		m_pCurrentVertex = &m_aVertex[ 0 ];
#endif
		m_bSetParam = true;
		m_bCreateVertex = true;
		return true;
	}
	return false;
}

// �`��
void STDCALLMETHOD CD3DRender::Draw( const D3DFOURPARAM* pFourParam, float fAngleX, float fAngleY, float fAngleZ, UINT /* r */ )
{
	if( m_bSetTexture && m_bSetParam && m_bCreateVertex
			&& m_nNumPolygon < m_nMaxPolygon ){
		// �|���S�����W���`��
		float fCX =
			( pFourParam->fDestX[ 0 ] + pFourParam->fDestX[ 1 ]
			+ pFourParam->fDestX[ 2 ] + pFourParam->fDestX[ 3 ] ) / 4.0f;
		float fCY =
			( pFourParam->fDestY[ 0 ] + pFourParam->fDestY[ 1 ]
			+ pFourParam->fDestY[ 2 ] + pFourParam->fDestY[ 3 ] ) / 4.0f;
		// RHW�����߂�
		float w = static_cast< float >( fabs( pFourParam->fDestX[ 0 ] - fCX ) );
		for( int i = 1 ; i < 4 ; i++ ){
			float fTW = static_cast< float >( fabs( pFourParam->fDestX[ i ] - fCX ) );
			if( w < fTW ){
				w = fTW;
			}
		}
		for( int i = 0 ; i < 4 ; i++ ){
			float fTW = static_cast< float >( fabs( pFourParam->fDestY[ i ] - fCY ) );
			if( w < fTW ){
				w = fTW;
			}
		}
		float fRHW = w * PI;

		// ��]�s������(^^
		D3DMATRIX RMatrix;
		m_BaseRender.GetD3DHelper()->MatrixRotation( &RMatrix, fAngleX, fAngleY, fAngleZ );

		// �x�N�^�[������
		D3DVECTOR SrcVector[ 4 ] = {
			{ pFourParam->fDestX[ 0 ] - fCX, pFourParam->fDestY[ 0 ] - fCY, 0.0f },
			{ pFourParam->fDestX[ 1 ] - fCX, pFourParam->fDestY[ 1 ] - fCY, 0.0f },
			{ pFourParam->fDestX[ 2 ] - fCX, pFourParam->fDestY[ 2 ] - fCY, 0.0f },
			{ pFourParam->fDestX[ 3 ] - fCX, pFourParam->fDestY[ 3 ] - fCY, 0.0f }
		};

#ifdef _USE_DRAW_PRIMITIVE_
		m_pIndexBuffer->Unlock();
		// ���_�o�b�t�@�����b�N
		m_pVertexBuffer->Lock( 0, 0, reinterpret_cast< void** >( &m_pCurrentVertex ), 0 );
#endif
		// �e���_����]
		D3DVECTOR DestVector;
		int nVertex = m_nNumPolygon * 2;
		for( int i = 0 ; i < 4 ; i++ ){
			// �e���_����]
			m_BaseRender.GetD3DHelper()->TransformNormal( &DestVector, &SrcVector[ i ], &RMatrix );
			DestVector.z += fRHW;
			int n = nVertex + i;
			// ��]�����f�[�^���i�[
			m_pCurrentVertex[ n ].x = DestVector.x * fRHW / DestVector.z + fCX - 0.5f;
			m_pCurrentVertex[ n ].y = DestVector.y * fRHW / DestVector.z + fCY - 0.5f;
			m_pCurrentVertex[ n ].z = pFourParam->fDestZ[ i ] * m_BaseRender.GetAspectZ();
//			m_pCurrentVertex[ n ].z = 0.4f + ( pFourParam->fDestZ[ i ] * m_BaseRender.GetAspectZ() + DestVector.z / fRHW * ( r * m_BaseRender.GetAspectZ() ) ) * 0.1f;
			m_pCurrentVertex[ n ].fRHW = pFourParam->fRHW[ i ] / ( DestVector.z / fRHW );
			m_pCurrentVertex[ n ].Color = pFourParam->Color[ i ];
			// �]������ݒ�
			m_pCurrentVertex[ n ].fTU = pFourParam->fSrcX[ i ] * m_BaseRender.GetAspectX();
			m_pCurrentVertex[ n ].fTV = pFourParam->fSrcY[ i ] * m_BaseRender.GetAspectY();
		}
		// �o�b�t�@�[�̃J�E���g��i�߂�
		m_nNumPolygon += 2;
#ifdef _USE_DRAW_PRIMITIVE_
		// ���_���A�����b�N
		m_pVertexBuffer->Unlock();
		m_pCurrentVertex = NULL;
#endif
	}
}

// �����_�����O
void STDCALLMETHOD CD3DRender::Rendering( void )
{
	// �����_���Ƀe�N�X�`�����Z�b�g����Ă��邩?
	if( m_bSetTexture ){
		// �����_���ɒ��_��񂪐ݒ�ς݁A�\���\�Ȃ�`��^^
		bool bTexInited = m_BaseRender.GetTexture()->IsVRAMInited();
		if( bTexInited && m_bSetParam && m_bCreateVertex && m_nNumPolygon != 0 ){
			// �u�����h���[�h�̐ݒ�
			m_BaseRender.SetRendering();
			// Direct3D�f�o�C�X�擾
			IDirect3DDevice9* pD3DDevice;
			pD3DDevice = m_BaseRender.GetD3DHelper()->GetDevice();
			if( pD3DDevice == NULL ){ return; }
			// �e�N�X�`�����Z�b�g
			pD3DDevice->SetTexture( 0, m_BaseRender.GetTexture()->GetTexture() );
#ifdef _USE_DRAW_PRIMITIVE_
			// ���_�o�b�t�@���Z�b�g
			pD3DDevice->SetStreamSource( 0, m_pVertexBuffer.get(), 0, sizeof( D3DVERTEX ) );
			// �C���f�b�N�X�o�b�t�@���Z�b�g
			pD3DDevice->SetIndices( m_pIndexBuffer.get() );
			// �`��
			pD3DDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, m_nNumPolygon * 2, 0, m_nNumPolygon );
#else
			pD3DDevice->DrawIndexedPrimitiveUP(
				D3DPT_TRIANGLELIST,
				0,
				m_nNumPolygon * 2,
				m_nNumPolygon,
				m_pIndex,
				m_IndexFormat,
				m_pCurrentVertex,
				sizeof( D3DVERTEX ) );
#endif
			m_nNumPolygon = 0;
		}
	}
}

#ifdef _USE_DRAW_PRIMITIVE_
// ���_�E�C���f�b�N�X�o�b�t�@����菜��
void CD3DRender::Delete( void )
{
	if( m_bSetParam ){
		m_bCreateVertex = false;
		// ���_���A�����b�N
		m_pVertexBuffer->Unlock();
		m_pCurrentVertex = NULL;
		// �o�b�t�@���폜����
		m_pVertexBuffer = NULL;
		m_pIndexBuffer = NULL;
	}
}

// ���_�E�C���f�b�N�X�o�b�t�@����蒼��
bool CD3DRender::Recreate( void )
{
	// ��Ԃ��m�F���āA������������I��(^^;
	if( m_bSetParam ){
		// Direct3D�f�o�C�X�̎擾
		IDirect3DDevice9* pD3DDevice;
		pD3DDevice = static_cast< IDirect3DDevice9* >( m_BaseRender.GetD3D()->CreateChildObject( IID_ID3DDevice ) );
		if( pD3DDevice == NULL ){ return false; }
		// ���_�o�b�t�@�̍쐬
		IDirect3DVertexBuffer9* p;
		//�e�N�X�`�����ꖇ�����Ȃ̂ŁA�ʏ�`��
		if( FAILED( pD3DDevice->CreateVertexBuffer( m_nMaxPolygon * 2 * sizeof( D3DVERTEX ), D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, VERTEXFVF, D3DPOOL_DEFAULT, &p, NULL ) ) ){
			return false;
		}
		m_pVertexBuffer = intrusive_ptr< IDirect3DVertexBuffer9 >( p, false );

		// �쐬����C���f�b�N�X�o�b�t�@�̏����擾
		const D3DCONSTPARAMETER& ConstParameter = m_BaseRender.GetD3DHelper()->GetConstParameter();
		
		// �C���f�b�N�X�o�b�t�@�̍쐬
		IDirect3DIndexBuffer9* p2;
		if( ConstParameter.nMaxVertexIndex > 65535 ){
			// 32Bit�C���f�b�N�X
			pD3DDevice->CreateIndexBuffer( m_nMaxPolygon * 3 * sizeof( DWORD ), D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, D3DFMT_INDEX32, D3DPOOL_DEFAULT, &p2, NULL );
			m_pIndexBuffer = intrusive_ptr< IDirect3DIndexBuffer9 >( p2, false );
			DWORD* pdwIndex;
			m_pIndexBuffer->Lock( 0, 0, reinterpret_cast< void** >( &pdwIndex ), D3DLOCK_NOSYSLOCK );
			DWORD dwCnt = 0;
			DWORD dwBlockSize = m_nMaxPolygon * 3;
			for( UINT i = 0 ; i < dwBlockSize ; i += 6 ){
				pdwIndex[ i ] = dwCnt;
				pdwIndex[ i + 1 ] = dwCnt + 1;
				pdwIndex[ i + 2 ] = dwCnt + 3;

				pdwIndex[ i + 3 ] = dwCnt;
				pdwIndex[ i + 4 ] = dwCnt + 3;
				pdwIndex[ i + 5 ] = dwCnt + 2;

				dwCnt += 4;
			}
		}else{
			// 16Bit�C���f�b�N�X
			pD3DDevice->CreateIndexBuffer( m_nMaxPolygon * 3 * sizeof( WORD ), D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &p2, NULL );
			m_pIndexBuffer = intrusive_ptr< IDirect3DIndexBuffer9 >( p2, false );
			WORD* pwIndex;
			m_pIndexBuffer->Lock( 0, 0, reinterpret_cast< void** >( &pwIndex ), D3DLOCK_NOSYSLOCK );
			WORD wCnt = 0;
			DWORD dwBlockSize = m_nMaxPolygon * 3;
			for( UINT i = 0 ; i < dwBlockSize ; i += 6 ){
				pwIndex[ i ] = wCnt;
				pwIndex[ i + 1 ] = wCnt + 1;
				pwIndex[ i + 2 ] = wCnt + 3;

				pwIndex[ i + 3 ] = wCnt;
				pwIndex[ i + 4 ] = wCnt + 3;
				pwIndex[ i + 5 ] = wCnt + 2;

				wCnt += 4;
			}
		}
		m_pIndexBuffer->Unlock();
		// ���_�o�b�t�@�����b�N���Ă���
		m_pVertexBuffer->Lock( 0, 0, reinterpret_cast< void** >( &m_pCurrentVertex ), 0 );
		m_bCreateVertex = true;
		return true;
	}
	return false;
}
#endif
