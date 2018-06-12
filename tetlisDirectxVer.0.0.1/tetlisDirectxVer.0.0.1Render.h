#ifndef TETLIS_DEIRECT_VER_0_0_1_RENDER_H
#define TETLIS_DEIRECT_VER_0_0_1_RENDER_H

#include <windows.h>
#include <d3dx9.h>

typedef struct
{
	FLOAT x, y, xScale, yScale;
}ImageState;

extern ImageState g_tetminoState;
extern ImageState g_GameoverStrState;
extern ImageState g_scoreStrState;

typedef struct
{
	FLOAT x, y, z, rhw;
	DWORD	color;
	FLOAT	tu, tv;
}CustomVertex;

//�`��Ɋւ���֐�
VOID Render(VOID);

//�����I�Ȏ��_�̐ݒ������֐�
VOID SetViewPointOverall(VOID);

//�����I�Ȏ���p�̐ݒ������֐�
VOID SetFocusOfViewOverall(VOID);

//��ʂ�\��������֐�
VOID DisplayScreen(CustomVertex * cusV4Background, CustomVertex *cusV4Tetmino, CustomVertex *cusV4Frame,CustomVertex *cusV4TarTetmino, RECT *rectGameoverStr, RECT *rectScoreStr);

//�e�g���X�̃u���b�N�̂S���_��ݒ肵�A�`�悷��֐�
VOID SetBlockVerticesAndRender(CustomVertex *cusV4Tetmino);

//�z�[���h�A�l�N�X�g�A�l�N�X�g�l�N�X�g�̂S���_��ݒ肵�A�`�悷��֐�
VOID SetHoldNextNextNextVerticesAndRender(CustomVertex *cusV4Tetmino);

//�e�g���~�m�̃^�[�Q�b�g�̂S���_��ݒ肵�A�`�悷��֐�
VOID SetTetliminoTargetTextureAndRender(CustomVertex *cusV4Tetmino, CustomVertex *cusV4TarTetmino);

//�^�[�Q�b�g�u���b�N�̃A���t�@�l��_�ł����邽�߂ɕϊ�����֐�
VOID UnderGoChangeTarAlpha(CustomVertex *cusV4TarTetmino);

#endif
