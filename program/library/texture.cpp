#include "wrap.h"
#include "util.h"
#include "texture.h"

namespace t2k{


 /*   Texture* Texture::create(const std::string& file_name) {
		Texture* instance = new Texture();
		instance->filename_ = file_name;
		instance->image_ = LoadGraph(file_name.c_str());
		GetGraphSize(instance->image_, &instance->size_w_, &instance->size_h_);
		if (-1 != instance->image_) return instance;
		return nullptr;
	}*/


	Texture::SharedPtr Texture::partiallyCreatedFromTexture(const int src_left_up_x, const int src_left_up_y, const int width, const int height) {
		Texture::SharedPtr ptr = SharedFactory::create<Texture>();
		ptr->image_ = DerivationGraph(src_left_up_x, src_left_up_y, width, height, image_);
		ptr->size_w_ = width;
		ptr->size_h_ = height;
		ptr->filename_ = filename_;
		return ptr;
	}

	Texture::SharedPtr Texture::createRenderTexture(const int width, const int height) {
		Texture::SharedPtr ptr = SharedFactory::create<Texture>();
		SetDrawValidGraphCreateFlag(TRUE);
		ptr->image_ = MakeGraph(width, height);
		SetDrawValidGraphCreateFlag(FALSE);
		ptr->size_w_ = width;
		ptr->size_h_ = height;
		return ptr;
	}

	Texture::SharedPtr Texture::createFromFile(const std::string& file_path) {
		return textures_.load(file_path, file_path);
	}

	Texture::SharedPtr Texture::fromFlyWeightLoader(const std::string& file_name, const std::string& file_path) {
		Texture::SharedPtr ptr = SharedFactory::create<Texture>();
		ptr->filename_ = file_name;
		ptr->image_ = LoadGraph(file_path.c_str());
		GetGraphSize(ptr->image_, &ptr->size_w_, &ptr->size_h_);
		if (-1 != ptr->image_) return ptr;
		warningMassage(( "テクスチャのロードに失敗 : " + file_path ).c_str())
		return nullptr;
	}



}