/*---------------------------------------------------------------------------------

	default ARM7 core

		Copyright (C) 2005 - 2010
		Michael Noland (joat)
		Jason Rogers (dovoto)
		Dave Murphy (WinterMute)

	This software is provided 'as-is', without any express or implied
	warranty.  In no event will the authors be held liable for any
	damages arising from the use of this software.

	Permission is granted to anyone to use this software for any
	purpose, including commercial applications, and to alter it and
	redistribute it freely, subject to the following restrictions:

	1.	The origin of this software must not be misrepresented; you
		must not claim that you wrote the original software. If you use
		this software in a product, an acknowledgment in the product
		documentation would be appreciated but is not required.

	2.	Altered source versions must be plainly marked as such, and
		must not be misrepresented as being the original software.

	3.	This notice may not be removed or altered from any source
		distribution.

---------------------------------------------------------------------------------*/
#include <nds.h>
#include "../../share/fifotool.h"

unsigned int * SCFG_ROM=	(unsigned int*)0x4004000;
unsigned int * SCFG_CLK=	(unsigned int*)0x4004004; 
unsigned int * SCFG_EXT=	(unsigned int*)0x4004008;

//---------------------------------------------------------------------------------
void VblankHandler(void) {
//---------------------------------------------------------------------------------
	//Wifi_Update();
}


//---------------------------------------------------------------------------------
void VcountHandler() {
//---------------------------------------------------------------------------------
	inputGetAndSend();
}

volatile bool exitflag = false;

//---------------------------------------------------------------------------------
void powerButtonCB() {
//---------------------------------------------------------------------------------
	exitflag = true;
}

static void myFIFOValue32Handler(u32 value,void* data)
{
  nocashMessage("myFIFOValue32Handler");

  switch(value)
  {
    case MSG_SCFG_ROM:
	  nocashMessage("MSG_SCFG_ROM");
	  swiDelay(100);
	  nocashMessage("fifoSendValue32");
      fifoSendValue32(FIFO_USER_02,*SCFG_ROM);	 
      break;
    case MSG_SCFG_CLK:
	  nocashMessage("MSG_SCFG_CLK");
	  swiDelay(100);
	  nocashMessage("fifoSendValue32");
	  fifoSendValue32(FIFO_USER_02,*SCFG_CLK);	  
      break;
    case MSG_SCFG_EXT:
	  nocashMessage("MSG_SCFG_EXT");
	  swiDelay(100);
	  nocashMessage("fifoSendValue32");
	  fifoSendValue32(FIFO_USER_02,*SCFG_EXT);
      break;
    default:
      nocashMessage("default");
	  swiDelay(100);
	  nocashMessage("fifoSendValue32");
	  fifoSendValue32(FIFO_USER_02,value);
      break;
  }
}

//---------------------------------------------------------------------------------
int main() {
//---------------------------------------------------------------------------------
    nocashMessage("ARM7 main.c main");
	// clear sound registers
	dmaFillWords(0, (void*)0x04000400, 0x100);

	REG_SOUNDCNT |= SOUND_ENABLE;
	writePowerManagement(PM_CONTROL_REG, ( readPowerManagement(PM_CONTROL_REG) & ~PM_SOUND_MUTE ) | PM_SOUND_AMP );
	powerOn(POWER_SOUND);

	readUserSettings();
	ledBlink(0);

	irqInit();
	// Start the RTC tracking IRQ
	initClockIRQ();
	fifoInit();

	//mmInstall(FIFO_MAXMOD);

	SetYtrigger(80);

	//installWifiFIFO();
	installSoundFIFO();

    nocashMessage("ARM7 main.c installSystemFIFO()");
	installSystemFIFO();

	irqSet(IRQ_VCOUNT, VcountHandler);
	irqSet(IRQ_VBLANK, VblankHandler);

	irqEnable( IRQ_VBLANK | IRQ_VCOUNT | IRQ_NETWORK);

	setPowerButtonCB(powerButtonCB);	
	

	
	//unsigned int * SCFG_ROM=			(unsigned int*)0x4004000;
	//unsigned int * SCFG_EXT=			(unsigned int*)0x4004008;
	//unsigned int * SCFG_ROM_ARM7_COPY=	(unsigned int*)0x2370000;
	//unsigned int * SCFG_EXT_ARM7_COPY=  (unsigned int*)0x2370008;
	//*SCFG_ROM_ARM7_COPY = *SCFG_ROM;
	//*SCFG_EXT_ARM7_COPY = *SCFG_EXT;
	
	fifoSetValue32Handler(FIFO_USER_01,myFIFOValue32Handler,0);
	
	//fifoSendValue32(FIFO_USER_01,MSG_SCFG_READY);
	

	// Keep the ARM7 mostly idle
	while (1) {
		swiWaitForVBlank();
	}
	return 0;
}
