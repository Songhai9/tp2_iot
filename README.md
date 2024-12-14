# Internet des objets

## TP 2 6LoWPAN

Dans ce TP, vous allez mettre en œuvre une communication entre deux nœuds IoT en utilisant le standard 6LoWPAN.
Vous explorerez son fonctionnement, analyserez ses mécanismes de compression et de fragmentation, et modifierez certains paramètres pour optimiser ses performances.

### Objectifs pédagogiques :
- Configurer une communication IPv6 sur un réseau sans fil IoT contraint ;
- Comprendre les mécanismes de compression et fragmentation de 6LoWPAN ;
- Analyser des traces réseau et calculer des taux de compression ;
- Effectuer des modifications de bas niveau pour optimiser les performances ;

Important : ce TP se réalise en binôme et repose sur l'utilisation des cartes nRF52840 DK.

### Rendu

Vous devrez rendre, sur moodle, une archive (format .tgz) comportant :
- un rapport au format PDF contenant les réponses aux questions posées et les analyses demandées (avec les taux de compression) dans la suite ;
- les traces demandées au format PCAP épurées de tout trafic superflu ;
- vos fichiers `sender-udp.c`, `receiver-udp.c`, `Makefile` et `project-conf.h` ;
- tous les fichiers sources modifiés pour réaliser le dernier exercice.

Il n'y a qu'un rendu par binôme.
La date limite de rendu est fixée au *vendredi 13/12 à 20h*.

## 1. Communication UDP

Vous commencerez par configurer une communication UDP entre deux cartes (la vôtre et celle de votre binôme).
Pour ce faire, vous devrez compléter les programmes `sender-udp.c` et `receiver-udp.c`.
Le programme `sender-udp.c` fera office d'émetteur et ne fera que transmettre des données alors que le programme `receiver-udp.c` fera office de récepteur et ne fera que recevoir des données.

Les messages transmis doivent comporter :
- un numéro de séquence (sur 1 octet)
- la valeur brute du capteur de température
- un message texte

Tous les affichages demandés devront être réalisés par le système de LOG de contiki-ng.
Le niveau devra être configurable via le fichier `project-conf.h`.

La documentation complète de l'API UDP de conitki-ng est disponible [ici](https://docs.contiki-ng.org/en/develop/_api/group__simple-udp.html).
Un [exemple basique](https://docs.contiki-ng.org/en/develop/doc/programming/UDP-communication.html) est également disponible.

Travail à réaliser :
1. Choisissez un canal radio (entre 11 et 26) et un PAN ID différents des autres groupes et modifiez le fichier `project-conf.h` pour les utiliser sur vos cartes.
2. Configurez la pile de communication pour utiliser CSMA au niveau 2, IPv6 au niveau 3 et le protocole de routage NULLROUTING.
3. Choisissez un numéro de port pour l'émetteur et un numéro de port pour le récepteur et complétez les constantes ```PORT_SENDER``` et ```PORT_RECV``` présentes dans les fichiers `sender-udp.c` et `receiver-udp.c` pour les utiliser.
3. Créez une nouvelle connexion UDP via la fonction ```simple_udp_register()` dans les deux programmes avec les paramètres relatifs à vos cartes.
4. Ajoutez le code nécessaire pour que l'émetteur envoie un datagramme UDP vers le récepteur toutes les 5 secondes. Le texte contenu dans les message devra être votre nom de famille. Vous pouvez réutiliser le code du TP1 pour récupérer la valeur du capteur de température. Pour chaque émission, l'émetteur doit afficher l'adresse de destination et le contenu du message transmis.
5. Écrivez la fonction appelée (_callback_) lors de la réception d'un datagramme dans le récepteur. Le récepteur doit afficher l'adresse IP source en plus des informations véhiculées dans les messages. Vous afficherez la valeur réelle du capteur de température (cf. TP1).
6. Testez vos programmes (émetteur sur votre carte et récepteur sur la carte de votre binôme puis inversez). Assurez-vous que l'affichage est correct sur les deux récepteurs.

## 2. Analyse 6LoWPAN et optimisation

Maintenant que la communication est fonctionnelle, vous devez capturer le trafic échangé afin d'analyser la compression réalisée par 6LoWPAN.

Travail à réaliser :
1. Déployez le programme `sensniff` que vous aviez utilisé dans le TP1 sur une 3e carte afin de capturer le trafic échangé entre votre émetteur et votre récepteur.
2. Réalisez une trace de la transmission d'un datagramme UDP entre votre émetteur et le récepteur de votre binôme. Attention, par défaut le FCS (_Frame Check Sequence_) est faux et les entêtes qui suivent l'entête de niveau 2 ne sont pas décodés par les outils classiques (Tshark, Wireshark, etc.). Pour résoudre le problème, vous pouvez :
- passer l'option ```-o "wpan.fcs_format= TI CC24xx metadata"``` à Tshark ;
- aller dans Éditer > Préférences > Protocoles > IEEE 802.15.4 et mettre le format FCS à `TI CC24xx metadata` dans Wireshark.
3. Analysez vos traces et répondez aux questions suivantes :
- quel est le standard de compression utilisé par 6LoWPAN ?
- détaillez comment l'entête IPv6 est compressée et calculez le taux de compression (uniquement au niveau IP) ;
- décrivez comment l'entête UDP est compressée et calculez le taux de compression global (sur l'intégralité du message) ;
4. Modifiez votre configuration pour augmenter le taux de compression. Expliquez et justifiez vos modifications. Réalisez une nouvelle trace (dans chaque sens) pour prouver l'efficacité de vos modifications.

## 3. Fragmentation 6LoWPAN

Dans cet exercice on vous demande d'analyser la fragmentation réalisée par 6LoWPAN.

Augmentez la taille des messages transmis pour dépasser le MTU maximum proposé par 802.15.4.
Que constatez-vous sur au niveau des entêtes 6LoWPAN ?
Réalisez une trace complète de l'envoi d'un datagramme UDP dans ce scénario (une trace par sens de communication).

## 4. Compression checksum UDP

Dans cette étape, vous allez modifier le code source de contiki-ng pour permettre la compression du checksum UDP.

Travail à réaliser :
1. Localisez la partie du code source de contiki-ng responsable du traitement du checksum UDP dans 6LoWPAN.
2. Modifiez l'implémentation actuelle pour permettre de compresser ce champ.
3. Ajoutez le code nécessaire pour rendre la compression de champ configurable via le fichier `project-conf.h`.
4. Testez votre modification et réalisez une trace pour valider vos changements.