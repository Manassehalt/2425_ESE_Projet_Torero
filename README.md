<p align="left"> <img src="Graphique_Logo/logo ENSEA.png" width="15%" height="auto" /> </p>
<p align="center"> <img src="Graphique_Logo/Torero-Logo.png" width="30%" height="auto" /> </p>

# 2425_ESE_Projet_Torero

Projet de 3ème année d'ESE réalisé par :  
- **JACQUOT Nolan**  
- **GUIFFAULT Gabriel**  
- **BOULOT Paul**  
- **THOMAR Jean-Philippe**

Ce projet consiste à concevoir un robot autonome de A à Z, combinant électronique et programmation, pour jouer une partie de "chat et souris".

---

## Cahier des charges:
> [!IMPORTANT]
>- Le robot doit être capable de se déplacer sur une table sans tomber (absence de bord et de marquage)
>- Le robot doit être capable de détecter l'autre robot présent sur la table pour le fuir ou le chasser en fonction de son rôle dans la partie.
>- Le robot doit être capable de détecter un contact, même léger, avec le robot adverse et changer son comportement d'un rôle vers l'autre.

Objectif annexe:
- Le robot peut réaliser une odométrie simple pour suivre le bord de table lorsqu'il débute en souris
- Le robot peut réaliser une odométrie simple pour se positionner au centre de la table avant de commencer la traque lorsqu'il débute en chat
- Le robot peut déclencher un brusque changement de direction s'il détecte un robot approchant rapidement en mode souris
- Le robot peut effectuer des prédictions de trajectoire et effectuer des trajectoires de collision en prévision du deplacement adverse en mode chat
- Le robot évite de prendre des directions ou il serait acculé dans un angle de table

## Architecture du système

<p align="center"> <img src="Graphique_Logo/Schéma_ArchiV2.png" width="80%" height="auto" /> </p>
[Schéma architectural](https://github.com/user-attachments/files/17220902/Schema.architectural.pdf)

1. **Détection de l'environnement** :
   - Les **capteurs de bord** détectent les limites de la table pour prévenir les chutes.
   - Le **LIDAR** identifie et localise les robots adverse.
   - La **centrale inertielle** mesure les mouvements brusques et détecte les chocs pour changer l'état, chat ou souris, du robot.

2. **Traitement des données** :
   - Le **microcontrôleur** centralise les informations provenant des capteurs, analyse les données et décide des actions en fonction du rôle actuel du robot (chat ou souris).

3. **Actionneur** :
   - Les moteurs sont contrôlés via un système de quadrants.
   - Le microcontrôleur ajuste les commandes de mouvement en temps réel (éviter les chutes, fuir ou poursuivre le robot adverse).

4. **Comportement interactif** :
   - Un **switch de comportement** permet de basculer entre les rôles de "chat" et "souris".
   - Les **LED de debug** affichent des informations sur l'état et le rôle actuel du robot.

5. **Alimentation** :
   - Une **batterie** alimente l'ensemble du système.
   - Des convertisseurs intégrés fournissent les tensions nécessaires (7,2 V, 5 V et 3,3 V) pour les différents composants.

## Environnement:


> [!IMPORTANT]
> Table de 200cm x 160cm
> 
> Table sans bords ni marquages!
> 
> Robot assimilable à un cylindre de 17 cm de diametre et 15 cm de hauteur 

##  Liens utiles

### Documentation et ressources :
- [Datasheets](https://github.com/Manassehalt/2425_ESE_Projet_Torero/tree/main/Datasheet)  
- [Liste des tâches](https://github.com/Manassehalt/2425_ESE_Projet_Torero/blob/main/Calculs%20et%20brouillons/Remarque_Sch.md)  
- [BOM (Bill of Materials)](https://github.com/Manassehalt/2425_ESE_Projet_Torero/blob/main/BOM.xlsx)
  
### Design électronique :
- [Fichiers PCB](https://github.com/Manassehalt/2425_ESE_Projet_Torero/tree/main/PCB)  
- [Schémas KiCad](https://github.com/Manassehalt/2425_ESE_Projet_Torero/blob/main/PCB/Projet_torero/Projet_torero.kicad_sch)  

### Code STM32 :
- [Dossier code](https://github.com/Manassehalt/2425_ESE_Projet_Torero/tree/main/Projet_ESE_STM32)  

---

### Aperçu de la carte PCB
<p align="center"> <img src="Graphique_Logo/PCB carte 3d.PNG" width="100%" height="auto" /> </p>

[Voir le modèle en 3D](https://github.com/Manassehalt/2425_ESE_Projet_Torero/blob/main/Graphique_Logo/PCB%20carte%203d.PNG)  

### Calculs et développement

- [Dossier calculs](https://github.com/Manassehalt/2425_ESE_Projet_Torero/tree/8892ed05e16a1e8a040fe9dc08f957ab23dc9e8/Calculs%20et%20brouillons)  

