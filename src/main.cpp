/*----------------------------------------------------------------------------
LAB EXERCISE - UART interface
SERIAL COMMUNICATION
 ----------------------------------------
	Print text to the PC via UART protocol
	
	Input: None
	Output: PC

	GOOD LUCK!
 *----------------------------------------------------------------------------*/
#include "mbed.h"
#include "Rtc_Ds1307.h"
#include "Rtc_Ds1307.cpp"
#include "pindef.h"
#include "debug.h"
Serial pc(USBTX,USBRX);
Rtc_Ds1307 rtc(I2C_SDA,I2C_SCL);
DigitalIn mode(PA_10);
DigitalIn action(PB_3);
DigitalIn insidebutton(PB_5);
DigitalIn uh(PC_13);
DigitalOut led(LED1);
AnalogIn potent1(PA_0);
AnalogIn potent2(PA_1);
PwmOut speaker(PB_10);
Timer t;
 
int sel = -1;

void buzzer()
{
	float pinput, period;
	
	while(!uh){
		
		pinput = potent1.read();
				
		period = 1.0/(7680 * pinput + 320);
		for(int j = 0; j < 1; j+=1)
		{
			speaker.period(period);
			speaker = potent2.read();
			wait(.25);
			for(int x = 0; x < 1; x++)
				{
					speaker = 0;
				wait(.25); 
				}
			}
		
		
	}
}
void stopwatch ()
{
	pc.printf("\n\rWelome to timer, press action to start clock");
	wait(1);
	Timer t;
	int j =0;

	while(1){
	while(uh){
	//run this loop when the inside button is pushed
	while(!insidebutton)
	{
		t.start();
		wait(.05);

		while(insidebutton)
		{
			
			pc.printf("\nElapsed Time: %f\n", t.read());
			wait(.001);
			
			//buzzer can be removed if needed doesn't do anything unless "uh" is pushed, even then just plays a buzz noise doesn't affect timing.
			//buzzer();
			if(!mode){
				t.stop();
				pc.printf("Timer stopped\n");
				
				break;
			}
		}
		}
	}
	break;
}
	
	}
	//if(!action)
		//{
		//	break;
		//}

				

int main() 
{
	int count; 
	led =1;
	int holder = 0;
	while(1){
		Rtc_Ds1307::Time_rtc tm = {};
    //tm.sec =0;
		//tm.min = 26;
		//tm.hour = 1;
		//tm.date = 14;
		//tm.mon = 12;
		//tm.year = 2017;
    //tm.wday = 5;
		rtc.getTime(tm);
		//rtc.setTime(tm,true,true);
			if (rtc.getTime(tm) ) {
				while(1){
					wait(0.5);
            pc.printf("The current time is : %02d:%02d:%02d\n\r", tm.hour, tm.min, tm.sec);
						if(!action||!mode)
							break;
								}
			}
		
			wait(1);
		
			holder = 0;
		if(holder == 0) {
			
		pc.printf("\n Select an action \n");
			
			//**********
			//as is the separate buttons just need a function to call and "select an action" will only print once. 
			//the reason being, without a function inside every button push is just a bunch of break;s 
			//**********
			
		while(1){
			holder = 1;
			//if the middle button is pushed the stopwatch is called
			if(!mode){
				stopwatch();
				
				break;
			}
			
			//if the inside button is pushed
			if(!insidebutton){
				
				break;
			}
			
			//if the outside button is pushed
			if(!action){
				
				break;
			}
		
		}
	}
		//else
			//continue;
	
			}
	}

// *******************************ARM University Program Copyright (c) ARM Ltd 2016************************************* 
 
 
 
 
 
 
 
 /*
 
char buffer[128];
//int readptr = 0;
//char  a,b,c,d,e,f;
int main() {
    char c;
    Rtc_Ds1307::Time_rtc tm = {};
    tm.sec =0;
		tm.min = 8;
		tm.hour = 2;
		tm.date = 14;
		tm.mon = 12;
		tm.year = 2017;
    tm.wday = 2;
		//rtc.getTime(tm);
		//rtc.setTime(tm,true,true);
		while(1){
			if (rtc.getTime(tm) ) {
            pc.printf("The current time is : %02d:%02d:%02d\n\r", tm.hour, tm.min, tm.sec);
			}
			wait(1);
		}
}
*/


 


// *******************************ARM University Program Copyright (c) ARM Ltd 2016*************************************
