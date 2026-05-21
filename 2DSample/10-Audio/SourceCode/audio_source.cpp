#include <string>
#include <DxLib.h>
#include "audio_source.h"

AudioSource::AudioSource(const std::string& filePath) : 
	_audioHandle(LoadSoundMem(filePath.c_str())),
	_isLoop		(false),
	_volume		(1.0f),
	_frequency	(-1),
	_pan		(0.0f)
{

}

void AudioSource::OnPlay()
{
	PlaySoundMem(_audioHandle, _isLoop ? DX_PLAYTYPE_LOOP : DX_PLAYTYPE_BACK, TRUE);
}

void AudioSource::OnStop()
{
	StopSoundMem(_audioHandle);
}

void AudioSource::SetVolume(const float volume)
{
	_volume = volume;
	ChangeVolumeSoundMem(_volume * UCHAR_MAX, _audioHandle);
}

void AudioSource::SetFrequency(const int frequency)
{
	_frequency = frequency;
	SetFrequencySoundMem(_frequency, _audioHandle);
}

void AudioSource::SetPan(const float pan)
{
	_pan = pan;
	ChangePanSoundMem(_pan * UCHAR_MAX, _audioHandle);
}
