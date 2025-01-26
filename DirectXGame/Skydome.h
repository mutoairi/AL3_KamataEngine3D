#pragma once
#include "3d/Camera.h"
#include "3d/Model.h"
#include "3d/ObjectColor.h"
#include "3d/WorldTransform.h"

///=====================================================///
/// 天球
///=====================================================///
class Skydome {

	///-------------------------------------------///
	/// メンバ関数
	///-------------------------------------------///
public:

	//初期化
	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* viewProjection);

	//更新
	void Update();

	//描画
	void Draw();

private:

	// ワールド変換データ
	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::Camera* viewProjection_ = nullptr;

	// モデル
	KamataEngine::Model* model_ = nullptr;
	KamataEngine::ObjectColor objColor = {};
};