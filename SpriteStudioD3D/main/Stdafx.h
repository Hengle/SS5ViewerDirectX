/*
 * @�T�v		�v���R���p�C���ς݃w�b�_�[
 *
 * $Id: $
 *
 * @author		��
 * @since		2014/07/18
 */

#pragma once

//RELEASE���s�̏ꍇ�́Avector�͈̔͊O�`�F�b�N�𖳌��ɂ���
#undef _SECURE_SCL
#ifndef _DEBUG
#define _SECURE_SCL 0
#else
#define _SECURE_SCL 1
#endif

// VC2005�ȍ~�łł郏�[�j���O��}�~
#define _CRT_SECURE_NO_WARNINGS 1

// �s�v�ȃw�b�_����菜��
#define WIN32_LEAN_AND_MEAN

#define DLL_EXPORT 

// �V�X�e���w�b�_�̒�`
#include <Windows.h>
#include <Process.h>
#include <mbctype.h>
#include <conio.h>

#include <string>
#include <vector>
#include <map>
#include <set>
#include <stack>
#include <list>
#include <deque>

#include <unknwn.h>
#include <atlbase.h>

#pragma comment( lib, "d3d9.lib" )
#pragma comment( lib, "d3dx9.lib" )
