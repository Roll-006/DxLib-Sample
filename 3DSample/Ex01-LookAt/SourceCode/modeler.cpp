#include <DxLib.h>
#include <Vector/vector3.hpp>
#include <Matrix/matrix_4x4.hpp>
#include <string>
#include "transform.h"
#include "modeler.h"

Modeler::Modeler(const std::string& filePath, Transform& transform) : 
	_modelHandle(MV1LoadModel(filePath.c_str())),
	_transform	(transform)
{

}

Modeler::~Modeler()
{
	MV1DeleteModel(_modelHandle);
}

void Modeler::Draw() const
{
	MV1SetMatrix(_modelHandle, _transform.GetMatrix());
	MV1DrawModel(_modelHandle);
}
