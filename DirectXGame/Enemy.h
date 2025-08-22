#pragma once
#include "KamataEngine.h"
class Enemy 
{
public:
	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& position);
	
	void Update();

	void Draw();
	static inline const float kWalkSpeed = 0.1f;
	KamataEngine::Vector3 velocity_ = {};
	float walkTimer_ = 0.0f;

private:
	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::Model* model_ = nullptr;
	KamataEngine::Camera* camera_;
	static inline const float kWalkMotionAngleStart = 0.1f;
	static inline const float kWalkMotionAngleEnd = 0.1f;
	static inline const float kWalkMotionTime = 0.1f;
};
