#pragma once
#include <KamataEngine.h>
///< summary>
/// カメラコントローラー
///  </summary>
// 前方処理
class Player;

class CameraController 
{
private:
	// カメラ
	KamataEngine::Camera camera_;
	Player* target_ = nullptr;
	KamataEngine::Vector3 targetOffset_ = {0, 0, -15.0f};

public:
	///< summary>
	/// 初期化
	///  </summary>
	void Intialize();
	///< summary>
	/// 更新
	///</summary>
	void Update();
	void SetTarget(Player* target) { target_ = target; }
	void Reset();
	const KamataEngine::Camera& GetViewProjection() const { return camera_; }

	struct Rect 
	{
		float left = 0.0f;
		float right = 1.0f;
		float bottom = 0.0f;
		float top = 1.0f;
	};
	//カメラ移動範囲
	Rect movableArea_ = {0, 100, 0, 100};
	void SetMovableArea(Rect area) { movableArea_ = area; }
	//カメラの目標座標
	KamataEngine::Vector3 targetPosition_;
	static inline const float kInterpolationRate = 0.8f;
	static inline const float kVelocityBias = 0.5f;
	static inline const Rect targetMargin = {-9.0f, 9.0f, -5.0f, 5.0f};
};