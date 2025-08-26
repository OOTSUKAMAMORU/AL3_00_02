#pragma once
#include <Windows.h>
#include "KamataEngine.h"
#include "GameScene.h"
#include "TitleScene.h"
using namespace KamataEngine;
GameScene* gameScene = nullptr;
TitleScene* titleScene = nullptr;
//シーン(型)
enum  class Scene 
{
	kUnknown=0,
	kTitle,
	kGame,
};
Scene scene = Scene::kUnknown;

// シーン切り替え処理
void ChangeScene() {
	switch (scene) {
	case Scene::kTitle:
			 if (titleScene->IsFinished())
			 {
				 //シーン変更
				 scene=Scene::kGame;
				 //旧シーンの解放
				 delete titleScene;
				 titleScene=nullptr;
				 //新シーンの生成と初期化
				 gameScene=new GameScene;
				 gameScene->Initialize();
			 }
			 break;
	case Scene::kGame:
			if (gameScene->IsFinished())
			 {
				 //シーン変更
				 scene=Scene::kTitle;
				 //旧シーンの解放
				 delete gameScene;
				 gameScene=nullptr;
				 //新シーンの生成と初期化
			     titleScene = new TitleScene;
				 titleScene->Initialize();
			 }
			 break;
		}
	}
	//シーンの更新
    void UpdateScene() 
	{
	    switch (scene) 
		{
	    case Scene::kTitle:
		    titleScene->Update();
		    break;
	    case Scene::kGame:
		    gameScene->Update();
		    break;
	    }
    }
	//シーンの描画
	void DrawScene()
	{
		switch (scene)
		{
	    case Scene::kTitle:
				titleScene->Draw();
				break;
	    case Scene::kGame:
					gameScene->Draw();
					break;
		}
	}

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	//エンジンの初期化
	KamataEngine::Initialize(L"LE2A_07_オオツカ_マモル_AL3");

	// DirectXCommonインスタンスの取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	// タイトルシーンのインスタンス生成
	titleScene = new TitleScene();
	// タイトルシーンの初期化
	titleScene->Initialize();

	scene = Scene::kTitle;

	//メインループ
	while (true)
	{
		//エンジンの更新
		if (KamataEngine::Update()) 
		{
			break;
		}
		ChangeScene();
		UpdateScene();
		//描画開始
		dxCommon->PreDraw();
		//描画終了
		DrawScene();
		dxCommon->PostDraw();

	}
	delete gameScene;
	delete titleScene;
	//エンジンの終了処理
	KamataEngine::Finalize();
	return 0;
}
