#pragma once
#include <KamataEngine.h>

#include "Player.h"

using namespace KamataEngine;

class GameScene {

	public:

		//ゲームシーン

		//コンストラクタ
	    GameScene();

		//デストラクタ
	    ~GameScene();

		void Initialize();

		void Update();

	    void Draw();

		private:
	    DirectXCommon* dxCommon_ = nullptr;
	    Input* input_ = nullptr;
	    Audio* audio_ = nullptr;

		//ゲームシーン用メンバ変数
	    Player* player_ = nullptr;
	    Model* model_=nullptr;
	    WorldTransform worldTransform_;
	    Camera viewProjection_;
	   // uint32_t texture = 0;


};
