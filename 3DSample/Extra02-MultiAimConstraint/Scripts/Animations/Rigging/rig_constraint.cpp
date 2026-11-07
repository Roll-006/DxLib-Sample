#include <math.hpp>
#include "../../Core/transform.h"
#include "../../Core/component_factory.h"
#include "../../Model/model_renderer.h"
#include "rig_constraint.h"

using namespace Core;
using namespace Animations::Rigging;

RigConstraint::RigConstraint() : 
	_weight		(1.f),
	_rigWeight	(1.f)
{
	
}