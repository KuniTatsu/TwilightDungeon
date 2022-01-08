#include <chrono>
#include "DxLib.h"
#include "../library/input.h"
#include "../support/FrameRateController.h"
#include "../game/game_main.h"

static std::chrono::system_clock::time_point clock_start, clock_end ; 

// プログラムは WinMain から始まります
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
						LPSTR lpCmdLine, int nCmdShow )
{

	// ウィンドウモードで起動
	ChangeWindowMode( true ) ;

	// ウィンドウサイズ設定
	SetGraphMode( 1024, 768, 32 ) ;

	// フレームレート設定
	FpsSetting( 60, 800 ) ;

	// ＤＸライブラリ初期化処理
	if( DxLib_Init() == -1 )		
	{
		// エラーが起きたら直ちに終了
		return -1 ;				
	}
	SetDrawScreen(DX_SCREEN_BACK);

	// 計測開始時間
	clock_start = std::chrono::system_clock::now();

	// キー入力制御の初期化
	t2k::Input::initialize();


	// メッセージループ
	while( 1 )
	{
		// フレーム間の経過時間
		// マイクロ秒で計測して秒に変換
		clock_end = std::chrono::system_clock::now();
		double microseconds = static_cast<double>( std::chrono::duration_cast<std::chrono::microseconds>(clock_end - clock_start).count() );
		float deltatime = static_cast<float>( microseconds / 1000.0 / 1000.0 );
		clock_start = clock_end;

		if( ProcessMessage() == -1 ){
			break ;
		}

		// 画面をクリア
		ClearDrawScreen() ;

		// キー状態の更新
		t2k::Input::update();

		// ゲームメインルーチン
		gameMain(deltatime);

		// フレームレートコントロール
		FpsControll() ;

		// バックバッファをフリップ
		ScreenFlip() ;
		//SetWaitVSyncFlag(FALSE);

	}

	// ＤＸライブラリ使用の終了処理
	DxLib_End() ;			

	// ソフトの終了
	return 0 ;				 
}
