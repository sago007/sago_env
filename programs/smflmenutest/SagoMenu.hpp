/* 
 * File:   SagoMenuButton.hpp
 * Author: poul
 *
 * Created on 31. maj 2014, 16:58
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
	void Action(sago::SagoCommandQueue &queue);
private:
	void placeButtons();
	std::vector<SagoMenuButton> buttons;
	SagoMenuButton exit;
	sf::Text fontText;
	size_t marked = 0;
};

class SagoMenuStack {
public:
	void PushMenu(const SagoMenu &menu);
	void PopMenu();
	void DrawMenu(sf::RenderWindow &target);
	void Action(sago::SagoCommandQueue &queue);
	void Up();
	void Down();
private:
	std::vector<SagoMenu> menus;
};

}
}

#endif	/* SAGOMENUBUTTON_HPP */

