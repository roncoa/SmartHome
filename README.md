# SmartHome: Casa domotica per ESP8266 basato sul protocollo MQTT

Il progetto SmartHome ha lo scopo di realizzare dei moduli domotici a basso costo per rendere "intelligente" la casa.  
Si divide in moduli realizzati con il microcontrollore ESP8266: modulo tapparella, modulo temperatura, modulo interruttore e modulo cancello.  
Il modulo "tapparella" serve per comandare tapparelle o serrande.  
Il modulo "temperatura" è un termostato, serve per comandare apparecchiature per il riscaldamento.  
Il modulo "interruttore" serve per comandare luci o prese.  
Il modulo "cancello" serve per comandare cancelli o portoni.  
Ogni modulo comunica attraverso il protocollo MQTT con il broker, che puo' essere locale (LAN) o remoto (internet).  
Per interagire con i singoli moduli bisogna mandare ad esso specifici comandi (contraddistinto da un topic MQTT univoco). Si possono avere piu' moduli dello stesso tipo, basta che abbiano tutti un topic diverso (ad esempio: "tapparella_sala", "tapparella_cucina", "tapparella_bagno", ecc.) .  
Inviando i comandi al modulo (topic), si interagisce con esso, facendogli fare delle operazioni o interrogandolo. Il modulo risponderà sul topic "ack".  

Piccolo video di esempio [QUI](https://www.youtube.com/watch?v=D-fIuXN_vOU)  
A breve sarà disponibile nel Google Play un [applicazione](https://play.google.com/store/apps/details?id=roncoa.SmartHome) Android dalla quale è possibile gestire tutti i vari nodi.  
Se vuoi diventare un beta tester clicca [QUI](https://play.google.com/apps/testing/roncoa.SmartHome)  

### Protocollo MQTT (Message Queuing Telemetry Transport) 
e' un protocollo di comunicazione machine-to-machine (M2M) progettato come un protocollo estremamente leggero che implementa un architettura publish/subscribe.

Nella sua essenza un architettura Publish/Subscribe prevede 4 elementi principali:
* Un agent con il ruolo di Publisher: che invia messaggi al broker indirizzati ad uno specifico Topic
* Un agent con il ruolo di Subscriber: che riceve messaggi dal broker provenienti da uno specifico Topic
* Un Broker di comunicazione: un elemento centrale che orchestra tutti i processi di comunicazione e gestisce il ciclo di vita dei topic e dei messaggi
* Un Topic: un canale di comunicazione dedicato. 

# SmartHome v4  
  
Changelog versione v4:  
* Raggruppato tutti i nodi in un unico file sorgente, basta configurare il file "SmartHome_config.h" .
* Aggiunto comando per alzare/abbassare tapparelle in percentuale (es. +50 alza del 50% , -30 abbassa del 30%).
* Bug fix.  
```
+XX            -> comando SU in percentuale (alza la tapparella del XX% es. +50 alza la tapparella del 50%)  
-XX            -> comando GIU in percentuale (abbassa la tapparella del XX% es. -30 abbassa la tapparella del 30%)  
```    
Utilizzare l'app dalla versione (16) 0.4β in poi.

## SmartHome tapparella V 3.0

* 2 GPIO vengono usati per comandare 2 relè per il movimento della tapparella.  
* 2 GPIO vengono usati come ingressi fisici da pulsanti per comandare direttamente il movimento della tapparella.  

Comandi da inviare al topic "Tapparella_Topic":

    su            -> comando SU  
    giu           -> comando GIU  
    stop          -> comando STOP  
    t=XX o T=XX   -> XX Indica per quanto tempo la tapparella può restare in azione (in sec.)  
    stato         -> restituisce sul topic ACK lo stato dei relè e per quanto tempo la tapparella può restare in azione (in sec.)  
    stato2        -> restituisce sul topic ACK2 lo stato dei relè  
    reset         -> pulisce la memoria EEPROM e resetta l'ESP  

## SmartHome temperatura V 3.0

* 1 GPIO viene usato per la sonda di temperatura e umidità (DHT22).  
* 1 GPIO viene usato per comandare il relè termostato (se impostato in AUTO, funziona come un normale termostato, se impostato in MAN, lo si può commutare a piacere).  
* 1 GPIO viene usato per comandare un relè liberamente gestibile dall'utente.  
* 2 GPIO vengono usati per interfacciare un display I2C (SSD1306).  

Comandi da inviare al topic "Temperatura_Topic":

    man           -> Imposta il termostato in "manuale"
    auto          -> Imposta il termostato in "automatico"
    t=XX o T=XX   -> Imposta il termostato alla temperatura XX
    1on           -> Imposta il relè 1 a ON
    1off          -> Imposta il relè 1 a OFF
    2on           -> Imposta il relè 2 a ON
    2off          -> Imposta il relè 2 a OFF
    stato         -> restituisce sul topic ACK lo stato dei relè la temperatura impostata e lo stato del termostato
    stato2        -> restituisce sul topic ACK2 lo stato dei relè
    read          -> legge la temperatura
    reset         -> pulisce la memoria EEPROM e resetta l'ESP

## SmartHome interruttore V 3.0

* 2 GPIO vengono usati per comandare 2 relè liberamente gestibili dall'utente.  

Comandi da inviare al topic "Interruttore_Topic":

    1on           -> Imposta il relè 1 a ON  
    1off          -> Imposta il relè 1 a OFF  
    2on           -> Imposta il relè 2 a ON  
    2off          -> Imposta il relè 2 a OFF  
    stato         -> restituisce sul topic ACK lo stato dei relè  
    stato2        -> restituisce sul topic ACK2 lo stato dei relè  
    reset         -> pulisce la memoria EEPROM e resetta l'ESP  

## SmartHome cancello V 3.0

* 2 GPIO vengono usati per comandare 2 relè temporizzati liberamente gestibili dall'utente.  

Comandi da inviare al topic "Interruttore_Topic":

    1on           -> Da un impulso ON al relè 1 per un determinato tempo gestibile dall'utente
    2on           -> Da un impulso ON al relè 2 per un determinato tempo gestibile dall'utente
    stato         -> restituisce sul topic ACK lo stato dei relè  
    stato2        -> restituisce sul topic ACK2 lo stato dei relè  
    reset         -> pulisce la memoria EEPROM e resetta l'ESP  
