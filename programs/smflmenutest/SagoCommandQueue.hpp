/* 
 * File:   SagoCommandQueue.hpp
 * Author: poul
 *
 * Created on 31. maj 2014, 20:40
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
	void ReadKeysAndAddCommands();
	void PushCommand(const std::string &cmd);
	void ClearCommands();
	void BindKey(const sf::Keyboard::Key &key, const std::string &bindname);
	void BindKeyCommand(const std::string &bindname, const std::string &cmd);
	bool IsPressed(const std::string &bindname) const;
	const std::vector<std::string> &GetCommandQueue() const;
private:
	SagoCommandQueue(const SagoCommandQueue& base) = delete;
    SagoCommandQueue& operator=(const SagoCommandQueue& base) = delete;
	struct SagoCommandQueueData;
	SagoCommandQueueData *data;
};

}

#endif	/* SAGOCOMMANDQUEUE_HPP */

