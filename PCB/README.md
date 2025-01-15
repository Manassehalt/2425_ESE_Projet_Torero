# Partie PCB - Projet Torero

Ce projet utilise **KiCad** pour la conception des schémas et des PCB. L'architecture électronique est organisée de manière hiérarchique pour une gestion claire.

---

##  Organisation des fichiers
La hiérarchie des schémas est structurée en plusieurs pages, chacune correspondant à une fonctionnalité spécifique :

- **Page principale (Racine)** : Vue d'ensemble de l'architecture.
- **STM32 (Page 2)** : Microcontrôleur central et ses connexions.
- **Puissance (Page 3)** : Alimentation et distribution des tensions.
  - Contrôle des moteurs gauche et droit.
- **Capteurs (Page 4)** : Intégration des capteurs, tels que le Lidar, l'accéléromètre, et les détecteurs de bord.
- **Alimentation (Page 5)** : Régulateurs pour les tensions de 3,3 V et 5 V.

---

## Composants principaux

### **Microcontrôleur STM32** (Page 2)
- **STM32G431CBU** : Le contrôle central du système.
- Connexions UART, SPI, et GPIO configurées pour communiquer avec les capteurs, moteurs, et périphériques.

### **Puissance et Moteurs** (Pages 3, 6, 7)
- **Drivers moteurs ZXBM5210-SP** pour le contrôle des moteurs gauche et droit.
- Les connecteurs moteurs incluent des signaux pour les encodeurs et les commandes de direction.

### **Capteurs** (Page 4)
- **Lidar** : Connecté via UART pour les mesures de distance et d'obstacles.
- **Accéléromètre ADXL343** : Géré via SPI pour les données inertiales.
- **Capteurs de bord QRD1114** : Utilisés pour détecter les limites de la table.

### **Alimentation** (Page 5)
- Régulateur **5 V** : MP14755.
- Régulateur **3,3 V** : BU33SD5WG-TR.
- Connecteurs pour la batterie 7,2 V et le switch ON/OFF.

---

## Fonctionnalités de conception
- **Hiérarchie des schémas** : Organisation pour une meilleure lisibilité.
- **Test points** : Ajoutés pour faciliter les mesures et le debug sur le PCB avec l'ocsilloscope.

---

## Aperçu des schémas

### Vue principale
![image](https://github.com/user-attachments/assets/d5b51745-18c6-4a9c-8f8e-032a793fdd09)

### STM32
![image](https://github.com/user-attachments/assets/89c283f3-68ce-4b55-bbaf-cb2c9a038328)

### Puissance
![image](https://github.com/user-attachments/assets/c1104057-1ae1-49f7-af87-efd100b9e126)

Voici le schéma d'un des moteurs :

![image](https://github.com/user-attachments/assets/ee2cc9cb-104b-4208-a33a-d1947327ecfc)

### Capteurs
![image](https://github.com/user-attachments/assets/a33f190a-a8d4-418a-b449-527e2fc9abad)

Voici le schéma d'un des caepterus de bord :

![image](https://github.com/user-attachments/assets/9b12c729-1bf5-4a62-af90-fe520a7cc88e)

### Alimentation
![image](https://github.com/user-attachments/assets/f5ae0c31-2263-4cc9-9992-8436d3f53350)


