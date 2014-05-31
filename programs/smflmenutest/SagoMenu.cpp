/* 
 * File:   SagoMenuButton.cpp
 * Author: poul
 * 
 * Created on 31. maj 2014, 16:58
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

void SagoMenu::Action(sago::SagoCommandQueue &q) {
	if (marked < buttons.size()) {
		q.PushCommand(buttons.at(marked).GetCommand());
	}
	else {
		q.PushCommand(exit.GetCommand());
	}
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

void SagoMenuStack::Action(sago::SagoCommandQueue &queue) {
	if (this->menus.size() > 0) {
		this->menus.back().Action(queue);
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

}
}