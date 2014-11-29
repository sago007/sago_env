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

#ifndef SAGOMENUBUTTON_HPP
#define	SAGOMENUBUTTON_HPP

#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include <memory>
#include "SagoCommandQueue.hpp"

namespace sago {
namespace menu {

class SagoMenuButton {
public:
	SagoMenuButton();
    void SetLabel(const std::string &label);
    const std::string& GetLabel() const;
    void SetX(float x);
    float GetX() const;
    void SetY(float y);
    float GetY() const;
    void SetCommand(const std::string &command);
    std::string GetCommand() const;
private:
	std::string label;
	std::string command;
	float x;
	float y;
};

class SagoMenu {
public:
	SagoMenu(const sf::Text &sfText,bool isSubMenu);
	void DrawMenu(sf::RenderWindow &target);
	void addButton(const SagoMenuButton &b);
	void Up();
	void Down();
	void Action(const sago::SagoCommandQueue &queue, std::vector<std::string> &outCommands);
	void Update(float fDeltaTime, const sago::SagoCommandQueue &input);
	bool IsOverAButton() const;
private:
	void placeButtons();
	std::vector<SagoMenuButton> buttons;
	SagoMenuButton exit;
	sf::Text fontText;
	size_t marked = 0;
	bool overAButton = false;
};

class SagoMenuStack {
public:
	void PushMenu(const SagoMenu &menu);
	void PopMenu();
	void DrawMenu(sf::RenderWindow &target);
	void Action(const sago::SagoCommandQueue &queue, std::vector<std::string> &outCommands);
	void Update(float fDeltaTime, const sago::SagoCommandQueue &input);
	bool IsOverAButton() const;
	void Up();
	void Down();
private:
	std::vector<SagoMenu> menus;
};

}
}

#endif	/* SAGOMENUBUTTON_HPP */

