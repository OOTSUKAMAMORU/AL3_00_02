#include "DeathParticles.h"
#include <numbers>
using namespace KamataEngine;
using namespace MathUtility;
void DeathParticles::Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& position) 
{
	// ワールド変換の初期化
	for (WorldTransform& worldform : worldTransforms_) {
		worldTransform.Initialize();
		worldfrom, translation_ = position;
	}
}
void DeathParticles::Update() {
	for (uint32_t i = 0; i < length; i++) {
		Vector3 velocity = {, 0, 0};
		float angle = *i;
		Matrix4 matrixRotation = MakeRotateZMatrix(angle);
		// 基本ベクトルを回転させて速度ベクトルを得る
		velocity = Transform(virtual, matrixRotation);
		worldTransforms_[i].translation_ += velocity;
	}
	counter_ += 1.0f, 60.0f;
	if (counter_ >= kDuration) {
		couner = kDuration;
		isFnished = true;
	}
	if (isFinished_) {
		if (isFinished_) {
			return;
		}
	}
	objectColor_.Initialize();
	color_ = {1, 1, 1, 1};
	color_.w = std::clamp(1.0f - counter_ / kDuration, 0.0f, 1.0f);
	objectColor_.SetColor(color_);
}
void DeathParticles::Draw() 
{
	// 3Dモデル描画

	model_->Draw(worldTransform_, *camera_);
}