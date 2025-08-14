#define NOMINMAX
#include "player.h"
#include "MyMath.h"
#include <algorithm>
#include <numbers>
using namespace KamataEngine;
using namespace MathUtility;

// Initializeの関数定義
void Player::Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& position) {
	assert(model);
	model_ = model;
	worldTransform_.Initialize();
	camera_ = camera;
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;
}

// Updateの関数定義
void Player::Update() 
{
	if (onGround_) {
		// 移動入力

		// 行列更新

		// 移動入力
		// 左右移動動作
		if (Input::GetInstance()->PushKey(DIK_LEFT) || Input::GetInstance()->PushKey(DIK_LEFT)) {
			// 左右加速
			Vector3 acceleration = {};
			if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
				// 左移動中の右入力
				if (velocity_.x < 0.0f) {
					velocity_.x += (1.0f - kAttenuation);
				}
				acceleration.x += kAcceleration;

				if (lrDirection_ != LRDirection::kLeft) {
					lrDirection_ = LRDirection::kLeft;
					// 旋回開始時の角度を記録する
					turnFirstRotationY_ = worldTransform_.translation_.y;
					// 旋回タイマーに時間を設定する
					turnTimer_ = kTimeTurn;
				}
			} 
			else if (Input::GetInstance()->PushKey(DIK_LEFT)) {
				// 右移動中の左入力
				if (velocity_.x > 0.0f) {
					velocity_.x *= (1.0f - kAttenuation);
				}
				acceleration.x -= kAcceleration;
				if (lrDirection_ != LRDirection::kRight) {
					lrDirection_ = LRDirection::kLeft;
					turnFirstRotationY_ = worldTransform_.rotation_.y;
					turnTimer_ = kTimeTurn;
				}
			}
				// 加速/減速_
				velocity_ += acceleration;
				// 最大速度制限
				velocity_.x = std::clamp(velocity_.x, -kLimitRunSpeed, kLimitRunSpeed);
			} else {
				// 非入力時
				velocity_ *= (1.0f - kAttenuation);
				// (Input::GetInstance()->PushKey(DIK_LEFT))
				// acceleration.x -= kAcceleration;
			}
			if (Input::GetInstance()->PushKey(DIK_UP)) {
				// ジャンプ初速
				velocity_ += Vector3(0, kJumpAcceleration, 0);
			}
		}
			// 空中
		else
		{
			// 落下速度
			velocity_ += Vector3(0, -kGravityAcceleration, 0);
			// 落下速度制限
			velocity_.y = std::max(velocity_.y, -kLimitFallSpeed);
		}
		// 着地フラグ
		bool landing = false;
		// 地面との当たり判定
		// 下降中？
		if (velocity_.y < 0) 
		{
			// Y座標が地面以下になったら着地
			if (worldTransform_.translation_.y <= 1.0f) {
				landing = true;
			}
		}

		// 接地判定
		if (onGround_) {
			// ジャンプ開始
			if (velocity_.y > 0.0f) {
				onGround_ = false;
			}
		} 
		else 
		{
			// 着地
			if (landing) 
			{
				// めり込み俳斤
				worldTransform_.translation_.y = 1.0f;
				// 摩擦で横方向速度が減衰する
				velocity_.x *= (1.0f - kAttenuation);
				// 下方向速度をリセット
				velocity_.y = 0.0f;
				// 接地状態に移行
				onGround_ = true;
			}
		}
		// 移動
		worldTransform_.translation_ += velocity_;

		// 旋回制御
		if (turnTimer_ > 0.0f) {
			turnTimer_ -= 1.0f / 60.0f;
			// 左右の自キャラ角度テーブル
			float destinationRotationRotationYTable[] = {std::numbers::pi_v<float> / 2.0f, std::numbers::pi_v<float> * 3.0f / 2.0f};
			// 状態に応じた角度を取得する
			float destinationRotationY = destinationRotationRotationYTable[static_cast<uint32_t>(lrDirection_)];

			// 自キャラの角度を設定する
			worldTransform_.rotation_.y = EaseInOut(destinationRotationY, turnFirstRotationY_, turnTimer_ / kTimeTurn);
		}
	    worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	    worldTransform_.TransferMatrix();
	}
	//描画
	void Player::Draw() 
	{
		//3Dモデルを描画
		model_->Draw(worldTransform_, *camera_);
	}
}

	// Drawの関数定義
			void Player::Draw() 
			{
				// 3Dモデル描画

				model_->Draw(worldTransform_, *camera_);
			}
