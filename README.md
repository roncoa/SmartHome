## SmartHome: Casa domotica per ESP8266 basato sul protocollo MQTT
_Firmware per ESP8266_  
Il progetto SmartHome ha lo scopo di realizzare dei moduli domotici a basso costo per rendere "intelligente" la casa.

[**Documentazione**](https://github.com/roncoa/SmartHome/wiki)  
[**Ultima versione**](https://github.com/roncoa/SmartHome/releases/latest)

[![GitHub version](https://img.shields.io/github/release/roncoa/smarthome.svg)](https://github.com/roncoa/smarthome/releases/latest)
[![GitHub download](https://img.shields.io/github/downloads/roncoa/smarthome/total.svg)](https://github.com/roncoa/smarthome/releases/latest)
[![License](https://img.shields.io/github/license/roncoa/smarthome.svg)](https://github.com/roncoa/smarthome/blob/development/LICENSE.txt)

Se ti piace **SmartHome** .......
[![donate](https://img.shields.io/badge/donate-PayPal-blue.svg)](https://paypal.me/roncoa)
[![GitHub stars](https://img.shields.io/github/stars/roncoa/smarthome.svg?style=social&label=Star)](https://github.com/roncoa/smarthome/stargazers)
[![GitHub forks](https://img.shields.io/github/forks/roncoa/smarthome.svg?style=social&label=Fork)](https://github.com/roncoa/smarthome/network)



# SmartHome v5

# SmartHome v4  
Utilizzare l'app dalla versione (16) 0.4β in poi.  
Changelog versione v4:  
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
  
# SmartHome v3  
Utilizzare l'app dalla versione (13) 0.3.3β in poi.  
Changelog versione v3:  
* Miglioramenti per la stabilità.  
* Introdotto aggiornamento OTA da Arduino IDE.  
* Miglioramenti vari.  
* Bug fix.
    


