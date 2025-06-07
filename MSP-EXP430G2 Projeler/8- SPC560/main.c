/****************************************************************************
*
* Copyright © 2015-2019 STMicroelectronics - All Rights Reserved
*
* This software is licensed under SLA0098 terms that can be found in the
* DM00779817_1_0.pdf file in the licenses directory of this software product.
* 
* THIS SOFTWARE IS DISTRIBUTED "AS IS," AND ALL WARRANTIES ARE DISCLAIMED, 
* INCLUDING MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*
*****************************************************************************/

/* Inclusion of the main header files of all the imported components in the
   order specified in the application wizard. The file is generated
   automatically.*/
#include "components.h"
#include "can_lld_cfg.h"

/**
 * FLEXCAN Error callback. Interrupt sources managed are: Bus Off, Error, Tx Warning and Rx Warning.
 *
 * canp                 Pointer to the @p CAN Driver object triggering the error
 * esr                  ESR Register Value
 * rx_err_counter       Receive Error Counter
 * tx_err_counter       Transmit Error Counter
 */
void cfg0_errorcb(CANDriver *canp, uint32_t esr,uint8_t rx_err_counter, uint8_t tx_err_counter){
  /* Put error management code Here */
  (void)canp;
  (void)esr;
  (void)rx_err_counter;
  (void)tx_err_counter;
}

/**
 * FIFO notification callback
 *
 * canp      pointer to the @p CAN Driver object triggering the callback
 * crfp      RX frame containg data
 */
void cfg0_Fifo_RX(CANDriver *canp, CANRxFrame crfp) {
  if (crfp.IDE == CAN_IDE_EXT && crfp.EID == 0x70U) {
	  pal_lld_togglepad(PORT_C, PC_LED7);

  }
  (void)canp;
}

/*
 * Application entry point.
 */
int main(void) {

  CANTxFrame txmsg;
  CANRxFrame rxmsg;

	/* Initialization of all the imported components in the order specified in
	 the application wizard. The function is generated automatically.*/
  componentsInit();

  /* Enable Interrupts */
  irqIsrEnable();

  /*
  * Activates the CAN driver 1.
  */
  can_lld_start(&CAND1, &can_config_cfg0);

  /*
  * CAN TX Message structure.
  */
  txmsg.IDE = CAN_IDE_EXT;
  txmsg.RTR = CAN_RTR_DATA;
  txmsg.LENGTH = 8U;
  txmsg.data32[0] = 0x55AA55AAU;
  txmsg.data32[1] = 0x00000000UL;

  /* Application main loop.*/
  for ( ; ; ) {
    txmsg.data32[1]++;

    /* Transmit Can message received by mailbox  */
    txmsg.IDE = CAN_IDE_EXT;
    txmsg.EID = 0x8901234UL;
    while (can_lld_transmit(&CAND1, 1, &txmsg) == CAN_MSG_WAIT) {
    }

    /* Transmit Can message received by FIFO  */
    txmsg.IDE = CAN_IDE_EXT;
    txmsg.EID = 0x70;
    while (can_lld_transmit(&CAND1, 2, &txmsg) == CAN_MSG_WAIT) {
	}

	/*
	* Receives the RX CAN Message.
	*/
    if (can_lld_receive(&CAND1, 1, &rxmsg) == CAN_MSG_OK) {
      pal_lld_togglepad(PORT_C, PC_LED8);
    }

    osalThreadDelayMilliseconds(250UL);
  }
}
