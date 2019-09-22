# Guide de développement

## Setup de l'environnement de développement

### Dependances

#### Librairie OneWire

Nous utilisons un capteur de température DS18S20. Il s'agit d'un capteur numérique qui communique via le protocole OneWire. Nous avons donc besoin de dépendre de la librairie OneWire implémentant ce protocole pour Arduino.

La documentation à propos de la librairie OneWire pour Arduino se trouve [ici](http://playground.arduino.cc/Learning/OneWire). La dernière version de la librairie est également téléchargeable sur cette page.


### Code DjoroSensor et DjoroBlock

Afin de pouvoir Editer/Compiler/Installer le code pour DjoroSensor et DjoroBLock
dans leurs cartes respectives Arduino et CC3200/Energia, il faut déplacer les répertoires
contenant le code dans les dossiers correspondant des IDE Arduino ou Energia. Un lien symbolique
est suffisant :

    $ ln -s /path/to/DjoroSensor ~/Documents/Arduino/DjoroSensor

    $ ln -s /path/to/DjoroBlock ~/Documents/Energia/DjoroBlock


### Librairie DjoroCommons

Afin de compiler DjoroSensor ou DjoroBlock, la librairie DjoroCommons doit être
placée dans le dossier `libraries`de Energia ou Arduino. Un lien symbolique suffit.

    $ ln -s /path/to/DjoroCommons ~/Documents/Energia/libraries/DjoroCommons

### Librairies Wifi modifiées

Déplacer ou linker les fichiers WiFi.cpp et WiFi.h du dossier wifipatch dans le répertoire Energia/hardware/cc3200/libraries/WiFi
L'utilisation de WiFi.begin() (sans paramètres) démarre la connectivité Wifi en utilisant les profils renseignés par Ti's SmartConfig.

* En cas de reboot, reset ou reflash par Energia la configuration WiFi n'est pas perdue.
* La connexion est vérifiée toutes les 10 minutes. En cas de problème (DHCP, gateway...) le mode autoconnect est en place et reconnecte la carte.
* /!\ L'utilisation d'un WiFi.begin() avec des paramètres (ssid, pass), dans un autre programme tournant sur la carte, une autre librairie chargée... va casser les profils et le comportement de reconnexion de la carte ; il faut absolument refaire un smart-config avant d'utiliser sereinement les profils wifi embarqués.

### Utilisation de HttpClient

(http://forum.43oh.com/topic/6242-libraries-working-in-energia-13-and-also-in-arduino-but-not-in-energia14/)

### Utilisation de smartconfig dans le code DjoroBlock

Démarrer ou reset la carte en maintenant le bouton PUSH2 lors du boot. La carte passe en access-point smart-config. A ce stade le code reste bloqué dans le setup.
Une fois le réseau configuré via l'application Ti, la carte est connectée et fonctionnelle.


## Bonnes pratiques de développement Arduino/Energia

### Ne pas utiliser l'objet String

La gestion de la mémoire des objets de type String n'est pas forcément très au point.
Il y a un côté pratique à l'usage de String qui fait une allocation dynamique de la mémoire.
En revanche elle n'est pas forcément bien libérée. Le garbage collecting n'est pas forcément efficace ou fait.
Cela vaut pour la cc3200 et pour les cartes Arduino.

**Références : **

* [Very long discussion about Sring and memory allocation](http://forum.arduino.cc/index.php?topic=124367.0)
* [The don't use String testimonial](http://forum.arduino.cc/index.php?topic=185469.0)