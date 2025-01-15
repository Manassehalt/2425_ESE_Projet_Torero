# Software

Le dossier contenant le code à implémenter sur la carte est situé au lien suivant :
https://github.com/Manassehalt/2425_ESE_Projet_Torero/tree/main/Projet_ESE_STM32/Projet_ESE_Torero/STM32G431CBU6

## Driver Moteurs motor.c
Démarre la génération des PWM sur les timers TIM1 et TIM2

Fonction Motor_SetSpeed_L pour contrôler le moteur gauche

Fonction Motor_SetSpeed_R pour contrôler le moteur droit

## Driver ADXL ADXL.c
ADXL_Init permet de configurer l'accéléromètre pour que ce dernier génère des interruptions lors d'un choc.

Source d'erreur : après une interruption générée par l'ADXL, il faut lire le registre INT_SOURCE pour générer une nouvelle interruption

## Capteur de bord
Les capteurs de bord génèrent directement des interruptions lorsqu'ils ne voient plus la table et 
lorsqu'ils la voient de nouveau permettant ainsi de régler la marche arrière du robot pour se dégager du bord.

## Driver Lidar lidarX4.c
Le driver n'est pas pleinement fonctionnel, mais son objectif est de gérer l'initialisation, la transmission des commandes et le traitement des données pour extraire les informations de distance et d'angle. Les données seraient filtrées pour réduire le bruit, et des commandes spécifiques permettraient de démarrer, arrêter ou vérifier l'état du Lidar. L'intégration repose sur l'utilisation de l'interface UART avec DMA en mode circulaire pour un traitement en arrière-plan.

## Implémentation FreeRTOS

### TaskLIDAR de priorité 0
Non implémentée car le driver lidar n'est pas fonctionnel. S'il l'avait été, la tâche aurait une priorité 0 pour traiter les données reçues par le
lidar en arrière tâche car ce traitement est moins important que le reste. On aurait ajouter un vTaskDelay() pendant un nb de tick important dans TaskMOTOR pour laisser 
le temps à cette tâche de s'exécuter et ainsi mettre à jour les commandes moteurs alpha1 et alpha2 pour l'asservissement et le suivi de cible. TaskMOTOR 
aurait seulement été active si le lidar ne toruvait pas de cible pour avancer tout droit.

### TaskMOTOR de priorité 1
Cette tâche actualise la vitesse des moteurs à l'aide des variable alpha1 pour le moteur droit et alpha2 pour le moteur gauche pour atteindre 
la vitesse maximale en ligne droite.
Les fonctions utilisées Motor_SetSpeed_L et Motor_SetSpeed_R font converger le rapport cyclique des PWM par pas de 1 à chaque interruption
du timer TIM16. Une interruption intervient toutes les 1ms permettant ainsi de réaliser une rampe d'accélération pour éviter les appels de
courant trop importants.
Cette tâche est bloqué par deux flags :
  - EdgeProcess, actif lorque le robot réalise la manoeuvre d'évitement du bord
  - ShockProcess, actif lorque le robot réalise la manoeuvre de changement de direction suite à un choc
Cet ajout permet de réaliser les manoeuvres d'évitement de bord et de changement de direction suite à un choc sans que TaskMOTOR remette la vitesse
des moteurs à leur valeur maximale en ligne droite.

### TaskETAT de priorité 2
Cette tâche reçoit une notification par l'interruption générée lorsque le robot reçoit un choc. Elle active le flag ShockProcess car on réalise 
un changement de direction qui ne doit pas être influencer par TaskMOTOR.

### TaskEDGE de priorité 3
Cette tâche reçoit une notification par les interruptions générées par les capteurs de bord. Elle active le flag EdgeProcess qui bloque TaskMOTOR
pour réaliser les manoeuvres d'évitement de bord. Ce flag bloque aussi les interruptions dûes aux chocs pendant l'évitement du bord.
Cet ajout permet de ne pas changer d'état suite à des changements brusques de direction. Il y avait des changements d'état virtuels lorsque le 
robot passait rapidement de la marche avant à l'arrêt lors de l'arrêt d'urgence. Augmenter le seuil de détection des chocs réglait ce problème 
mais rendait par la même occasion le robot moins réactif aux chocs. Le choix réalisé a cependant le défaut de ne pas détecter les chocs avec d'autres 
robots au niveau des bords.
