/*
 * GLOBALS.h
 *
 *  Created on: Jan 2, 2021
 *      Author: Duane Mathias
 */

#ifndef GLOBALS_H_
#define GLOBALS_H_

#include "main.h"
//#include "../absEncoder.h"

// Pin mapping for encoders

GPIO_TypeDef* const ElEncoderPorts[10] = {ElBlack_GPIO_Port, ElRed_GPIO_Port, ElOrange_GPIO_Port, ElYellow_GPIO_Port, ElGreen_GPIO_Port, ElPurple_GPIO_Port, ElGrey_GPIO_Port, ElWhite_GPIO_Port, ElBlackWhite_GPIO_Port, ElRedWhite_GPIO_Port};
const uint16_t ElEncoderPins[10] = {ElBlack_Pin, ElRed_Pin, ElOrange_Pin, ElYellow_Pin, ElGreen_Pin, ElPurple_Pin, ElGrey_Pin, ElWhite_Pin, ElBlackWhite_Pin, ElRedWhite_Pin};

GPIO_TypeDef* const ElStepperPorts[3] = {ElPul_GPIO_Port, ElDir_GPIO_Port,ElEna_GPIO_Port};
const uint16_t ElStepperPins[3] = {ElPul_Pin, ElDir_Pin,ElEna_Pin};

GPIO_TypeDef* const AzEncoderPorts[10] = {AzBlack_GPIO_Port, AzRed_GPIO_Port, AzOrange_GPIO_Port, AzYellow_GPIO_Port, AzGreen_GPIO_Port, AzPurple_GPIO_Port, AzGrey_GPIO_Port, AzWhite_GPIO_Port, AzBlackWhite_GPIO_Port, AzRedWhite_GPIO_Port};
const uint16_t AzEncoderPins[10] = {AzBlack_Pin, AzRed_Pin, AzOrange_Pin, AzYellow_Pin, AzGreen_Pin, AzPurple_Pin, AzGrey_Pin, AzWhite_Pin, AzBlackWhite_Pin, AzRedWhite_Pin};

GPIO_TypeDef* const AzStepperPorts[3] = {AzPul_GPIO_Port, AzDir_GPIO_Port,AzEna_GPIO_Port};
const uint16_t AzStepperPins[3] = {AzPul_Pin, AzDir_Pin,AzEna_Pin};


#endif /* GLOBALS_H_ */
