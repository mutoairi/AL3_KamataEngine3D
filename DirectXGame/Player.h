#pragma once
#include"3d/WorldTransform.h"
#include"3d/ObjectColor.h"
#include"3d/ViewProjection.h"
#include"3d/Model.h"
class Player {
public:
	void Initialize(Model*model,ViewProjection*viewProjection);
	void Update();
	void Draw();

	private:
	ViewProjection* viewProjection_ = nullptr;
	    Model* model_ = nullptr;

};
