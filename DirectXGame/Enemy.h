#pragma once
#include<3d/WorldTransform.h>
#include<3d/ObjectColor.h>
#include<3d/Model.h>
#include<3d/Camera.h>
#include<input/Input.h>


//行動フェーズ
enum class Phase {
	Approach,//接近する
	Leave,//離脱する
};
class Enemy
{
public:
	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* viewProjection);
	void Update();
	void Draw();
	void Approach();
	void Leave();
	

private:

	KamataEngine::Input* input_ = nullptr;
	KamataEngine::Camera* viewProjection_ = nullptr;
	KamataEngine::Model* model_ = nullptr;
	KamataEngine::WorldTransform worldTransform_;
	//KamataEngine::ObjectColor objColor = {};
	uint32_t texture_ = 0u;
	//フェーズ
	Phase phase_;
};

