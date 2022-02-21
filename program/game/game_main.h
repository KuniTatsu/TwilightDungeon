///*****Description*****
///ゲームサイクルに関する関数
///gameMain関数が毎フレーム処理され、この関数の内部で各種シーン,シークエンスに別れた処理が走る
///*********************
#pragma once
void gameMain( float deltatime );
static double zoomEx = 1;