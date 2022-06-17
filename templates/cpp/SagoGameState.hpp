#include "sago/GameStateInterface.hpp"


#pragma once

class SagoGameState : public sago::GameStateInterface {
public:
	SagoGameState();
	SagoGameState(const SagoGameState& orig) = delete;
	virtual ~SagoGameState();
	
	bool IsActive() override;
	void ProcessInput(const SDL_Event& event, bool &processed) override;
	void Draw(SDL_Renderer* target) override;
	void Update() override;

private:
	bool isActive = true;
};

