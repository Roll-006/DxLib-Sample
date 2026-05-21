#pragma once

class ModelRenderer
{
public:
	ModelRenderer(Model& model);

	void Render() const;

private:
	Model& _model;
};
