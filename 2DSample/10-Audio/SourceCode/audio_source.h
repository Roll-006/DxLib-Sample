#pragma once

class AudioSource
{
public:
	AudioSource(const std::string& filePath);

	void OnPlay();
	void OnStop();

	bool IsPlaying() const { return CheckSoundMem(_audioHandle); }

	/// <summary>
	/// 音量を設定
	/// </summary>
	/// <param name="volume">音量 (範囲 : 0.0f～1.0f)</param>
	void SetVolume(const float volume);

	/// <summary>
	/// パンを設定する
	/// </summary>
	/// <param name="pan">パン (範囲 : -1.0f [left] ～1.0f [right] )</param>
	void SetPan(const float pan);

	/// <summary>
	/// 周波数を設定
	/// </summary>
	/// <param name="frequency">周波数 (範囲 : 100～100,000) -1 : デフォルト</param>
	void SetFrequency(const int frequency);

	/// <summary>
	/// ループ再生するかを設定
	/// </summary>
	/// <param name="isLoop">true : ループ再生する, false : ループ再生しない</param>
	void SetLoop(const bool isLoop) { _isLoop = isLoop; }

private:
	int		_audioHandle;
	float	_volume;
	float	_pan;
	int		_frequency;
	bool	_isLoop;
};