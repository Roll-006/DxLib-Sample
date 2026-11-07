#pragma once

namespace Core
{
	class Time final
	{
	public:
		/// @brief インスタンスを取得
		static Time& I()
		{
			// ローカル静的オブジェクトにより実装
			// ※オブジェクトの解放タイミングを任意に決めることはできない
			static Time instance;
			return instance;
		}

		void Update();

		/// @brief FPSに上限を設ける
		void CapFPS() const;

		[[nodiscard]] float GetTimeScale() const			{ return _timeScale; }
		[[nodiscard]] float GetDeltaTime() const			{ return _deltaTime * _timeScale; }
		[[nodiscard]] float GetUnscaledDeltaTime() const	{ return _deltaTime; }
		[[nodiscard]] float GetAverageFPS() const			{ return _averageFps; }

		void SetTimeScale(const float timeScale) { _timeScale = timeScale; }

	private:
		Time();

	private:
		static constexpr float kFPS				= 60.f;
		static constexpr float kMaxWaitTimeMs	= 1000;
		static constexpr float kMs				= 1000.f;		// ミリ秒
		static constexpr float kUs				= 1000000.f;	// マイクロ秒

		LONGLONG	_firstTimeUs;
		LONGLONG	_currentTimeUs;
		LONGLONG	_prevTimeUs;
		int			_frameCount;
		float		_averageFps;
		float		_deltaTime;
		float		_timeScale;
	};
}