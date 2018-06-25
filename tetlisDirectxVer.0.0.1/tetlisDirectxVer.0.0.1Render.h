/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Render
//
// DATE 2018.06.13
// VER.0.0.5
//
// Author Harutaka-Tsujino
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef TETLIS_DEIRECT_VER_0_0_1_RENDER_H
#define TETLIS_DEIRECT_VER_0_0_1_RENDER_H

#include <windows.h>

typedef struct
{
	FLOAT x, y, xScale, yScale;
}ImageState;

extern ImageState g_tetminoState;
extern ImageState g_gameoverStrState;
extern ImageState g_scoreStrState;
extern ImageState g_undergroundStrState;

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

//�w�i��`�悷��֐�
VOID RenderBackground(VOID);

//���C���������ꂽ���̃G�t�F�N�g��`�悷��֐�
VOID SetDeletedLineEffectTextureAndRender(VOID);

//�J�E���g�ɂ��G�t�F�N�g�̃A�j���[�V�����p�̐ݒ���s���֐�
VOID CountToAnimation(INT *additionalDeletedEffectScale, INT *deletedLineEffectCaunt, CustomVertex *cusV4DeletedLineEffect);

//�z����`�悷��֐�
VOID RnderFrame(VOID);

//�Q�[���I�[�o�[���̕������`�悷��֐�
VOID RenderGameoverStr(VOID);

//�X�R�A�̕������`�悷��֐�
VOID RenderScoreStr(VOID);

//�ǂ̂��炢�n�ʂ��@��i�񂾂��̕������`�悷��֐�
VOID RenderUndergroundStr(VOID);

//�e�g���X�̃u���b�N�̂S���_��ݒ肵�A�`�悷��֐�
VOID SetBlockVerticesAndRender(VOID);

//�J�X�^���o�[�e�b�N�X��tu,tv���f�t�H���g�ɖ߂��֐�
VOID ResetCustomVertexTuTv(VOID);

//�z�[���h�A�l�N�X�g�A�l�N�X�g�l�N�X�g�̂S���_��ݒ肵�A�`�悷��֐�
VOID SetHoldNextNextNextVerticesAndRender(VOID);

//�e�g���~�m�̃^�[�Q�b�g�̂S���_��ݒ肵�A�`�悷��֐�
VOID SetTetliminoTargetTextureAndRender(VOID);

//�^�[�Q�b�g�u���b�N�̃A���t�@�l��_�ł����邽�߂ɕϊ�����֐�
VOID UnderGoChangeTarAlpha(CustomVertex *cusV4TarTetmino);

#endif
