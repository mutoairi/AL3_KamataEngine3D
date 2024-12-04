#include "Player.h"
#include"math/Vector3.h"
#include"myMath.h"
#include<imgui.h>


Player::~Player()
{
	for (PlayerBullet* bullet_ : bullets_) {
		delete bullet_;
	}
}

void Player::Initialize(KamataEngine::Model* model, KamataEngine::Camera* viewProjection) {
	model_ = model;
	viewProjection_ = viewProjection;

	worldTransform_.Initialize();
	//シングルトンインスタンスを取得する
	input_ = KamataEngine::Input::GetInstance();

	objColor.Initialize();

}
void Player::Update() {

	//デスフラグが立った弾を削除
	bullets_.remove_if([](PlayerBullet* bullet_) {
		if (bullet_->IsDead()) {
			delete bullet_;
			return true;
		}
		return false;
		});
	//旋回処理
	Rotate();
	//移動処理
	Move();
	//攻撃処理
	Attack();
	//弾更新
	for (PlayerBullet*bullet_:bullets_) {
		bullet_->Update();
	}
	worldTransform_.UpdateMatrix();


	//キャラクターの座標を画面表示する処理
	ImGui::Begin("Window");
	ImGui::DragFloat3("Player", &worldTransform_.translation_.x, 0.1f);
	ImGui::End();


};

void Player::Draw() {
	model_->Draw(worldTransform_, *viewProjection_, &objColor);
	//弾描画
	for (PlayerBullet* bullet_ : bullets_) {
		bullet_->Draw(*viewProjection_);
	}
}

void Player::Rotate()
{
	//回転速さ
	const float kRotSpeed = 0.02f;
	//押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_A)) {
		worldTransform_.rotation_.y -= kRotSpeed;
	}
	else if (input_->PushKey(DIK_D)) {
		worldTransform_.rotation_.y += kRotSpeed;
	}

}

void Player::Attack()
{
	if (input_->TriggerKey(DIK_SPACE)) {
		//弾の速度
		const float kBulletSpeed = 1.0f;
		KamataEngine::Vector3 velocity(0, 0, kBulletSpeed);
		//速度ベクトルを自機の向きに合わせて回転させる
		velocity = TransformNormal(velocity, worldTransform_.matWorld_);
		
		//弾を生成し初期
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_, worldTransform_.translation_,velocity);

		//弾を登録する
		bullets_.push_back(newBullet);
	}
}

void Player::Move()
{
	//キャラクターの移動ベクトル
	KamataEngine::Vector3 move = { 0, 0, 0 };

	//キャラクターの移動の速さ
	const float kCharacterSpeed = 0.2f;

	//押した方向で移動ベクトル
	if (input_->PushKey(DIK_LEFT)) {
		move.x -= kCharacterSpeed;
	}
	else if (input_->PushKey(DIK_RIGHT)) {
		move.x += kCharacterSpeed;
	}
	if (input_->PushKey(DIK_UP)) {
		move.y += kCharacterSpeed;
	}
	else if (input_->PushKey(DIK_DOWN)) {
		move.y -= kCharacterSpeed;
	}
	//移動限界座標
	const float kMoveLimitX = 30.0f;
	const float kMoveLimitY = 18.0f;



	worldTransform_.translation_ += move;

	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);
}

KamataEngine::Vector3 Player::GetWorldPosition()
{
	//ワールド座標を入れる変数
	KamataEngine::Vector3 worldPos;
	//ワールド座標を取得
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}
