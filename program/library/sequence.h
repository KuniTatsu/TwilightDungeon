#pragma once
#include <functional>

namespace t2k {

	template <class T>
	class Sequence final {
	private:
		T object_;
		std::function<bool(T, const float)> now_;
		std::function<bool(T, const float)> next_;
		bool is_start_ = true;
		bool is_change_ = false;
		float sum_time_ = 0;
		Sequence() {}
	public:

		//===================================================================================
		// コンストラクタ
		// arg1... 包含クラスの this ポインタを指定
		// arg2... コールバックで呼び出したい包含クラスのメンバメソッドまたはラムダ式指定
		//===================================================================================
		Sequence(const T obj, const std::function<bool(T, const float)> func)
			: object_(obj)
			, next_(func)
			, now_(func)
		{}
		~Sequence() {}

		//===================================================================================
		// シーケンスのアップデート ( 毎フレーム呼び出せばOK )
		// arg1... フレーム間の経過時間( 秒のデルタタイム )
		//===================================================================================
		inline bool update(const float deltatime) {
			sum_time_ += deltatime ;
			bool ret = now_(object_, deltatime);
			if (!is_change_) {
				is_start_ = false;
				return ret ;
			}
			now_ = next_;
			is_start_ = true;
			sum_time_ = 0;
			is_change_ = false;
			return ret;
		}

		//===================================================================================
		// 初期化用　シーケンスの最初の１フレームだけ true が帰る
		//===================================================================================
		inline bool isStart() const { return is_start_; }

		//===================================================================================
		// シーケンスの経過時間を取得 ( 秒 )
		//===================================================================================
		inline float getProgressTime() const { return sum_time_; }

		//===================================================================================
		// シーケンスの変更
		// arg1... 次のフレームから実行させる包含クラスのメソッドを指定
		//===================================================================================
		inline void change(const std::function<bool(T, const float)> func) {
			next_ = func; 
			is_change_ = true;
		}

		//===================================================================================
		// シーケンスを即座に変更
		// arg1... 実行させる包含クラスのメソッドを指定
		// tisp... 次フレームを待たず即座にシーケンスを変更する
		//===================================================================================
		inline void immediatelyChange(const std::function<bool(T, const float)> func) {
			now_ = func;
			is_start_ = true;
			sum_time_ = 0;
			is_change_ = false;
		}

	};

}

