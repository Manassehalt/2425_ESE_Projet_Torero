# 2425_ESE_Projet_Torero

Projet de ESE de 3éme année de JACQUOT Nolan, GUIFFAULT Gabriel, BOULOT Paul et THOMAR Jean-Philippe

Ce projet contient toute les ressources de l'élaboration d'un robot capable de jouer au chat et a la souris.
Les liens vers les differentes ressources sont énuméré avec un court intitulé a la FIN du README.
Ci dessous le schéma architectural donnant les grande ligne du projet.

[Schéma architectural.pdf](https://github.com/user-attachments/files/17220902/Schema.architectural.pdf)

Cahier des charges: 
-Le robot doit etre capable de ce déplacer sur une table sans tomber (absence de bord et de marquage)
-Le robot doit etre capable de detecter l'autre robot present sur la table pour le fuir ou le chasser en fonction de son role dans la partie.
-Le robot doit etre capable de detecter un contact meme leger avec le robot adverse et changer son comportement d'un role vers l'autre.

Objectif annexe:
-Le robot peut realiser une odometrie simple pour suivre le bord de table lorsqu'il debute en souris
-Le robot peut realiser une odometrie simple pour se positionner au centre de la table avant de commencer la traque lorsqu'il debute en chat
-Le robot peut declencher un brusque changement de direction si il detecte un robot approchant rapidement en mode souris

Methode si nouvel ordinateur : ssh-keygen(penser a mettre "y" pour overwrite)cat ~/.ssh/id_ed25519.pub

Dossier contenant les Datasheets : https://github.com/Manassehalt/2425_ESE_Projet_Torero/tree/main/Datasheet

LISTE des choses faite ou a faire : https://github.com/Manassehalt/2425_ESE_Projet_Torero/blob/main/Remarque_Sch.txt

Lien utile : https://github.com/Manassehalt/2425_ESE_Projet_Torero/blob/main/Liens.xlsx

BOM : https://github.com/Manassehalt/2425_ESE_Projet_Torero/blob/main/BOM.xlsx

INFO PCB : https://github.com/Manassehalt/2425_ESE_Projet_Torero/blob/main/PCB/info_PCB.docx

DOSSIER PCB : https://github.com/Manassehalt/2425_ESE_Projet_Torero/tree/main/PCB

Shematics_torero : https://github.com/Manassehalt/2425_ESE_Projet_Torero/blob/main/PCB/Projet_torero/Projet_torero.kicad_sch

DOSSIER CODE STM32 : https://github.com/Manassehalt/2425_ESE_Projet_Torero/tree/main/Projet_ESE_STM32/Projet_ESE_Torero/STM32G431CBU6

DOSSIER CALCULS : https://github.com/Manassehalt/2425_ESE_Projet_Torero/tree/8892ed05e16a1e8a040fe9dc08f9f57ab23dc9e8/Calculs%20et%20brouillons
