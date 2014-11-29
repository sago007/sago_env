/*
===========================================================================
blockattack - Block Attack - Rise of the Blocks
Copyright (C) 2014 Poul Sander

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see http://www.gnu.org/licenses/

Source information and contacts persons can be found at
http://blockattack.sf.net
===========================================================================
*/

#ifndef GAMESTATEMANAGER_HPP
#define	GAMESTATEMANAGER_HPP

#include "GameState.hpp"
#include "SagoCommandQueue.hpp"
#include <memory>

namespace sago {

class GameStateManager {
public:
	GameStateManager();
	virtual ~GameStateManager();
	/**
	 * Adds a gamestate on top of the old gamestate
     * @param GameState to add
     */
	void PushState(std::shared_ptr<GameState>);
	/**
	 * Checks the last inserted element and removes them until the topmost element is an active one or the stack is empty
     */
	void PopInactive();
	/**
	 * Draws the top of the stack to the render window.
	 * The top most non opaque element is drawn and all transparant elements on top of that
     * @param target Render window to draw to
     */
	void Draw(sf::RenderWindow &target);
	/**
	 * Asks the elements in the stack to update themself. If a stack element blocks updating then all elements below that will not be asked to update themself
	 * This function may only read commands already on the command queue
     * @param fDeltaTime time since last call
     * @param input The input to react to
     */
	void Update(float fDeltaTime, const sago::SagoCommandQueue &input);
	/**
	 * Gives the element in the stack an option to add commands to the command queue
     * @param inout
     */
	void UpdateCommandQueue(sago::SagoCommandQueue &inout);
private:
	GameStateManager(const GameStateManager& base) = delete;
	GameStateManager& operator=(const GameStateManager& base) = delete;
	struct GameStateManagerData;
	GameStateManagerData *data;
};

}

#endif	/* GAMESTATEMANAGER_HPP */

