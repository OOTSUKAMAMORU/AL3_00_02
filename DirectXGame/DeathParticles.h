#pragma once
#include <array>
#include <numbers>
#include "KamataEngine.h"
class DeathParticles 
{
public:
	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& position);

	void Update();

	void Draw();

	bool IsFinished() const { return isFinished_; }

private:
	KamataEngine::Model* model_ = nullptr;
	KamataEngine::Camera* camera_;
	static inline const uint32_t kNumParticles = 8;

	std::array<KamataEngine::WorldTransform, kNumParticles> worldTransforms_;
	//持続時間(消滅までの時間)<秒>
	static inline const float kDuration = 1.0f;

	//移動の速さ
	static inline const float kSpeed = 0.1f;

	//分割した１個分の角度
	static inline const float kAngleUnit = 2.0f * std::numbers::pi_v<float> / kNumParticles;
	bool isFinished_ = false;
	float counter_ = 0.0f;
	//色変更
	KamataEngine::ObjectColor objectColor_;
	KamataEngine::Vector4 color_;
};
