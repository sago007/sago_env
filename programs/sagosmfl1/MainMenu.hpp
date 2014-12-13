/* 
 * File:   mainmenu.hpp
 * Author: poul
 *
 * Created on 29. november 2014, 13:41
 */

#ifndef MAINMENU_HPP
#define	MAINMENU_HPP

#include "sago/GameState.hpp"
#include "sago/SagoMenu.hpp"
#include "sago/SagoDataHolder.hpp"
#include <vector>

class MainMenu : public sago::GameState {
public:
	MainMenu(const sago::SagoDataHolder &texHolder);
	bool IsActive();
	bool IsBlockingDraw();
	bool IsBlockingUpdate();
	void Draw(sf::RenderWindow &target);
	void Update(float fDeltaTime, const sago::SagoCommandQueue &input);
	void UpdateCommandQueue(sago::SagoCommandQueue &inout);
private:
	void ReadEvents(const sago::SagoCommandQueue &queue);
	sago::menu::SagoMenuStack stack;
	std::vector<std::string> outQueue;
	bool mousePressed = true;
};

#endif	/* MAINMENU_HPP */

