/* 
 * File:   SagoSprite.cpp
 * Author: poul
 * 
 * Created on 29. maj 2014, 14:51
 */

#include "SagoSprite.hpp"

struct SagoSprite::SagoSpriteData {
	const sf::Texture* tex;
	sf::Sprite sprite;
	sf::IntRect imgCord;
	int aniFrames = 0;
	int aniFrameTime = 0;
};

SagoSprite::SagoSprite(const TextureHolder &texHolder, const std::string &texture,const sf::IntRect &initImage,const int animationFrames, const int animationFrameLength) {
	data = new SagoSpriteData();
	data->tex = texHolder.getTexturePtr(texture);
	data->sprite.setTexture(*texHolder.getTexturePtr(texture));
	data->imgCord = initImage;
	data->aniFrames = animationFrames;
	data->aniFrameTime = animationFrameLength;
}



SagoSprite::~SagoSprite() {
	delete data;
}

void SagoSprite::Draw(sf::RenderWindow &target, sf::Int32 frameTime, float x, float y) {
	sf::IntRect rect = data->imgCord;
	rect.left+=rect.width*((frameTime/data->aniFrameTime)%data->aniFrames);
	data->sprite.setTextureRect(rect);
	data->sprite.setPosition(x,y);
	target.draw(data->sprite);
}