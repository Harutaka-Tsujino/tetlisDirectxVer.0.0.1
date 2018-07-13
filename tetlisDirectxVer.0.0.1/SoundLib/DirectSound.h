#ifndef DIRECTSOUND_H
#define DIRECTSOUND_H

#include<mmsystem.h>
#include<dsound.h>

class DirectSound{
public:
	static void CreateInstance(HWND hWnd);
	static DirectSound& GetInstance();

	IDirectSound8** GetDevice();

private:
	static DirectSound* m_pInstance;
	DirectSound(HWND hWnd);
	~DirectSound();

	IDirectSound8* m_pDirectSound;

	DirectSound(const DirectSound&);
	void operator =(const DirectSound&);
};
#endif