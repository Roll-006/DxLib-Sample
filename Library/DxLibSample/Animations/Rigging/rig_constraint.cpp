#include <math.hpp>
#include "../../transform.h"
#include "../../component_factory.h"
#include "../../Model/model_renderer.h"
#include "rig_constraint.h"

using namespace DxLibSample;
using namespace DxLibSample::Animations::Rigging;

RigConstraint::RigConstraint() : 
	_weight		(1.f),
	_rigWeight	(1.f)
{
	
}