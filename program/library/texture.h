#pragma once
#include <string>
#include "shared_factory.h"
#include "shared_flyweight_map.h"

namespace t2k{

	class Texture final : public SharedFactory<Texture>{
	private:
		int image_ = 0;
		int size_w_ = 0;
		int size_h_ = 0;
		std::string filename_;
		inline static SharedFlyWeightMap<std::string, Texture> textures_;

	public:
		typedef std::shared_ptr<Texture> SharedPtr;
		typedef std::weak_ptr<Texture> WeakPtr;
		Texture() {}
		~Texture(){}

		inline int getImage() { return image_; }
		inline int getWidth() { return size_w_; }
		inline int getHeight() { return size_h_; }

		//static Texture* create(const std::string& file_name);

		// テクスチャから範囲を指定して新しいテクスチャを生成する
		Texture::SharedPtr partiallyCreatedFromTexture( const int src_left_up_x, const int src_left_up_y, const int width, const int height );

		// 描画テクスチャの生成
		static Texture::SharedPtr createRenderTexture( const int width, const int height );

		// リソースファイルからテクスチャを生成
		static Texture::SharedPtr createFromFile(const std::string& file_path);

		// 参照されなくなったテクスチャの開放( シーンの切り替え時などで使用 )
		inline static void releaseUnique() { textures_.eraseUnique(); }

		// SharedFlyWeightMap 用のローダー
		// public に存在するがユーザーは直接使用しない( 本来なら inlineヘッダなどで隠蔽対象 )
		static Texture::SharedPtr fromFlyWeightLoader(const std::string& key, const std::string& file_name);
	};

}
