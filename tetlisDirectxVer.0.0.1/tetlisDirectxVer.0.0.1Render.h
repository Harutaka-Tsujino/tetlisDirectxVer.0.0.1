#ifndef TETLIS_DEIRECT_VER_0_0_1_RENDER_H
#define TETLIS_DEIRECT_VER_0_0_1_RENDER_H

#include <windows.h>
#include <d3dx9.h>

//�`��Ɋւ���֐�
VOID Render(VOID);

//�����I�Ȏ��_�̐ݒ������֐�
VOID SetViewPointOverall(VOID);

//�����I�Ȏ���p�̐ݒ������֐�
VOID SetFocusOfViewOverall(VOID);

//��ʂ�\��������֐�
VOID DisplayScreen(CustomVertex * cusV4Background, CustomVertex *cusV4Tetmino, CustomVertex *cusV4Frame, RECT *rectGameoverStr, RECT *rectScoreStr);

#endif
