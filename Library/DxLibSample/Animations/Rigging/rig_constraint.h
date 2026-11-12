#pragma once
#include "../../component.h"

namespace DxLibSample
{
	class GameObject;
}

namespace DxLibSample::Animations::Rigging
{
	class RigConstraint abstract : public Component
	{
	public:
		RigConstraint();
		
		[[nodiscard]] float GetWeight() const { return _weight; }
		[[nodiscard]] float GetEffectiveWeight() const { return _weight * _rigWeight; }
		
		void SetWeight(float weight) { _weight = std::clamp(weight, 0.f, 1.f); }
		void SetRigWeight(float rigWeight) { _rigWeight = std::clamp(rigWeight, 0.f, 1.f); }
	
	protected:
		float _weight;
		float _rigWeight;
	};
}