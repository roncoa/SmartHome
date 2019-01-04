# SmartHome v5.x  
Changelog coming soon

# SmartHome v4.x  
Utilizzare l'app dalla versione (16) 0.4β in poi.  
* Raggruppato tutti i moduli in un unico file sorgente, basta configurare il file "SmartHome_config.h" .
```
NODI_TAPPARELLA     X                       // Max 2 nodi tapparella (consigliato usarne max 1)
NODI_INTERRUTTORE   X                       // Max 4 nodi interruttore
NODI_CANCELLO       X                       // Max 2 nodi cancello
NODI_TEMPERATURA    X                       // Max 1 nodo temperatura
```
* Aggiunto comando per alzare/abbassare tapparelle in percentuale (es. +50 alza del 50% , -30 abbassa del 30%).
```
+XX            -> comando SU in percentuale (alza la tapparella del XX% es. +50 alza la tapparella del 50%)  
-XX            -> comando GIU in percentuale (abbassa la tapparella del XX% es. -30 abbassa la tapparella del 30%)  
```
* Bug fix.  
  
# SmartHome v3.x  
Utilizzare l'app dalla versione (13) 0.3.3β in poi.  
* Miglioramenti per la stabilità.  
* Introdotto aggiornamento OTA da Arduino IDE.  
* Miglioramenti vari.  
* Bug fix.
