/* 
 * File:   GameModelClient.hpp
 * Author: poul
 *
 * Created on 30. maj 2014, 20:12
 */

#ifndef GAMEMODELCLIENT_HPP
#define	GAMEMODELCLIENT_HPP

class GameModelClient {
public:
	GameModelClient();
	virtual ~GameModelClient();
private:
	GameModelClient(const GameModelClient& base) = delete;
    GameModelClient& operator=(const GameModelClient& base) = delete;
	struct GameModelClientData;
	GameModelClientData *data;
};

#endif	/* GAMEMODELCLIENT_HPP */

