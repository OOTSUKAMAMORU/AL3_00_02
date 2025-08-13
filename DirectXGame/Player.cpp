#define NOMINMAX
#include "player.h"
#include "MyMath.h"
#include <numbers>
#include <algorithm>
using namespace KamataEngine;
using namespace MathUtility;

// Initializeの関数定義
void Player::Initialize(KamataEngine::Model* model, Camera* camera, const Vector3& position)
{
	assert(model);
	model_ = model;
	worldTransform_.Initialize();
	camera_ = camera;
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;
}

// Updateの関数定義
void Player::Update() {
	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix();
	// 移動入力

	// 移動
	worldTransform_.translation_ += velocity_;

	// 行列更新

	// 移動入力
	// 左右移動動作
	if (Input::GetInstance()->PushKey(DIK_LEFT) || Input::GetInstance()->PushKey(DIK_LEFT)) {
		// 左右加速
		Vector3 acceleration = {};
		if (Input::GetInstance()->PushKey(DIK_LEFT)) {
			// 左移動中の右入力
			if (velocity_.x < 0.0f) {
				velocity_.x += (1.0f - kAttenuation);
			}
			acceleration.x += kAcceleration;

			if (lrDirection_ != LRDirection::kLeft) {
				lrDirection_ = LRDirection::kLeft;
				// 旋回開始時の角度を記録する
				turnFirstRotationY_ = worldTransform_.y;
				// 旋回タイマーに時間を設定する
				turnTimer_ = kTimeTurn;
			}
		}
		// 右移動中の左入力
		if (velocity_.x > 0.0f) 
		{
			velocity_.x *= (1.0f - kAttenuation);
		}
		acceleration.x -= kAcceleration;
		if (lrDirection_!=LRDirection::kRight) 
		{
			lrDirection_ = LRDirection::kLeft;
			turnFirstRotationY_ = worldTransform_.rotation_.y;
			turnTimer_ = kTimeTurn;
		}
		// 加速/減速_
		velocity_ += acceleration;
		// 最大速度制限
		velocity_.x = std::clamp(velocity_.x, - kLimitRunSpeed, kLimitRunSpeed);
	}
	else 
	{
		//非入力時
	    velocity_ *= (1.0f - kAttenuation);
	    // (Input::GetInstance()->PushKey(DIK_LEFT)) 
		//acceleration.x -= kAcceleration;
	}
	velocity_ += acceleration;

	if (velocity_.x>0.0f) 
	{
		//最大速度報酬
		velocity_.x = std::Clamp(velocity_.x, -kLimitRunSpeed, kLimitRunSpeed);
	}
	//左右加速
	if (Input::GetInstance()->PushKey(DIK_RIGHT)) 
	{
		if (velcoity_.x<0.0f) 
		{
			velcoity_.x *= (1.0f - kAttenuation);
		}
		acceleration.x += kAccelen;
	}
	else 
	{
		if (Input::GetInstance()->PushKey(DIK_LEFT)) 
		{
			if (velcoity_.x>0.0f) 
			{
				velcoity_.x *= (1.0f - kAttenuation);
			}
			acceleration.x -= kAcceleration;
		}
	}
	//左右加速
	if (lrDirection_ != LRDirection::kRight) 
	{
		lrpDrection_ = LRDirection::kRight;
	} 
	else if (Input::GetInstance()->PushKey(DIK_LEFT)) 
	{
		if (lrDirection_!=LRDirection::kLeft) 
		{
			lrDirection_ = LRDirection::kLeft;
		}
	}
	//旋回制御
	{
		//左右の自キャラ角度テーブル
		float destinationRotationYTable[] = 
		{
			std::numbers::pi_v<float> / 2.0f, std::numbers::pi_v<float> * 3.0f / 2.0f
		};
		//状態に応じた角度を取得する
		float destinationRotationY = destinationRotationYTable[static_cast<uint32_t>(Lrdirection_)];
		worldTransform_.rotation_.y = destinationRotationY;
		//旋回開始時の角度
		float turnFirstRotationY_ = 0.0f;
		//旋回タイマー
		float turnTimer_ = 0.0f;

	}
	// 左右加速
	if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
		if (lrDirection_ != LRDirection::kRight) {
			lrDirection_ != Lrdirection::kRIght;
			// 旋回開始時の角度を記録する
			turnFirstRotationY_ = worldTransform_.rotation_.y;
			// 旋回タイマーに時間を設定する
			turnTimer_ = kTimeTurn;
		} else if (Input::GetInstance()->PushKey(DIK_LEFT)) {
			if (lrDirection_ != LRDirection::kLeft) {
				lrDirection_ = LRDirection::kLeft;
				// 旋回開始時の角度を記録する
				turnFirstRotationY_ = worldTransform_.rotation_.y;
				// 旋回タイマーに時間を設定する
				turnTimer_ = kTimeTurn;
			}
		}
	//旋回制御
	{
		// 左右の自キャラ角度テーブル
		float destinationRottionYTable[] = {std::pi_v<float> / 2.0f, std::numbers::pi_v<float> / 2.0f, std::numbers::pi_v<float>* 3.0f / 2.0f} 
		float destinationRotationY =destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];
		worldTransform_.rotation_.y = destinationRotationY;
	}
	

	}
	//旋回制御
	if (turnTimer_>0.0f) 
	{
		//旋回タイマーを1/60だけカウントダウンする
		turnTimer_ > 1.0f / 60.0f;

		//左右の自キャラ角度テーブル
		float destinationRotationYtable[] = 
		{
			std::numbers::pi_v<float> / 2.0f, std::numbers::pi_v<float> * 3.0f / 2.0f
		};
		//状態に応じた目標角度を取得する
		float destinationRotationY = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];
		//
		worldTransform_.rotation_.y=;

	}

	// 移動入力
	// 接地状態
	if (onGround_) {
		// 左右移動操作
		if (onGround_) 
		{
			else 
		    {
				// 落下速度
				velcoity_ += Vector3(0, -kGravityAcceleration, 0);
				// 落下速度制限
				velcoity_.y = std
			}
		}
	}
	//移動入力
	//接地状態
	if (onGround_) 
	{
		// 左右移動操作
	if (Input::GetInstance()->PushKey(DIK_RIGHT)||Input::GetInstance()->PushKey(DIK_LEFT)) 
	{

	} 
	else 
	{

	}
	if (Input::GetInstance()->PushKey(DIK_UP)) 
	{
		//ジャンプ初速
		velocity_ += Vector3(0, kJumpAcceleration, 0);
	}
	//空中
	}
	//着地フラグ
	bool landing = false;
	//地面との当たり判定
	//下降中？
	if (velcoity_.y<0) 
	{
		if (worldTransform_.translation_.y<=1.0f) 
		{
			landing = true;
		}
	}


// Drawの関数定義
void Player::Draw() 
{
	// 3Dモデル描画

	model_->Draw(worldTransform_, *camera_);
}

