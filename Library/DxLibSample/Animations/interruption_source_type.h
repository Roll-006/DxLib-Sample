#pragma once

namespace DxLibSample::Animations
{
    enum class InterruptionSourceType : uint8_t
    {
        kNone, // 遷移の割り込みは許可しない
        kCurrentState, // 現在の State から他の State への遷移の割り込みを許可する
        kNextState, // 遷移先の State から他の State への遷移の割り込みを許可する
        kCurrentStateThenNextState, // kCurrentState と kNextState が同時に発生した場合、kCurrentState を優先する
        kNextStateThenCurrentState, // kCurrentState と kNextState が同時に発生した場合、kNextState を優先する
    };
}