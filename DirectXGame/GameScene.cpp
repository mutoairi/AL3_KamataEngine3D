#include "GameScene.h"
#include <cassert>



GameScene::GameScene() {};
GameScene::~GameScene() {
	delete model_;
	delete player_;
	delete debugCamera_;
};


void GameScene::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	//texture = TextureManager::Load("white1x1.png");
	model_ = Model::CreateFromOBJ("block", true);
	viewProjection_.Initialize();
	worldTransform_.Initialize();
	// 自キャラの生成
	player_ = new Player();
	player_->Initialize(model_, &viewProjection_);
	//敵の生成
	enemy_ = new Enemy();
	enemy_->Initialize(model_, &viewProjection_);
	//デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);

	//軸方向の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	AxisIndicator::GetInstance()->SetTargetCamera(&viewProjection_);
}

void GameScene::Update() {
#ifdef _DEBUG
	if (input_->TriggerKey(DIK_0)) {
		if (isDebugCameraActive_ == true)
			isDebugCameraActive_ = false;
		else
			isDebugCameraActive_ = true;

	}

#endif
	if (isDebugCameraActive_) {
		// デバッグカメラの更新
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetCamera().matView;
		viewProjection_.matProjection = debugCamera_->GetCamera().matProjection;
		//ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();
	}
	else {
		//ビュープロジェクション行列の更新と転送	
		viewProjection_.UpdateMatrix();
	}
	player_->Update();
	enemy_->Update();
};

void GameScene::Draw() {
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	KamataEngine::Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// 
	player_->Draw();
	enemy_->Draw();
	/// </summary>

	// 3Dオブジェクト描画後処理
	KamataEngine::Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}