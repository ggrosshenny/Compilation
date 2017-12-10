---
title: Rapport du projet de compilation
date: 2017-12-10
author: 
- Guillaume Grosshenny
- Audric Wasmer
...

\newpage

\tableofcontents

\newpage

Travail effectué
================

Le compilateur supporte :

* Les expressions arithmétiques
* Les expressions booléennes
* Les affectations
* Les structures de contrôle
* Les tableaux
* La déclaration et l'utilisation des stencils
* Les "define"
* Les déclarations et les appels de fonctions

Le compilateur :

* Ne supporte pas les returns
* N'applique aucune optimisation de code

Points importants du compilateur
================================

Le compilateur utilise un *AST* afin de gérer l'analyse sémantique, la génération de la table des symboles et la génération du code intermédiaire. Vous trouverez tout le code utile dans le dossier **/AST**. La structure du code est modulaire, chaque étape est gérée dans un fichier différent et les structures ont été pensées pour cette modularité. Ainsi, il est aisé de rajouté un élément au compilateur. L'ensemble de l'avancée du travail est disponible sur le github suivant : *https://github.com/ggrosshenny/Compilation*.

Utilisation du compilateur
==========================

dans le dossier racine du projet, taper la commande *make* pour compiler le compilateur. Pour l'utiliser, il suffit de taper la commande : *./compilateur [fichier]*.
