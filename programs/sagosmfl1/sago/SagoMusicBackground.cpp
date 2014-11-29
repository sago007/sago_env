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

#include "SagoMusicBackground.hpp"

namespace {
	std::string currentMusic;
	sf::Music* themusic = nullptr;
	sago::SagoDataHolder *dataHolder;
}

namespace sago {
	namespace music {
		
		void SetDataHolder(SagoDataHolder &texHolder) {
			dataHolder = &texHolder;
		}

		bool isPlaying() {
			if (themusic) {
				return themusic->getStatus() == sf::Music::Playing;
			}
			return false;
		}

		std::string getCurrentMusic() {
			return currentMusic;
		}

		void Play(const std::string& name) {
			if (name != currentMusic) {
				currentMusic = name;
				themusic = dataHolder->getMusicPtr(name);
				themusic->setLoop(true);
				themusic->play();
			}
		}

		void Stop() {
			if(themusic) {
				themusic->stop();
				themusic = nullptr;
				currentMusic = "";
			}
		}
	}
}