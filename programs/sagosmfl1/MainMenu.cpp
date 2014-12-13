
#include "MainMenu.hpp"
#include "sago/SagoMusicBackground.hpp"

MainMenu::MainMenu(const sago::SagoDataHolder &texHolder) {
	sf::Text menutext;
	menutext.setFont(*texHolder.getFontPtr("FreeSerif"));
	menutext.setCharacterSize(24);
	menutext.setColor(sf::Color::White);
	sago::menu::SagoMenu topmenu(menutext,false);
	sago::menu::SagoMenuButton b;
	b.SetLabel("Start game");
	b.SetCommand("START_GAME");
	topmenu.addButton(b);
	stack.PushMenu(topmenu);
}

bool MainMenu::IsActive() {
	return true;
}

bool MainMenu::IsBlockingDraw() {
	return true;
}

bool MainMenu::IsBlockingUpdate() {
	return true;
}

void MainMenu::Draw(sf::RenderWindow &target) {
	this->stack.DrawMenu(target);
}


void MainMenu::ReadEvents(const sago::SagoCommandQueue &cmdQ) {
	for (size_t i = 0; i < cmdQ.GetCommandQueue().size();i++) {
			std::string cmd = cmdQ.GetCommandQueue().at(i);
		if (cmd == "POP_MENU") {
			stack.PopMenu();
		}
		if (cmd == "UP") {
			stack.Up();
		}
		if (cmd == "DOWN") {
			stack.Down();
		}
		if (cmd == "CONFIRM") {
			stack.Action(cmdQ,outQueue);
		}
	}
}

void MainMenu::Update(float fDeltaTime, const sago::SagoCommandQueue &input) {
	this->stack.Update(fDeltaTime,input);
	ReadEvents(input);
	if (!this->mousePressed && sf::Mouse::isButtonPressed(sf::Mouse::Left) && this->stack.IsOverAButton()) {
		stack.Action(input,outQueue);
	}
	this->mousePressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);
}

void MainMenu::UpdateCommandQueue(sago::SagoCommandQueue& inout) {
	for (std::string item : this->outQueue) {
		inout.PushCommand(item);
	}
	this->outQueue.clear();
}