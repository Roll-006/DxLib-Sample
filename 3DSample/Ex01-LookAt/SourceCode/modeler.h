#pragma once

class Modeler
{
public:
	Modeler(const std::string& filePath, Transform& transform);
	~Modeler();

	void Draw() const;

	int GetModelHandle() const { return _modelHandle; }

private:
	int _modelHandle;
	Transform& _transform;
};