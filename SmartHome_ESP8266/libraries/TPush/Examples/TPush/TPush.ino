#include <LiquidCrystal.h>
#include <TPush.h>

LiquidCrystal lcd(2, 3, 4, 5, 6, 7);

TPush Btn1, LCD;

boolean Hold = false;
boolean DoubleTap = false;
int t = 0;

void setup() {
	// Setto la variabile del pulsante e la inizializzo.
        // Il pin digitale viene automaticamente impostato come OUTPUT, SE attivo basso viene abilitata la resistenza di pullup interna al micro
	// Btn1 è posizionato sul pin 2 ed è attivo basso
	Btn1.setUp(8, LOW);

	// Inizializzazione display
	lcd.begin(16, 2);

	Serial.begin(9600);
}

void loop() {
	// La condizione LCD.Wait(150) restituisce TRUE ogni 150ms
	if (LCD.Wait(150)) {
		lcd.clear();
		// Visualizzo lo stato del doppioclick
		lcd.print("DoubleClick: ");
		if (DoubleTap)
			lcd.print("A");
		else
			lcd.print("B");
		
		// Posiziono il cursore nella seconda riga
		lcd.setCursor(0,1);

		// Visualizzo lo stato della pressione
		lcd.print("Keep: ");

		if (Hold)
			lcd.print("ATTIVO");
		else
			lcd.print(t);
	}
     
        // Dopo 200ms di pressione, 't' viene impostata ad ogni ciclo
        // se t supera i 2 secondi di pressione
	//if ((t = Btn1.Keep(200, true)) && t > 2000)
	//	Hold = !Hold;

	if(Btn1.Keep(2000))
		Hold = !Hold;


	// Doppio click sul pulsante
	if(Btn1.DoubleClick())
		DoubleTap = !DoubleTap;
}
