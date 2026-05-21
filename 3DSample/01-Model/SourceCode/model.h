#pragma once

class Model
{
public:
	Model(const std::string& filePath, Transform& transform);
	Model(const int modelHandle, Transform& transform);
	~Model();

	void ApplyMatrix() { MV1SetMatrix(_modelHandle, ToDxLibMatrix(_transform.GetMatrix())); }

	int GetModelHandle() const { return _modelHandle; }

private:
	int _modelHandle;
	Transform& _transform;
};
