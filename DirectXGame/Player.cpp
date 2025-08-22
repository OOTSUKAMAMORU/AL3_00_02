#define NOMINMAX
#include "player.h"
#include <algorithm>
#include <numbers>
#include <algorithm>
#define NOMINMAX
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

	CollisionMapInfo collisionMapInfo;
	collisionMapInfo.move = velocity_;
	CheckMapCollision(collisionMapInfo);
	CheckMapMove(collisionMapInfo);
	CheckMapCeiling(collisionMapInfo);
	CheckMapLanding(collisionMapInfo);

	// 移動

	// 旋回制御
	AnimateTurn();

	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	worldTransform_.TransferMatrix();

	
	//(info);
	//(info);
	//(info);
}
// Drawの関数定義
void Player::Draw() {
	// 3Dモデル描画

	model_->Draw(worldTransform_, *camera_);
}
KamataEngine::Vector3 Player::GetWorldPosition() 
{ 
	Vector3 worldPos;
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}

AABB Player::GetAABB() 
{
	Vector3 worldPos = GetWorldPosition();
	AABB aabb;
	aabb.min = {worldPos.x - 0.8f/ 2.0f, worldPos.y - 0.8f/ 2.0f, worldPos.z - 0.8f/ 2.0f};
	aabb.max = {worldPos.x + 0.8f/ 2.0f, worldPos.y + 0.8f/ 2.0f, worldPos.z + 0.8f/ 2.0f};
	return aabb;
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
	CheckMapCollisionDown(info);
	CheckMapCollisionRight(info);
	CheckMapCollisionLeft(info);
}
//マップ衝突チェック　上
void Player::CheckMapCollisionUp(CollisionMapInfo& info) 
{
	// 上昇あり？
	if (info.move.y <= 0) 
	{
		return;
	}
	std::array<Vector3,kNumCorner> positionsNew;
	for (uint32_t i = 0; i < positionsNew.size(); ++i) 
	{
		positionsNew[i] = CornerPosition(worldTransform_.translation_+ info.move, static_cast < Corner>(i));
	}
	// 移動後の４つの角の座標
	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.move, static_cast<Corner>(i));
	}
	MapChipType mapChipType;
	// 真上の当たり判定を行う
	bool hit = false;
	// 左上の判定
	MapChipField::IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType ==MapChipType::kBlock) 
	{
		hit = true;
	}
	//右上点の判定
	//kRightTopについて同様に判定する。
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType==MapChipType::kBlock) 
	{
		hit = true;
	}
	//ブロックにヒット?
	if (hit) 
	{
		//めり込みを排除する方向に移動量を設定する
		indexSet = mapChipField_->GetMapChipIndexSetByPosition	(worldTransform_.translation_ + info.move + Vector3(0, +kHeight / 2.0f, 0));
		//めり込み先ブロックの範囲短形
		MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info.move.y = std::max(0.0f, rect.bottom - worldTransform_.translation_.y - (kHeight/2.0f+kBlank));
		//天井に当たったことを記録する
		info.ceiling = true;
	}
}

// マップ衝突チェック　下
void Player::CheckMapCollisionDown(CollisionMapInfo& info) {
	// 下昇あり？
	if (info.move.y >= 0) {
		return;
	}
	std::array<Vector3, kNumCorner> positionsNew;
	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.move, static_cast<Corner>(i));
	}
	// 移動後の４つの角の座標
	MapChipType mapChipType;
	// 真下の当たり判定を行う
	bool hit = false;
	// 左下の判定
	MapChipField::IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	// 右下点の判定
	// kRightTopについて同様に判定する。
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	// ブロックにヒット?
	if (hit) {
		// めり込みを排除する方向に移動量を設定する
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + info.move + Vector3(0, -kHeight / 2.0f, 0));
		// めり込み先ブロックの範囲短形
		MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info.move.y = std::min(0.0f, rect.top - worldTransform_.translation_.y + (kHeight / 2.0f + kBlank));
		// 床に当たったことを記録する
		info.landing = true;
	}
}

// マップ衝突チェック　右
void Player::CheckMapCollisionRight(CollisionMapInfo& info) {
	// 上昇あり？
	if (info.move.x <= 0) {
		return;
	}
	std::array<Vector3, kNumCorner> positionsNew;
	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.move, static_cast<Corner>(i));
	}
	// 移動後の４つの角の座標
	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.move, static_cast<Corner>(i));
	}
	MapChipType mapChipType;
	// 真上の当たり判定を行う
	bool hit = false;
	// 右上の判定
	MapChipField::IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	// 右下点の判定
	// kRightTopについて同様に判定する。
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	// ブロックにヒット?
	if (hit) {
		// めり込みを排除する方向に移動量を設定する
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + info.move + Vector3(+kWidth / 2.0f, 0,0));
		// めり込み先ブロックの範囲短形
		MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info.move.x = std::max(0.0f, rect.left - worldTransform_.translation_.x - (kWidth / 2.0f + kBlank));
		// 天井に当たったことを記録する
		info.hitWall = true;
	}
}

// マップ衝突チェック　左
void Player::CheckMapCollisionLeft(CollisionMapInfo& info) {
	// 上昇あり？
	if (info.move.x >= 0) 
	{
		return;
	}
	std::array<Vector3, kNumCorner> positionsNew;
	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.move, static_cast<Corner>(i));
	}
	// 移動後の４つの角の座標
	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.move, static_cast<Corner>(i));
	}
	MapChipType mapChipType;
	// 真上の当たり判定を行う
	bool hit = false;
	// 左上の判定
	MapChipField::IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	// 左下点の判定
	// kRightTopについて同様に判定する。
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	// ブロックにヒット?
	if (hit) {
		// めり込みを排除する方向に移動量を設定する
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + info.move + Vector3(-kWidth / 2.0f, 0, 0));
		// めり込み先ブロックの範囲短形
		MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info.move.x = std::min(0.0f, rect.right - worldTransform_.translation_.x + (kWidth / 2.0f + kBlank));
		// 天井に当たったことを記録する
		info.hitWall = true;
	}
}

	//⑤
    void Player::CheckMapWall(const CollisionMapInfo& info) 
	{
		//壁接触による減速
	    if (info.hitWall) 
		{
		    velocity_.x *= (1.0f - kAttenuationWall);
	    }
    }
	//⑥接地状態による減速
    void Player::CheckMapLanding(const CollisionMapInfo& info) {
	    // 自キャラが接地状態？
	    if (onGround_) {
		    // 接地状態の処理
		    // ジャンプ開始
		    if (velocity_.y > 0.0f) {
			    onGround_ = false;
		    } else {
			    std::array<Vector3, kNumCorner> positionsNew;
			    for (uint32_t i = 0; i < positionsNew.size(); ++i) {
				    positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.move, static_cast<Corner>(i));
			    }
			    // 移動後の４つの角の座標
			    MapChipType mapChipType;
			    // 真下の当たり判定を行う
			    bool hit = false;
			    // 左下の判定
			    MapChipField::IndexSet indexSet;
			    indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftBottom]);
			    mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
			    if (mapChipType == MapChipType::kBlock) {
				    hit = true;
			    }
			    // 右下点の判定
			    // kRightTopについて同様に判定する。
			    indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightBottom]);
			    mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
			    if (mapChipType == MapChipType::kBlock) {
				    hit = true;
			    }
			    // 落下なら空中状態に切り替え
			    // 落下開始
			    if (!hit) {
				    // 空中状態に切り替える
				    onGround_ = false;
			    }
		    }
	    } else {
		    // 空中状態の処理
		    // 着地フラグ
		    if (info.landing) 
			{
			    onGround_ = true;
			    velocity_.x *= (1.0f - kAttenuationLanding);
			    velocity_.y = 0.0f;
		    }
	    }
    }
    void Player::OnCollision(const Enemy* enemy) 
	{ 
		(void)enemy;
	    velocity_ += Vector3(0,1,0);
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
		velocity_.y = 0;	
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

KamataEngine::Vector3 Player::CornerPosition(const KamataEngine::Vector3& center, Corner corner) 
{
	Vector3 offsetTable[kNumCorner] = {
	    {+kWidth / 2.0f, -kHeight / 2.0f, 0},
        {-kWidth / 2.0f, -kHeight / 2.0f, 0},
        {+kWidth / 2.0f, +kHeight / 2.0f, 0},
        {-kWidth / 2.0f, +kHeight / 2.0f, 0}
    };
	return center + offsetTable[static_cast<uint32_t>(corner)];
}