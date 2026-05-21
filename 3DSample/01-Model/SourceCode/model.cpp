#include <math.hpp>
#include "transform.h"
#include "model.h"

Model::Model(const std::string& filePath, Transform& transform) :
	_modelHandle(MV1LoadModel(filePath.c_str())),
	_transform	(transform)
{

}

Model::Model(const int modelHandle, Transform& transform) :
	_modelHandle(modelHandle),
	_transform	(transform)
{

}

Model::~Model()
{
	if (_modelHandle != -1)
	{
		MV1DeleteModel(_modelHandle);
	}
}
