e!! LISTE A FAIRE !!


LISTE FAIT : 
- [x] Regarder pour encodeurs OK
- [x] Reprendre régulateur 5V (entrée à gauche et sortie à droite) OK
- [x] VDDA pas alimenté OK
- [x] Calculer valeur R pour les leds d'état OK
- [x] supprimer label hiérarchique 7.2V OK
- [x] Merge les feuilles pour moteur afin de modifier les deux à la fois OK
- [x] faire symbole tof OK?
- [x] remplir valeur composants OK?
- [x] Renommer comme il faut pins de communication accélero OK?
- [x] Remplir le readme avec les schémas + calculs + présentation globale (ne pas avoir à aller chercher les fichiers à gauche à droite)OK
- [x] Mettre texte descriptif pour définir connections
- [x] valeur R 2k3 plutôt que 2.3k  OK
- [x] valeur C 100n plutôt que 0.1µF  OK
- [x] Rendre l'ensemble plus clair (label et autre) OK


Description du PCB :
PCB 54mm x 57mm
4 trou de fixation a 4mm du bord Taille M3 OK
Remplacer l'empreinte de l'interrupteur pour le deporter hors du PCB OK
condo proche OK
reverifier empreinte condo imperial OK
mettre port ldidar a coté de l'alimentation 5V
orientation serio j201 OK
en dernier les gpio pour le routage
router en premier les signaux rapides comme oscillateur, puis connecteur j201
plan de masse unique ideal
plan alim en 3,3V
changer empreinte grosse bobine OK
7.2V refaire connection (pas le meme tag) OK
interrupteur a refaire empeinte plus petite OK
refaire alim en suivant doc conseillé / 3 via OK
Inverser pin driver moteur +zone de connexion plutot que piste OK
rapprocher pastilles de ground et alim des vias associé OK
aligner les composants OK
Rapprocher la partie puissance (driver moteur) OK
regarder plan pour retirer les antennes OK
découplage OK
plan sur grosse bobine OK
agrandir piste gros courant OK
Routage :
Changement règle du CI -> diamètre mini via 0.45
		       -> diamètre mini anneaux 0.075
		       -> voir avec prof pour confirmer modifs (via copié depuis   correction fiack)

TESTER LES CONNECTIONS

MECANIQUE:
designer le parchoc qui porte les capteur de bord 
designer les support ajustable
imprimer les pieces du robot

SOFTWARE:
piloter les driver moteurs
piloter le lidar
recevoir les donnés du lidar et les interpréter
recevoir les donnés de l'accelerometre
recevoir les donnés des capteurs de bords
realiser les comportement en odometrie
coder les comportement de fuite/evitement
coder les comportement de chasse/collision

