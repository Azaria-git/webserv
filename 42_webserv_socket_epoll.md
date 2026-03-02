# 🚀 Mon expérience sur la création d’un Socket TCP en C++98 et l’intégration d’`epoll`

Dans le cadre d’un **projet d’équipe de 3 personnes sur 42 Webserv**, j’ai pris en charge la **création et la configuration d’un socket TCP** sur Linux, ainsi que son **intégration avec `epoll`** pour gérer efficacement plusieurs clients. L’objectif était de préparer un serveur modulable, robuste et évolutif.

J’ai encapsulé le socket dans une **classe `Socket`**, centralisant création, configuration et fermeture, ce qui m’a permis de fournir une base réutilisable pour le reste de l’équipe et de mieux comprendre le rôle exact d’un socket TCP.

Le socket a été configuré en **mode non-bloquant** avec `fcntl` et j’ai activé `SO_REUSEADDR` pour éviter les problèmes de bind lors du redémarrage. Toutes les configurations (ports et adresses IP possibles) sont définies dans un **fichier de configuration**, rendant le serveur flexible et facilement adaptable.

Après la création du socket, j’ai intégré **`epoll`** pour la gestion des événements. Cette étape m’a permis de **maîtriser la lecture/écriture non-bloquante**, la surveillance de multiples sockets simultanément et l’optimisation des performances réseau. J’ai également géré les erreurs via **retours booléens** et **exceptions spécifiques** (comme `EAGAIN`) et assuré la **fermeture propre des sockets**.

Cette étape m’a permis de développer une **compréhension solide des sockets TCP, du non-blocking et de la gestion multi-événements avec `epoll`**, tout en mettant en place un code **robuste, modulable et facilement configurable**, prêt pour les étapes suivantes du projet.
