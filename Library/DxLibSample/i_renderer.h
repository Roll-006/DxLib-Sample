#pragma once

namespace DxLibSample
{
    class IRenderer abstract
    {
    public:
        virtual ~IRenderer() = default;
        
        virtual void Render() const abstract;
        virtual void DebugRender() const abstract;
    };
}