#ifndef TETLIS_DIRECTX_VER_0_0_1_CONTROL_H
#define TETLIS_DIRECTX_VER_0_0_1_CONTROL_H

#include <windows.h>

//�e�g���X�Ȃǂ̑���Ɋւ���֐�
VOID Control(VOID);

//�e�g���X�z��ɑI�΂ꂽ�p�^�[�����R�s�[����֐�
VOID ChooseAndCpyTetlisBoardSourceToBoard(VOID);

//�e�g���X�{�[�h������������֐�
VOID InitTetlisBoard(VOID);

//�e�g���X�{�[�h�o�b�t�@�[������������֐�
VOID InitTetlisBoardBuf(VOID);

//�z�[���h�A�l�N�X�g�A�l�N�X�g�l�N�X�g������������֐�
VOID InitNextAndNextNext(BOOL *isNewGame);

//�z�[���h�A�l�N�X�g�A�l�N�X�g�l�N�X�g�{�[�h�̒��g���X�V����֐�
VOID UpdateHoldNextNextNextBoard(VOID);

//�e�g���X�z��o�b�t�@�[�ɉ��e�g���~�m���������e�g���X�z��̗v�f�𓯊�������֐�
VOID SynchroTetlisBoardBufToTetlisBoard(VOID);

//�e�g���X�z��ɉ��e�g���~�m�𓯊�������֐�
VOID SynchroTetlisBoardToMovMinoNumOfArBuf(INT currentTetmino);

//�e�g���X�Ɋւ���f�[�^��������Ԃɖ߂��֐�
VOID ReturnToInitialStateWithTetlis(BOOL *isGameover, BOOL *canCreate, BOOL *canInputRA, BOOL *canInputLA, BOOL *canInputDA, BOOL *canInputR, BOOL *canInputSpace,
	BOOL *canHold, BOOL *wasHold, INT *rACount, INT *lACount, INT *dACount, INT *stopCount, INT *downCount, INT *scoreBuf, INT *currentTetmino, INT *minoIRoatationCount);

//�e�g���~�m�𐶐��Ɋւ���֐�
VOID CreateTetlimino(INT currentTetmino, BOOL *canInputLA, BOOL *canInputDA, BOOL *canInputRA, 
	BOOL *canInputR, BOOL *canInputSpace, BOOL *wasHold, INT *stopCount, INT *downCount, INT *minoIRoatationCount, BOOL *canCreate);

//�L�[���͂��J�E���g���Ƃ�\�ɂ���֐�
VOID CountToMakeFlagTrue(BOOL *canInputLA, INT *lACount);

//�e�g���~�m���z�[���h���邱�ƂɊւ���֐�
VOID HoldTetlimino(BOOL *canHold, INT *currentTetmino, BOOL *canCreate, BOOL *wasHold);

//�e�g���~�m��X�������̈ړ��Ɋւ���֐�
VOID ShiftTetliminoX(INT shiftX, BOOL *canInputRA);

//�e�g���~�m�̉�]�Ɋւ���֐�
VOID RotateTetlimino(INT *minoIRoatationCount, INT currentTetmino);

//�e�g���~�m��Y�������̈ړ��Ɋւ���֐�
VOID DownTetlimino(BOOL *canInputDA);

//�e�g���~�m�̃n�[�h�h���b�v�Ɋւ���֐�
VOID HardDropTetlimino(VOID);

//�e�g���~�m�̃^�[�Q�b�g�̍��W�����킹��֐�
VOID SetTetliminoTarget(VOID);

//�e�g���~�m�̎��������Ɋւ���֐�
VOID CountToDawnTetlimino(INT *downCount);

//�e�g���~�m��~�Ɋւ���֐�
VOID CountToStopTetlimino(INT *stopCount, INT *currentTetmino,BOOL *canCreate, BOOL *canHold, BOOL *wasHold);

//��������e�g���X�̃��C���������J�E���g���Ƃ�֐�
VOID DeleteAndCountFilledLine(INT *lineCount);

//�����ꂽ�e�g���X�̃��C�������ɂ��炷�֐�
VOID ShiftTetlisLine(VOID);

//�e�g���X�̃X�R�A�Ɋւ���֐�
VOID GetScoreByLineCount(INT lineCount, INT *scoreBuf);

//�Q�[���I�[�o�[���A�ۂ��𒲂ׂ�֐�
VOID CheckGameover(BOOL *isGameover);

#endif
