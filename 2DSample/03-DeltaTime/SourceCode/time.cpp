#include <DxLib.h>
#include "time.h"

Time::Time() : 
	_firstTimeUs	(GetNowHiPerformanceCount()),
	_currentTimeUs	(_firstTimeUs),
	_prevTimeUs		(_firstTimeUs),
	_frameCount		(1),
	_averageFps		(0.0f),
	_deltaTime		(0.0f),
	_timeScale		(1.0f)
{

}

Time::~Time()
{

}

void Time::Update()
{
	_currentTimeUs	= GetNowHiPerformanceCount();			// Windowsが起動してからの経過時間を取得
	_deltaTime		= (_currentTimeUs - _prevTimeUs) / kUs;	// 1フレーム前との差分(デルタタイム)を計算 (マイクロ秒で割って単位を秒にする)
	_prevTimeUs		= _currentTimeUs;						// 現在の値を保存

	// フレームカウントが目的のFPSに到達した場合、カウントをリセットする
	if (_frameCount == kFPS)
	{
		// 表示のため、平均FPSを計算する
		const auto totalFrameTime = static_cast<float>(_currentTimeUs - _firstTimeUs);
		const auto calcAverage = totalFrameTime / kFPS;
		_averageFps = kUs / calcAverage;

		// カウントリセット
		_firstTimeUs = GetNowHiPerformanceCount();
		_frameCount = 1;
	}
	else
	{
		++_frameCount;
	}
}

void Time::CapFPS() const
{
	// 理想の時間と実際の時間の差分を取得し、時間を停止させる
	const auto idealTimeUs	= kUs / kFPS * _frameCount;
	const auto actualTimeUs = _currentTimeUs - _firstTimeUs;
	const auto waitTimeMs	= (idealTimeUs - actualTimeUs) / kMs;

	// 指定の時間動作を停止
	if (waitTimeMs > 0 && waitTimeMs <= kMaxWaitTimeMs)
	{
		WaitTimer(static_cast<int>(waitTimeMs));
	}
}
