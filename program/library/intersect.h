#pragma once
#include "matrix.h"

namespace t2k {

	//-----------------------------------------------------------------------------------------------------
	// 点と矩形(正方形)のの衝突検知
	// arg1...点のｘ座標
	// arg2...点のｙ座標
	// arg3...矩形の中心ｘ座標
	// arg4...矩形の中心ｙ座標
	// arg5...矩形のサイズ
	// ret....[ 衝突している : true ]   [ 衝突していない : false ]
	bool isIntersectPointRect(const int point_x, const int point_y, const int rect_x, const int rect_y, const int rect_size);

	//-----------------------------------------------------------------------------------------------------
	// 矩形と矩形の衝突検知 ※矩形の左上・右下の頂点座標指定
	// arg1...矩形Aの左のｘ座標
	// arg2...矩形Aの右のｘ座標
	// arg3...矩形Aの上のｙ座標
	// arg4...矩形Aの下のｙ座標
	// arg5...矩形Bの左のｘ座標
	// arg6...矩形Bの右のｘ座標
	// arg7...矩形Bの上のｙ座標
	// arg8...矩形Bの下のｙ座標
	// ret....[ 衝突している : true ]   [ 衝突していない : false ]
	bool isIntersectRectPrimitive(const int a_left, const int a_right, const int a_top, const int a_bottom,
		const int b_left, const int b_right, const int b_top, const int b_bottom);

	//-----------------------------------------------------------------------------------------------------
	// 矩形と矩形の衝突検知　※矩形の中心と幅高さ指定
	// arg1...矩形Aのｘ座標
	// arg2...矩形Aのｙ座標
	// arg3...矩形Aの幅
	// arg4...矩形Aの高さ
	// arg5...矩形Bのｘ座標
	// arg6...矩形Bのｙ座標
	// arg7...矩形Bの幅
	// arg8...矩形Bの高さ
	// ret....[ 衝突している : true ]   [ 衝突していない : false ]
	bool isIntersectRect(const int a_rect_x, const int a_rect_y, const int a_rect_size_w, const int a_rect_size_h,
		const int b_rect_x, const int b_rect_y, const int b_rect_size_w, const int b_rect_size_h);

	//-----------------------------------------------------------------------------------------------------
	// 矩形と矩形の衝突検知　※矩形の中心と幅高さ指定
	// arg1...矩形Aの座標
	// arg2...矩形Aの幅
	// arg3...矩形Aの高さ
	// arg4...矩形Bの座標
	// arg5...矩形Bの幅
	// arg6...矩形Bの高さ
	// ret....[ 衝突している : true ]   [ 衝突していない : false ]
	inline bool isIntersectRect(const t2k::Vector3& a, const int a_rect_size_w, const int a_rect_size_h,
		const t2k::Vector3& b, const int b_rect_size_w, const int b_rect_size_h) {
		return isIntersectRect(a.x, a.y, a_rect_size_w, a_rect_size_h, b.x, b.y, b_rect_size_w, b_rect_size_h);
	}
 
	//-----------------------------------------------------------------------------------------------------
	// 矩形と矩形の衝突検知 & 座標補正
	// arg1...矩形Aの現在座標( 補正対象の座標変数 )
	// arg2...矩形Aの移動前の座標
	// arg3...矩形Aの幅
	// arg4...矩形Aの高さ
	// arg5...矩形Bの座標
	// arg6...矩形Bの幅
	// arg7...矩形Bの高さ
	// arg8...補正の追加スペース( 省略可 デフォルト 1.0f )
	// ret....[ 衝突していない : 0 ] 
	//        [ 対象を左に補正 : 1 ] 
	//        [ 対象を右に補正 : 2 ]
	//        [ 対象を上に補正 : 3 ]
	//        [ 対象を下に補正 : 4 ]
	// tips...矩形Aが補正対象のオブジェクト
	//        矩形Bは壁等の移動しないオブジェクトを想定しています
	int isIntersectRectToCorrectPosition( t2k::Vector3& a_now, const t2k::Vector3 &a_prev, const int a_rect_size_w, const int a_rect_size_h,
		const t2k::Vector3& b, const int b_rect_size_w, const int b_rect_size_h, const float correct_space = 1.0f);


	//----------------------------------------------------------------------------------------------
	// name... isIntersectLine2D
	// work... 2D上の線分の判定
	// arg1... 線分Aの始点
	// arg2... 線分Aの終点
	// arg3... 線分Bの始点
	// arg4... 線分Bの終点
	// ret.... [衝突している : true] [衝突していない : false]
	// tips... none
	bool isIntersectLine2D(const Vector3& p1, const Vector3& p2, const Vector3& p3, const Vector3& p4);

	//----------------------------------------------------------------------------------------------
	// name... isIntersectLineTriangle2D
	// work... 三角形と線分の衝突判定
	// arg1-3. 三角形を構成する頂点
	// arg5-5. 始点と終点 
	// ret.... [衝突している : true] [衝突していない : false]
	// tips... 2D 用
	bool isIntersectLineTriangle2D(const Vector3& _v1, const Vector3& _v2, const Vector3& _v3, const Vector3& _s, const Vector3& _e);


	//-----------------------------------------------------------------------------------------------------
	// 球と球の衝突検知
	// arg1...円Aの中心座標
	// arg2...円Aの半径
	// arg3...円Bの中心座標
	// arg4...円Bの半径
	// ret....[ 衝突している : true ]   [ 衝突していない : false ]
	bool isIntersectSphere(const t2k::Vector3& a, float a_radius, const t2k::Vector3& b, float b_radius);

	//----------------------------------------------------------------------------------------------
	// name... isIntersectLinePlane
	// work... 線分と平面の衝突判定
	// arg1... 線分の始点
	// arg2... 線分の終点
	// arg3... 平面上の座標 ( 0, 0, 0 以外 )
	// arg4... 平面の法線
	// arg5... 衝突点の受け取り用( 必要なければ省略可 )
	// ret.... [衝突している : true] [衝突していない : false]
	// tips... 衝突していない場合 arg5 で渡した引数は変化なし
	bool isIntersectLinePlane(const t2k::Vector3 &line_start, const t2k::Vector3 &line_end,
		const t2k::Vector3 &plane_pos, const t2k::Vector3 &plane_normal,
		t2k::Vector3 &intersect_position = t2k::Vector3(0, 0, 0));

	//----------------------------------------------------------------------------------------------
	// name... isIntersectTrianglePoint
	// work... 三角形と点の判定
	// arg1-3. 三角形の頂点座標
	// ret.... [衝突している : true] [衝突していない : false]
	// tips... none
	bool isIntersectTrianglePoint(const Vector3& v1, const Vector3& v2, const Vector3& v3, const Vector3& p);

	//----------------------------------------------------------------------------------------------
	// name... isIntersectLineTriangle
	// work... 三角形と線分の衝突判定
	// arg1-3. 三角形を構成する頂点
	// arg2-4. 始点と終点 
	// ret.... [衝突している : true] [衝突していない : false]
	// tips... 3D 用
	bool isIntersectLineTriangle(const Vector3& v1, const Vector3& v2, const Vector3& v3, const Vector3& s, const Vector3& e);

	//----------------------------------------------------------------------------------------------
	// name... isIntersectRayOBB
	// work... レイとOBB の衝突判定
	// arg1... レイ上の座標 ( 線分なら始点 )
	// arg2... レイの方向ベクトル
	// arg3... ボックスをAABB とした時の 左奥上の座標
	// arg4... ボックスをAABB とした時の 右前下の座標
	// arg5... OBB の回転行列
	// arg7... 演算結果の交点( 省略可 )
	// ret.... [衝突している : true] [衝突していない : false]
	bool isIntersectRayOBB( const Vector3& pos, const Vector3& dir, const Vector3& aabb_max, const Vector3& aabb_min, const Matrix& obb_rot, Vector3& intersect_pos = t2k::Vector3(0, 0, 0));

	//----------------------------------------------------------------------------------------------
	// name... isIntersectLineOBB
	// work... 線分とOBB の衝突判定
	bool isIntersectLineOBB(const Vector3& s, const Vector3& e, const Vector3& aabb_max, const Vector3& aabb_min, const Matrix& obb_rot, Vector3& intersect_pos = t2k::Vector3(0, 0, 0));

}
