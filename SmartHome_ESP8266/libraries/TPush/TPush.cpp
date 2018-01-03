#include "TPush.h"

TPush::TPush() { }
TPush::~TPush() { }

void TPush::setUp(int i, bool s) {
	pinMode(i, INPUT);
	BPIN = i;
	ActiveState = s;

	// Utilizzabile SOLO con pulsanti attivi bassi!
	if (!s && AutoPullUp) // +06/07/2015
		digitalWrite(i, HIGH); // Non usare il pin 13 ! (DOC: https://www.arduino.cc/en/Tutorial/DigitalPins)
}

bool TPush::Wait(int ms, int i) {
	if((millis() - iWait[i]) >= ms) {
		iWait[i] = millis();
		return true;
	}
	return false;
}

bool TPush::Click(int ms, int i) {
	if (bClickState[i] == false) {
		if (digitalRead(BPIN) == ActiveState) {

			if((millis() - iClick[i]) >= ms) {
				#if bDebug == true
					Serial.print("Single Click Pressed");
				#endif

				iClick[i] = millis();
				bClickState[i] = true;
				return true;
			}
		} else iClick[i] = millis();
	} else
		if (digitalRead(BPIN) == !ActiveState) {
			#if bDebug == true
				Serial.println("Single Click Released");
			#endif
			bClickState[i] = false;
		}

	return false;
}

bool TPush::DoubleClick(int first, int second, int i, int sep) {
	if (DoubleState[i] > 0 && (millis() - iDouble[i]) >= sep) {
				#if bDebug == true
					Serial.println("DoubleClick TimedOut");
				#endif

				DoubleState[i] = 0;
				iDouble[i] = millis();
	}

	if (DoubleState[i] == 0) { // Prima pressione
		if (digitalRead(BPIN) == ActiveState) {
			if((millis() - iDouble[i]) >= first) {

				#if bDebug == true
					Serial.println("First Click");
				#endif

				DoubleState[i] = 1;
				iDouble[i] = millis();
			}
		}
	} else if (DoubleState[i] == 1) { // Release time
		if (digitalRead(BPIN) == !ActiveState) {
			DoubleState[i] = 2;

			#if bDebug == true
				Serial.println("First Click Released");
			#endif
		}
	} else if (DoubleState[i] == 2) { // Seconda pressione
		if (digitalRead(BPIN) == ActiveState) {
			if((millis() - iDouble[i]) >= second) {

				#if bDebug == true
					Serial.println("Second Click");
				#endif

				iDouble[i] = millis();
				DoubleState[i] = 3;
				return true;
			}
		} else {
			/*if((millis() - iDouble[i]) >= sep) {
			}*/
		}
	} else if (DoubleState[i] == 3) {
		if (digitalRead(BPIN) == !ActiveState) {

			#if bDebug == true
				Serial.println("DoubleClick Done\n");
			#endif

			DoubleState[i] = 0;
		}
	}
	return false;
}


int TPush::Keep(int minTime, bool returnwhileactive) {
	 TPush::Keep(minTime, 0, returnwhileactive);
}

int TPush::Keep(int minTime, int i, bool returnwhileactive) {
	if (digitalRead(BPIN) == ActiveState) {
		if (!bPressed[i] && (millis() - iPressed[i]) >= minTime) {
			PressedTime[i] = millis();


			#if bDebug == true
				Serial.println("KeepHold Pressed");
				Serial.println(returnwhileactive);
			#endif

			iPressed[i] = millis();
			bPressed[i] = true;
		}

		if (((millis() - iPressed[i]) >= minTime) && returnwhileactive)
			return (millis()-PressedTime[i]);

	} else
		iPressed[i] = millis();

	// Fronte di rilascio
	if (bPressed[i] && digitalRead(BPIN) == !ActiveState) {

		#if bDebug == true
			Serial.print("KeepHold Released, time: ");
			Serial.println((millis()-PressedTime[i]));
		#endif

		bPressed[i] = false;
		return (millis() - PressedTime[i]);
	}

	return 0;
}

#if bDebug == true
bool TPush::KeepON(int time, int i) {
	if (digitalRead(BPIN) == ActiveState) {

		if((millis() - iKeepON[i]) >= time) {

			#if bDebug == true
				Serial.println("Keeping On");
			#endif

			return true;
		}
	} else iKeepON[i] = millis();

	return false;
}
#endif