#ifndef TETLIS_DIRECTX_VER_0_0_1_INITIALIZE_H
#define TETLIS_DIRECTX_VER_0_0_1_INITIALIZE_H

#include<windows.h>
#include <d3dx9.h>
#include <dinput.h>

extern LPDIRECT3D9 g_pDirect3D;
extern LPDIRECT3DDEVICE9 g_pD3dDevice;
extern LPDIRECTINPUT8 g_pDinput;
extern LPDIRECT3DTEXTURE9 g_pTexture[g_texMax];
extern LPDIRECTINPUTDEVICE8 g_pDKeyDevice;
extern LPD3DXFONT g_pFont[g_fontMax];
extern D3DPRESENT_PARAMETERS g_D3dPresentParameters;

//�_�C���N�g3D�Ɋւ��鏉�����̊֐�
HRESULT InitDirect3D(HWND hWnd);

//�����I�ȃo�b�t�@�̐ݒ������֐�
VOID SetBuckBufferOverall(VOID);

//�����I�ȕ`��̐ݒ������֐�
VOID SetRenderStateOverall(VOID);

//�����I�ȃe�N�X�`���K�w�̐ݒ������֐�
VOID SetTextureStageStateOverall(VOID);

//�e�N�X�`�������֐�
VOID CreateTextureFromFile(VOID);

//�_�C���N�g�C���v�b�g�Ɋւ��鏉�����̊֐�
HRESULT InitDinput(HWND hWnd);

//�_�C���N�g�t�H���g�Ɋւ��鏉�����̊֐�
HRESULT InitDfont(HWND hWnd);

//�t�H���g�����֐�
VOID CreateFont(VOID);

#endif
