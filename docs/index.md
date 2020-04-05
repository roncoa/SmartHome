<table>
<tbody>
<tr class="odd">
<td><img src="./media/image1.jpeg" style="width:1.89583in;height:1.89583in" /></td>
<td><p><em><strong>SmartHome</strong></em></p>
<p><strong>Firmware per ESP8266 basato sul protocollo MQTT</strong></p></td>
</tr>
</tbody>
</table>

***SmartHome*** è un firmware per dispositivi basati sul
microcontrollore ***ESP8266*** con lo scopo di realizzare moduli
domotici a basso costo per rendere "*intelligente*" la casa.

La logica del progetto è divisa in nodi ed ogni nodo ha la propria
logica di funzionamento e gestisce i **GPIO** (ingressi ed uscite) ad
esso collegati.

I nodi sono: nodo <span class="underline">tapparella</span>,
<span class="underline">interruttore</span>,
<span class="underline">impulso</span>,
<span class="underline">temporizzatore,</span>
<span class="underline">sensore</span>,
<span class="underline">power</span>,
<span class="underline">termostato</span> e
<span class="underline">display</span>.

Su ogni singolo ***ESP8266***, si possono caricare più nodi, anche non
dello stesso tipo, basta che abbiano tutti un *topic* diverso (ad
esempio: "*Tapparella/sala*", "*Tapparella/cucina*",
"*Interruttore/sala*", ecc.).

Ogni nodo comunica attraverso il protocollo **MQTT** con il broker, che
può essere locale (LAN) o remoto (internet).

Per interagire con i singoli nodi (contraddistinti da un *topic*
**MQTT** univoco), bisogna inviargli specifici comandi. In questo modo
gli facciamo compiere operazioni specifiche oppure possiamo interrogarlo
per avere informazioni come per esempio lo stato. Il nodo risponderà sul
topic "*TopicNodo/ack*".

Tutti i nodi sono gestibili attraverso l’***<span class="underline">app
Android</span>*** dedicata (scaricabile dal Google Play store) oppure
attraverso HUB personali come per esempio
***<span class="underline">Home Assistant</span>***.

Progetto su GitHub: <https://github.com/roncoa/SmartHome>  
Ultima
release: [https://github.com/roncoa/SmartHome/Relèases/latest](https://github.com/roncoa/SmartHome/releases/latest)  
Wiki: <https://github.com/roncoa/SmartHome/wiki>  
APP
Android: <https://play.google.com/store/apps/details?id=roncoa.SmartHome>  
Canale
YouTube: <https://www.youtube.com/channel/UCH0nHGMIiRXIqrjvXte5OcA>  
Gruppo Facebook: <https://www.facebook.com/groups/SmartHome.roncoa>  
Gruppo Telegram: <https://t.me/joinchat/HDit8BbtVrId4Xh89_gBqw>  
Forum HassioHelp: <https://forum.hassiohelp.eu/t/smarthome-by-at-roncoa>