#pragma once
#include <KamataEngine.h>


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

};
