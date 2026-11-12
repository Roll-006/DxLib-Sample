#pragma once

namespace DxLibSample
{
    class Screen final
    {
    public:
        /// <summary>
        /// インスタンスを取得
        /// </summary>
        static Screen& I()
        {
            // ローカル静的オブジェクトにより実装
            // ※オブジェクトの解放タイミングを任意に決めることはできない
            static Screen instance;
            return instance;
        }
		
        void UpdateScreenMode();
        void SetResolution(const Vector2& screenSize, bool isFullScreen);
        
        [[nodiscard]] Vector2 GetScreenSize() const { return _screenSize; }
        [[nodiscard]] bool IsFullScreen() const { return _isFullScreen; }
		
    public:
        static constexpr Vector2 kFullHD = { 1920.f, 1080.f };
        static constexpr Vector2 kWXGA = { 1366.f, 768.f };
        static constexpr Vector2 kQHD = { 2560.f, 1440.f };
        static constexpr Vector2 k4KUHD = { 3840.f, 2160.f };
        static constexpr int kColorBitDepth = 32;
        
    private:
        Screen();
		
    private:
        Vector2 _screenSize;
        bool _isFullScreen;
        int _changeScreenModeCount;
    };
}