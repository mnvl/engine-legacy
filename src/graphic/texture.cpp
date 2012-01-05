//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2012 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include <SDL_image.h>
#include <boost/shared_ptr.hpp>
#include <luabind/luabind.hpp>
#include "renderer.h"
#include "texture.h"

namespace graphic
{

Texture::Texture(std::string const &filename):
	filename_(filename),
	width_(-1),
	height_(-1),
	pixel_format_(-1),
	dynamic_(false),
	writeonly_(false) {
}

Texture::Texture(int width, int height, unsigned pixel_format,
	bool dynamic, bool writeonly):
	width_(width),
	height_(height),
	pixel_format_(pixel_format),
	dynamic_(dynamic),
	writeonly_(writeonly)
{
}

Texture::~Texture()
{
}

const void *Texture::lock() const {
	return 0;
}

void Texture::unlock() const {
}

void *Texture::lock(bool discard) {
	return 0;
}

void Texture::unlock() {
}

TexturePtr Texture::load_from_sdl_surface(RendererPtr const &renderer, SDL_Surface *surface) {
	TexturePtr texture = renderer->create_texture(surface->w, surface->h,
		PixelFormat::X8R8G8B8, true, false);

	PixelFormat const &pixel_format = PixelFormat::get(texture->get_pixel_format());

	void *pixels = texture->lock(true);

	SDL_Surface *texture_surface = SDL_CreateRGBSurfaceFrom(pixels, surface->w, surface->h,
		pixel_format.bits, pixel_format.bits * surface->w / 8,
		pixel_format.masks.r, pixel_format.masks.g, pixel_format.masks.b, pixel_format.masks.a);

	int status = SDL_BlitSurface(surface, 0, texture_surface, 0);
	if (status < 0) throw std::runtime_error(SDL_GetError());

	SDL_FreeSurface(texture_surface);

	texture->unlock();

	return texture;
}

TexturePtr Texture::load_from_file(RendererPtr const &renderer, std::string const &filename) {
	SDL_Surface *surface = IMG_Load(filename.c_str());

	TexturePtr texture = load_from_sdl_surface(renderer, surface);

	SDL_FreeSurface(surface);

	return texture;
}

void Texture::bind(lua_State *L)
{
	using namespace luabind;

	module(L, "graphic")
	[
		class_<Texture, TexturePtr >("Texture")
	];
}

}
