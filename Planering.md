# Planering Driverbot
## Mål:
En LEGO-bil som kan köra fram, back och smidigt kan svänga. Den ska ha en funktion, kartläggning, men jag ska komma på något mer. Bilen och den/de extra funktionerna ska visas på en hemsida som ska använda sig av svelte och MQTT.
## Byggande:
### Hållare:
1. Kadda/3D-printa motorhållare
2. Kadda/3D-printa Servohållare
3. Kadda/3D-printa ESP-shield hållare
### Lego:
1. Styrningsmekanism
2. "chassi" med differential
3. Sätt ihop chassit, styrningsmekanismen och alla hållare
## Programering:
### Hemsida:
1. Lära mig svelte
2. Lära mig MQTT
3. Få bilen att köra fram, bak
4. Programera servo så att den kan åka fram/bak samtidigt som servot rör sig
5. Omforma svelte-templaten till en passande hemsida
6. Integrera styrfunktion i hemsidan
## Hur?
### Bil:
En funktion som styr motorns hastighet som jag kallar i en if-statement med w/s som restriktioner, w = vanlig hastighet, s = hastighet * (-1). Funktionen ska även räkna varv på hjulen som jag sedan ska använda för att kartlägga åkturen. 

En funktion som styr servots rotation som jag kallar i en if-statement med a/d som restriktioner. Drar med hjälp av python turtle.
### Hemsida:
Använda svelte för att läsa av knapptrycken
![Skärmbild 2024-04-23 122927](https://github.com/abbindustrigymnasium/driverbot-oliver/assets/143164356/97c30ba9-18a9-44a4-a114-cd4ee01b6812)

![Skärmbild 2024-04-23 124729](https://github.com/abbindustrigymnasium/driverbot-oliver/assets/143164356/399085bf-3e84-430a-b154-f86107a7b338)

