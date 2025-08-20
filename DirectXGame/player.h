#pragma once
#include "KamataEngine.h"
#include "MapChipField.h"
class Player {
public:
	struct CollisionMapInfo {
		bool ceiling = false;
		bool landing = false;
		bool hitWall = false;
		KamataEngine::Vector3 move;
	};
	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& position);
	KamataEngine::Vector3 velocity_ = {};

	void Update();

	void Draw();

	static inline const float kAcceleration = 0.1f;
	static inline const float kAttenuation = 0.1f;
	static inline const float kLimitRunSpeed = 1.0f;
	const KamataEngine::WorldTransform& GetWorldTransform() const { return worldTransform_; }
	const KamataEngine::Vector3& GetVelocity() const { return velocity_; }
	const KamataEngine::Vector3& GetVelocty() const { return velocity_; }
	void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; }
	static inline const float kWidth = 0.8f;
	static inline const float kHeight = 0.8f;
	static inline const float kAttenuationLanding = 0.1f;

	enum Corner 
	{
		kRightBottom,
		kLeftBottom,
		kRightTop,
		kLeftTop,

		kNumCorner
	};

	private:
		KamataEngine::WorldTransform worldTransform_;
		KamataEngine::Model* model_ = nullptr;
		uint32_t textureHandle_ = 0u;
		KamataEngine::Camera* camera_;
		// 左右
		enum class LRDirection {
			kRight,
			kLeft,
		};

		// 旋回時間<秒>
		static inline const float kTimeTurn = 0.3f;

		// 旋回開始の角度
		float turnFirstRotationY_ = 0.0f;
		// 旋回タイマー
		float turnTimer_ = 0.0f;

		LRDirection lrDirection_ = LRDirection::kRight;

		// 接地状態フラグ
		bool onGround_ = true;

		// 重力加速度(下方向)
		static inline const float kGravityAcceleration = 0.1f;

		// 最大落下速度(下方向)
		static inline const float kLimitFallSpeed = 1.0f;

		// ジャンプ初速(上方向)
		static inline const float kJumpAcceleration = 1.0f;
		MapChipField* mapChipField_ = nullptr;
	    void InputMove();
	    void AnimateTurn();
	    void CheckMapCollision(CollisionMapInfo& info);
	    void CheckMapCollisionUp(CollisionMapInfo& info);
	    void CheckMapCollisionDown(CollisionMapInfo& info);
	    void CheckMapCollisionRight(CollisionMapInfo& info);
	    void CheckMapCollisionLeft(CollisionMapInfo& info);
	    static inline const float kAttenuationWall = 0.2f;

		//指定した角の座標計算
	    KamataEngine::Vector3 CornerPosition(const KamataEngine::Vector3& center, Corner corner);
	    static inline const float kBlank = 0.1f;
	    void CheckMapMove(const CollisionMapInfo& info);
	    void CheckMapCeiling(const CollisionMapInfo& info);
	    void CheckMapWall(const CollisionMapInfo& info);
	    void CheckMapLanding(const CollisionMapInfo& info);
	};