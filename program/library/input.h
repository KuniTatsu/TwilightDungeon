#pragma once

namespace t2k {

    class Input final {
	public :
		Input(){}
		~Input(){}

		#define MOUSE_OUTPUT_LEFT	(0x08)
		#define MOUSE_OUTPUT_RIGHT	(0x10)
		#define MOUSE_OUTPUT_MIDDLE	(0x20)

		// マウス対応キー 一覧
		enum eMouse {
			MOUSE_PRESSED_LEFT = MOUSE_OUTPUT_LEFT,
			MOUSE_PRESSED_RIGHT = MOUSE_OUTPUT_RIGHT,
			MOUSE_PRESSED_MIDDLE = MOUSE_OUTPUT_MIDDLE,
			MOUSE_RELEASED_LEFT = MOUSE_OUTPUT_LEFT,
			MOUSE_RELEASED_RIGHT = MOUSE_OUTPUT_RIGHT,
			MOUSE_RELEASED_MIDDLE = MOUSE_OUTPUT_MIDDLE
		};

		// キーボード対応キー 一覧
		enum eKeys {
			KEYBORD_BACK		// バックスペース
			, KEYBORD_TAB		// タブ
			, KEYBORD_RETURN	// エンター
			, KEYBORD_LSHIFT	// 左シフト
			, KEYBORD_RSHIFT	// 右シフト
			, KEYBORD_LCONTROL	// 左コントロール
			, KEYBORD_RCONTROL	// 右コントロール
			, KEYBORD_ESCAPE	// エスケープ
			, KEYBORD_SPACE	    // スペース
			, KEYBORD_PGUP		// ＰａｇｅＵＰ
			, KEYBORD_PGDN		// ＰａｇｅＤｏｗｎ
			, KEYBORD_END		// エンド
			, KEYBORD_HOME		// ホーム
			, KEYBORD_LEFT		// 左
			, KEYBORD_UP		// 上
			, KEYBORD_RIGHT	    // 右
			, KEYBORD_DOWN		// 下
			, KEYBORD_INSERT	// インサート
			, KEYBORD_DELETE	// デリート

			, KEYBORD_MINUS		    // －
			, KEYBORD_YEN			// ￥
			, KEYBORD_PREVTRACK	    // ＾
			, KEYBORD_PERIOD		// ．
			, KEYBORD_SLASH		    // ／
			, KEYBORD_LALT			// 左ＡＬＴ
			, KEYBORD_RALT			// 右ＡＬＴ
			, KEYBORD_SCROLL		// ScrollLock
			, KEYBORD_SEMICOLON	    // ；
			, KEYBORD_COLON		    // ：
			, KEYBORD_LBRACKET		// ［
			, KEYBORD_RBRACKET		// ］
			, KEYBORD_AT			// ＠
			, KEYBORD_BACKSLASH	    // ＼
			, KEYBORD_COMMA		    // ，
			, KEYBORD_CAPSLOCK		// CaspLock
			, KEYBORD_PAUSE		    // PauseBreak

			, KEYBORD_NUMPAD0	// テンキー０
			, KEYBORD_NUMPAD1	// テンキー１
			, KEYBORD_NUMPAD2	// テンキー２
			, KEYBORD_NUMPAD3	// テンキー３
			, KEYBORD_NUMPAD4	// テンキー４
			, KEYBORD_NUMPAD5	// テンキー５
			, KEYBORD_NUMPAD6	// テンキー６
			, KEYBORD_NUMPAD7	// テンキー７
			, KEYBORD_NUMPAD8	// テンキー８
			, KEYBORD_NUMPAD9	// テンキー９
			, KEYBORD_MULTIPLY	// テンキー＊
			, KEYBORD_ADD		// テンキー＋
			, KEYBORD_SUBTRACT	// テンキー－
			, KEYBORD_DECIMAL	// テンキー．
			, KEYBORD_DIVIDE	// テンキー／
			, KEYBORD_NUMPADENTER	// テンキーのエンター

			, KEYBORD_F1	// Ｆ１
			, KEYBORD_F2	// Ｆ２
			, KEYBORD_F3	// Ｆ３
			, KEYBORD_F4	// Ｆ４
			, KEYBORD_F5	// Ｆ５
			, KEYBORD_F6	// Ｆ６
			, KEYBORD_F7	// Ｆ７
			, KEYBORD_F8	// Ｆ８
			, KEYBORD_F9	// Ｆ９
			, KEYBORD_F10	// Ｆ１０
			, KEYBORD_F11	// Ｆ１１
			, KEYBORD_F12	// Ｆ１２

			, KEYBORD_A	// Ａ
			, KEYBORD_B	// Ｂ
			, KEYBORD_C	// Ｃ
			, KEYBORD_D	// Ｄ
			, KEYBORD_E	// Ｅ
			, KEYBORD_F	// Ｆ
			, KEYBORD_G	// Ｇ
			, KEYBORD_H	// Ｈ
			, KEYBORD_I	// Ｉ
			, KEYBORD_J	// Ｊ
			, KEYBORD_K	// Ｋ
			, KEYBORD_L	// Ｌ
			, KEYBORD_M	// Ｍ
			, KEYBORD_N	// Ｎ
			, KEYBORD_O	// Ｏ
			, KEYBORD_P	// Ｐ
			, KEYBORD_Q	// Ｑ
			, KEYBORD_R	// Ｒ
			, KEYBORD_S	// Ｓ
			, KEYBORD_T	// Ｔ
			, KEYBORD_U	// Ｕ
			, KEYBORD_V	// Ｖ
			, KEYBORD_W	// Ｗ
			, KEYBORD_X	// Ｘ
			, KEYBORD_Y	// Ｙ
			, KEYBORD_Z	// Ｚ
			, KEYBORD_0	// ０
			, KEYBORD_1	// １
			, KEYBORD_2	// ２
			, KEYBORD_3	// ３
			, KEYBORD_4	// ４
			, KEYBORD_5	// ５
			, KEYBORD_6	// ６
			, KEYBORD_7	// ７
			, KEYBORD_8	// ８
			, KEYBORD_9	// ９
			, KEYBORD_MAX
		};


  	    // 初期化
	    static void initialize();

	    // 更新
		static void update();

	    // 押下
		static bool isKeyDown(eKeys key);

	    // 押下( トリガー )
		static bool isKeyDownTrigger(eKeys key);

	    // リリーストリガー
		static bool isKeyReleaseTrigger(eKeys key);

		// マウス押下
		static bool isMouseTrigger(eMouse _mouse);

	};

}

