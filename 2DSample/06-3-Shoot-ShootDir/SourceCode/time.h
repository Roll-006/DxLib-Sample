#pragma once

class Time
{
public:
	/// @brief インスタンスを取得
	static Time& GetInstance()
	{
		// ローカル静的オブジェクトにより実装
		// ※オブジェクトの解放タイミングを任意に決めることはできない
		static Time instance;
		return instance;
	}

	void Update();

	/// @brief FPSに上限を設ける
	void CapFPS() const;

	void SetTimeScale(const float timeScale) { _timeScale = timeScale; }

	float GetTimeScale() const			{ return _timeScale; }
	float GetDeltaTime() const			{ return _deltaTime * _timeScale; }
	float GetUnscaledDeltaTime() const	{ return _deltaTime; }
	float GetAverageFPS() const			{ return _averageFps; }

private:
	Time();

private:
	static constexpr float kFPS				= 60.0f;
	static constexpr float kMaxWaitTimeMs	= 1000;
	static constexpr float kMs				= 1000.0f;		// ミリ秒
	static constexpr float kUs				= 1000000.0f;	// マイクロ秒

	LONGLONG	_firstTimeUs;
	LONGLONG	_currentTimeUs;
	LONGLONG	_prevTimeUs;
	int			_frameCount;
	float		_averageFps;
	float		_deltaTime;
	float		_timeScale;
};
