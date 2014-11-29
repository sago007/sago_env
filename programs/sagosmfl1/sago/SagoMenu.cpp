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

#include "SagoMenu.hpp"


namespace sago {
namespace menu {

SagoMenuButton::SagoMenuButton() {
}

void SagoMenuButton::SetLabel(const std::string &label) {
	this->label = label;
}

const std::string& SagoMenuButton::GetLabel() const {
	return label;
}

void SagoMenuButton::SetX(float x) {
	this->x = x;
}

float SagoMenuButton::GetX() const {
	return x;
}

void SagoMenuButton::SetY(float y) {
	this->y = y;
}

float SagoMenuButton::GetY() const {
	return y;
}

void SagoMenuButton::SetCommand(const std::string &command) {
	this->command = command;
}

std::string SagoMenuButton::GetCommand() const {
	return command;
}

SagoMenu::SagoMenu(const sf::Text &sfText,bool isSubMenu) {
	this->fontText = sfText;
	if (isSubMenu) {
		exit.SetLabel("Back");
		exit.SetCommand("POP_MENU");
	} 
	else {
		exit.SetLabel("Exit");
		exit.SetCommand("QUIT");
	}
	placeButtons();
}

void SagoMenu::DrawMenu(sf::RenderWindow &target) {
	sf::RectangleShape rs(sf::Vector2f(600.0f,60.0f));
	sf::RectangleShape rsm(sf::Vector2f(600.0f,60.0f));
	rs.setFillColor(sf::Color::Blue);
	rsm.setFillColor(sf::Color::Red);
	for ( size_t i = 0; i < buttons.size(); i++ ) {
		SagoMenuButton& item = buttons[i];
		if (marked == i) {
			rsm.setPosition(item.GetX(),item.GetY());
			target.draw(rsm);
			fontText.setPosition(item.GetX()+30,item.GetY()+10);
			fontText.setString(item.GetLabel());
			target.draw(fontText);
		}
		else {
			rs.setPosition(item.GetX(),item.GetY());
			target.draw(rs);
			fontText.setPosition(item.GetX()+30,item.GetY()+10);
			fontText.setString(item.GetLabel());
			target.draw(fontText);
		}
	}
	{
		SagoMenuButton& item = this->exit;
		if (marked == buttons.size()) {
			rsm.setPosition(item.GetX(),item.GetY());
			target.draw(rsm);
			fontText.setPosition(item.GetX()+30,item.GetY()+10);
			fontText.setString(item.GetLabel());
			target.draw(fontText);
		}
		else {
			rs.setPosition(item.GetX(),item.GetY());
			target.draw(rs);
			fontText.setPosition(item.GetX()+30,item.GetY()+10);
			fontText.setString(item.GetLabel());
			target.draw(fontText);
		}
	}
	
}

void SagoMenu::Action(const sago::SagoCommandQueue &q, std::vector<std::string> &output) {
	if (marked < buttons.size()) {
		output.push_back(buttons.at(marked).GetCommand());
	}
	else {
		output.push_back(exit.GetCommand());
	}
}

void SagoMenu::Update(float fDeltaTime, const sago::SagoCommandQueue &input) {
	if (input.MouseMoved()) {
		overAButton = false;
		const sf::Vector2f bSize = sf::Vector2f(600.0f,60.0f);
		const sf::Vector2i &pos = input.GetMousePosition();
		for ( size_t i = 0; i < buttons.size(); i++ ) {
			float x = buttons.at(i).GetX();
			float y = buttons.at(i).GetY();
			if ( pos.x > x && x+bSize.x > pos.x && y < pos.y && y+bSize.y > pos.y ) {
				marked = i;
				overAButton = true;
			}
		}
		{
			float x = exit.GetX();
			float y = exit.GetY();
			if ( pos.x > x && x+bSize.x > pos.x && y < pos.y && y+bSize.y > pos.y ) {
				marked = buttons.size();
				overAButton = true;
			}
		}
	}
}

bool SagoMenu::IsOverAButton() const {
	return overAButton;
}

void SagoMenu::Down() {
	marked++;
	if(marked>buttons.size())
		marked = 0;
}

void SagoMenu::Up() {
	if (marked == 0) {
		marked = buttons.size()+1;
	}
	marked--;
}

void SagoMenu::addButton(const SagoMenuButton& b) {
	buttons.push_back(b);
	placeButtons();
}

void SagoMenu::placeButtons() {

	int nextY = 100;
	const int X = 50;
	std::vector<SagoMenuButton>::iterator it;
	for(it = buttons.begin(); it < buttons.end(); it++)
	{
		(it)->SetX(X);
		(it)->SetY(nextY);
		nextY += 60+10;
	}
	exit.SetX(X);
	exit.SetY(nextY);
}

void SagoMenuStack::PushMenu(const SagoMenu &menu) {
	this->menus.push_back(menu);
}

void SagoMenuStack::PopMenu() {
	if (this->menus.size() > 0) {
		this->menus.pop_back();
	}
}

void SagoMenuStack::DrawMenu(sf::RenderWindow &target) {
	if (this->menus.size() > 0) {
		this->menus.back().DrawMenu(target);
	}
}

void SagoMenuStack::Action(const sago::SagoCommandQueue &queue, std::vector<std::string> &outCommands) {
	if (this->menus.size() > 0) {
		this->menus.back().Action(queue, outCommands);
	}	
}

void SagoMenuStack::Update(float fDeltaTime, const sago::SagoCommandQueue &input) {
	if (this->menus.size() > 0) {
		this->menus.back().Update(fDeltaTime,input);
	}	
}

void SagoMenuStack::Up() {
	if (this->menus.size() > 0) {
		this->menus.back().Up();
	}	
}
	
void SagoMenuStack::Down() {
	if (this->menus.size() > 0) {
		this->menus.back().Down();
	}		
}

bool SagoMenuStack::IsOverAButton() const {
	if (this->menus.size() > 0) {
		return this->menus.back().IsOverAButton();
	}	
	return false;
}

}
}