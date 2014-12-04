/* 
 * File:   CommandProcessor.hpp
 * Author: poul
 *
 * Created on 30. november 2014, 13:00
 */

#include "sago/SagoCommandQueue.hpp"
#include "sago/SagoDataHolder.hpp"
#include "sago/GameStateManager.hpp"

#ifndef COMMANDPROCESSOR_HPP
#define	COMMANDPROCESSOR_HPP

void ProcessCommands(sago::SagoCommandQueue &cmdQ, sago::SagoDataHolder &dh, sago::GameStateManager &m);

#endif	/* COMMANDPROCESSOR_HPP */

