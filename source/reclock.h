/*
 * reclock.h
 *
 *  Created on: Aug 17, 2014
 *      Author: http://sigalrm.blogspot.com/2014/01/overclocking-stm32f4.html?m=1
 */

#ifndef RECLOCK_H_
#define RECLOCK_H_

#include "stm32f4xx_rcc.h"

#ifdef __cplusplus
extern "C" {
#endif

enum sysclk_freq {
    SYSCLK_42_MHZ=0,
    SYSCLK_84_MHZ,
    SYSCLK_168_MHZ,
    SYSCLK_200_MHZ,
    SYSCLK_240_MHZ,
};

void rcc_set_frequency(enum sysclk_freq freq);

#ifdef __cplusplus
}
#endif

#endif /* RECLOCK_H_ */
