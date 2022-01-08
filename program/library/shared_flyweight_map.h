#pragma once
#include <unordered_map>
#include <memory>
#include <stdexcept>

namespace t2k {

	//------------------------------------------------------------------------------------------------------------
	//
	// フライウェイトなマップ 
	// tips... このマップに登録できるのは std::shared_ptr のみ
	//         登録する Class は std::shared_ptr<Class> を戻り値とした
	//         fromFlyWeightLoader を実装することで load を実行すると
	//         フライウェイトな load になります
	//         fromFlyWeightLoader はフライウェイトを意識しないロードメソッドを実装してください 
	//

	template< class Key, class Class >
	class SharedFlyWeightMap {
	private:
		std::unordered_map< Key, std::shared_ptr<Class> > management_map_;
	public:

		//-------------------------------------------------------------------------------
		// 存在の有無
		inline bool isExist(const Key& key) {
			return (management_map_.end() != management_map_.find(key));
		}

		//-------------------------------------------------------------------------------
		// 登録
		inline void regist(const Key & key, std::shared_ptr<Class> data) {
			management_map_.try_emplace(key, data);
		}

		//-------------------------------------------------------------------------------
		// 開放
		inline void erase(const Key & key) {
			std::unordered_map<std::string, std::shared_ptr<Class>>::iterator it = management_map_.find(key);
			if (management_map_.end() == it) return;
			management_map_.erase(it);
		}

		//-------------------------------------------------------------------------------
		// 全開放
		inline void clear() { management_map_.clear(); }

		//-------------------------------------------------------------------------------
		// このマップのみが参照しているデータを開放
		inline void eraseUnique() {
			std::unordered_map<std::string, std::shared_ptr<Class>>::iterator it = management_map_.begin();
			while (it != management_map_.end()) {
				if (1 < it->second.use_count()) {
					it++;
					continue;
				}
				it = management_map_.erase( it );
			}
		}

		//-------------------------------------------------------------------------------
		// 取得
		inline std::shared_ptr<Class> get(const Key & key) {
			std::shared_ptr<Class> ptr;
			try {
				ptr = management_map_.at(key);
			}
			catch (std::out_of_range&) {
				warningMassage(("missing map : " + key).c_str());
			}
			return ptr;
		}


		//-------------------------------------------------------------------------------
		// ロードと登録
		template< class Desc >
		std::shared_ptr<Class> load(const Key & key, const Desc & desc) {
			std::shared_ptr<Class> ptr;
			if (isExist(key)) {
				ptr = get(key);
				return ptr;
			}
			ptr = Class::fromFlyWeightLoader(key, desc);
			regist(key, ptr);
			return ptr;
		}

	};

}
