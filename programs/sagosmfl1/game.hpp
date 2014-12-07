/* 
 * File:   game.hpp
 * Author: poul
 *
 * Created on 29. november 2014, 14:36
 */

#ifndef GAME_HPP
#define	GAME_HPP

#include "sago/GameState.hpp"
#include "sago/SagoMenu.hpp"
#include "sago/SagoDataHolder.hpp"

class Game : public sago::GameState {
public:
	Game(const sago::SagoDataHolder &dataHolder);
	~Game();
	bool IsActive() override;
	bool IsBlockingDraw() override;
	bool IsBlockingUpdate() override;
	void Draw(sf::RenderWindow &target) override;
	void Update(float fDeltaTime, const sago::SagoCommandQueue &input) override;
	void UpdateCommandQueue(sago::SagoCommandQueue &inout) override;
private:
	void DrawTiles(sf::RenderWindow &target, int topX, int topY, int width, int height);
	struct GameImpl;
	GameImpl* data;
};

#endif	/* GAME_HPP */

