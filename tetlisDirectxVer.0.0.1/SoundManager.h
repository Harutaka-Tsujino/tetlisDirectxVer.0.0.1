#ifndef SOUNDBUFFERMANAGER_H
#define SOUNDBUFFERMANAGER_H

#include<map>
#include<string>
#include<mmsystem.h>
#include<dsound.h>

//注意：DirectSoundを生成してからでないとエラーを起こします
class SoundManager{
public:
	static SoundManager& GetInstance();

	bool Load(const char* filePath);

	void Remove(std::string filePath);

	void SetVolume(std::string filePath, LONG volume);

	void Play(std::string filePath, bool isLoop);

	void Resume(std::string filePath, bool isLoop);

	void Stop(std::string filePath);

private:
	static SoundManager* m_pInstance;
	SoundManager();
	~SoundManager();

	std::map<std::string, IDirectSoundBuffer8*> m_PtrDirectSoundBuffers;

	bool WasLoaded(std::string filePath);

	SoundManager(const SoundManager&);
	void operator =(const SoundManager&);
};
#endif