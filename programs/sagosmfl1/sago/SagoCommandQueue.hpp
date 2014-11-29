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

#ifndef SAGOCOMMANDQUEUE_HPP
#define	SAGOCOMMANDQUEUE_HPP

#include <string>
#include <vector>
#include <SFML/Graphics.hpp>

namespace sago {

class SagoCommandQueue {
public:
	SagoCommandQueue();
	~SagoCommandQueue();
	/**
	 * Reads the keyboard and mouse an generates the internal events. This should be the first method called in the game loop
     * @param window The window to read the mouse position from
     */
	void ReadKeysAndAddCommands(sf::RenderWindow &window);
	void PushCommand(const std::string &cmd);
	/**
	 * Clears all the internal commands. Should be called the game loop has processed all the event added by ReadKeysAndAddCommands
     */
	void ClearCommands();
	/**
	 * Binds a physical key on the keyboard to a named key
	 * If the physical key was already bound the old bind will be removed
     * @param key Physical key to bind
     * @param bindname Name of the bind
     */
	void BindKey(const sf::Keyboard::Key &key, const std::string &bindname);
	void BindMouseButton(const sf::Mouse::Button &b, const std::string &bindname);
	void BindKeyCommand(const std::string &bindname, const std::string &cmd);
	/**
	 * Returns true if the bindname bound with BindKey was pressed then REadKeysAndAddCommands was called
     * @param bindname Internal name of the key
     * @return true if held down
     */
	bool IsPressed(const std::string &bindname) const;
	/**
	 * Did the mouse move since the last frame?
     * @return true if the mouse did move. false otherwise
     */
	bool MouseMoved() const;
	const std::vector<std::string> &GetCommandQueue() const;
	const sf::Vector2i &GetMousePosition() const;
private:
	SagoCommandQueue(const SagoCommandQueue& base) = delete;
    SagoCommandQueue& operator=(const SagoCommandQueue& base) = delete;
	struct SagoCommandQueueData;
	SagoCommandQueueData *data;
};

}

#endif	/* SAGOCOMMANDQUEUE_HPP */

