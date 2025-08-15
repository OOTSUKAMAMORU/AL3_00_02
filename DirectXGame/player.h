#pragma once
#include "KamataEngine.h"
class Player 
{
public:
	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& position);
	KamataEngine::Vector3 velocity_ = {};
	
	void Update();

	void Draw();

	static inline const float kAcceleration = 0.1f;
	static inline const float kAttenuation = 0.1f;
	static inline const float kLimitRunSpeed =1.0f;
	const KamataEngine::WorldTransform& GetWorldTransform() const { return worldTransform_; }
	const KamataEngine::Vector3& GetVelocity() const { return velocity_;}
	const KamataEngine::Vector3& GetVelocty() const { return velocity_; }

private:
	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;
	KamataEngine::Camera* camera_;
	//左右
	enum class LRDirection 
	{
		kRight,kLeft,
	};

	//旋回時間<秒>
	static inline const float kTimeTurn = 0.3f;
	
	//旋回開始の角度
	float turnFirstRotationY_ = 0.0f;
	//旋回タイマー
	float turnTimer_ = 0.0f;

	LRDirection lrDirection_ = LRDirection::kRight;

	//接地状態フラグ
	bool onGround_ = true;

	//接地状態フラグ
	//BOOL onGrround_ = true;

	//重力加速度(下方向)
	static inline const float kGravityAcceleration = 0.1f;

	//最大落下速度(下方向)
	static inline const float kLimitFallSpeed = 1.0f;

	//ジャンプ初速(上方向)
	static inline const float kJumpAcceleration = 1.0f;
};