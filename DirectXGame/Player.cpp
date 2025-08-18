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
void Player::Update() {
	InputMove();

	// 着地フラグ
	bool landing = false;
	// 地面との当たり判定
	// 下降中？
	if (velocity_.y < 0) {
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
	} else {
		// 着地
		if (landing) {
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
	AnimateTurn();

	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix();

	CollisionMapInfo CollisionMapInfo;
	CollisionMapInfo collsionMapInfo;
	CollisionMapInfo.move = velocity_;
	CheckMapCollision(collisionMapInfo);
	CheckMapCollisionUp(info);
	//(info);
	//(info);
	//(info);
}
// Drawの関数定義
void Player::Draw() {
	// 3Dモデル描画

	model_->Draw(worldTransform_, *camera_);
}
// 移動入力
void Player::InputMove() 
{
	if (onGround_) {
		// 移動入力

		// 行列更新

		// 移動入力
		// 左右移動動作
		if (Input::GetInstance()->PushKey(DIK_LEFT) || Input::GetInstance()->PushKey(DIK_RIGHT)) {
			// 左右加速
			Vector3 acceleration = {};
			if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
				// 左移動中の右入力
				if (velocity_.x < 0.0f) {
					velocity_.x *= (1.0f - kAttenuation);
				}
				acceleration.x += kAcceleration;

				if (lrDirection_ != LRDirection::kRight) {
					lrDirection_ = LRDirection::kRight;
					// 旋回開始時の角度を記録する
					turnFirstRotationY_ = worldTransform_.translation_.y;
					// 旋回タイマーに時間を設定する
					turnTimer_ = kTimeTurn;
				}
			} else if (Input::GetInstance()->PushKey(DIK_LEFT)) {
				// 右移動中の左入力
				if (velocity_.x > 0.0f) {
					velocity_.x *= (1.0f - kAttenuation);
				}
				acceleration.x -= kAcceleration;
				if (lrDirection_ != LRDirection::kLeft) {
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
	else {
		// 落下速度
		velocity_ += Vector3(0, -kGravityAcceleration, 0);
		// 落下速度制限
		velocity_.y = std::max(velocity_.y, -kLimitFallSpeed);
	}
}
void Player::CheckMapCollision(CollisionMapInfo& info) 
{
	CheckMapCollisionUp(info);
	//CheckMapCollisionDown(info);
	//CheckMapCollisionRight(info);
	//CheckMapCollisionLeft(info);
}
//マップ衝突チェック　上
void Player::CheckMapCollisionUp(CollisionMapInfo& info) 
{
	// 上昇あり？
	if (info.move.y <= 0) 
	{
		return;
	}
	std::array<Vector3, > positionsNew;
	for (uint32_t i = 0; i < positionsNew.size(); ++i) 
	{
		positionsNew[i] = CornerPosition(worldTransform_.+ info., static_cast < Corner(i));
	}
	// 移動後の４つの角の座標
	std::array<Vector3, kNumCorner> positionsNew;
	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.move, static_cast<Corner>(i));
	}
	MapChipType mapChipType;
	// 真上の当たり判定を行う
	bool hit = false;
	// 左上の判定
	MapChipField::IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftTop]);
	mapChipType = mapChipField_->GetMaapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType ==MapChipType::kBlock) 
	{
		hit = true;
	}
	//右上点の判定
	//kRightTopについて同様に判定する。
	indexSet = mapChipField->GetMapChipIndexSetByPosition(positionsNew[kRightTop]);
	mapChipType = mapChipField_->GetMapChipIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType==MapChipType::kBlock) 
	{
		hit = true;
	}
	//ブロックにヒット?
	if (hit) 
	{
		//めり込みを排除する方向に移動量を設定する
		indexSet = mapChipField->GetRectByIndexSetByPosition(worldTransform_.translation_ + info.move + Vector3(0, +kHeight / 2.0f, 0));
		//めり込み先ブロックの範囲短形
		MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info.move.y = std::max(0.0f, rect.bottom - worldtransform_.translation_.y - (kBlank));
		//天井に当たったことを記録する
		info.ceiling = true;
	}
}
//判定結果を反映して移動させる
void Player::CheckMapMove(const CollisionMapInfo& info) 
{
	//移動
	worldTransform_.translation_ += info.move;
}
void Player::CheckMapCeiling(const CollisionMapInfo& info) 
{
	//天井に当たった？
	if (info.ceiling) 
	{
		DebugText::GetInstance()->ConsolePrintf("hitceiling/n");
		velocty_.y = 0;	
	}
}
//旋回制御
void Player::AnimateTurn() 
{
	if (turnTimer_ > 0.0f) {
		turnTimer_ -= 1.0f / 60.0f;
		// 左右の自キャラ角度テーブル
		float destinationRotationRotationYTable[] = {std::numbers::pi_v<float> / 2.0f, std::numbers::pi_v<float> * 3.0f / 2.0f};
		// 状態に応じた角度を取得する
		float destinationRotationY = destinationRotationRotationYTable[static_cast<uint32_t>(lrDirection_)];

		// 自キャラの角度を設定する
		worldTransform_.rotation_.y = EaseInOut(destinationRotationY, turnFirstRotationY_, turnTimer_ / kTimeTurn);
	}
}

KamataEngine::Vector3 Player::CornerPosition(const KamataEngine::Vector3& center, Corner corner) {
if (corner == kRIghtBottom) {
	return center + {+kWidth / 2.0f / -kHeight / 2.0f, 0};
} else if (corner == kLeftBottom) {
	return center + {-kWidth / 2.0f / +kHeight / 2.0f, 0};
} else {
	return center + {+kWidth / 2.0f / -kHeight / 2.0f, 0};
}
else {
	return cerner + {+kWidth / 2.0f / +kHeight / 2.0f, 0};
}
Vector3 CornerPosition(const Vector3& center, Corner corner);
Vector3 offsetTable[kNumCorner] = {{kWidth}};
Vector3 CornerPosition(const vector3& center, Corner corner);
Vector3 offsetTable[kNumCorner] = {
	{+kWidth / 2.0f, -kHeight / 2.0f, 0},
    {-kWidth / 2.0f, -kHeight / 2.0f, 0},
    {+kWidth / 2.0f, -kHeight / 2.0f, 0}
    {-kWidth / 2.0f, -kHeight / 2.0f, 0}
};
return center + offsetTable[static_cast<uint32_t>(corner)];