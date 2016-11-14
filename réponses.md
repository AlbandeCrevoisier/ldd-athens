#### Linux Device Drivers ####

## Découverte noyau ##

# Syntaxe et signification de la configuration #

**1. D'où proviennent les ficihers `/boot/config-*` ? Dans quels cas sont-ils
utiles ?**

Configuration du kernel installé, générés lors d'un make config. Ilsrenseignent
sur la configuration actuelle du noyau, ce qui est pratique lorsque l'on compile
à nouveau le noyau pour la même machine : il suffit de copier cette
configuration.

**3. Existe-t-il d'autres cibles du `Makefile`  ayant un impact sur la
configuration ? Si oui lesquelles et à quoi peuvent-elles servir ?**

Oui, plein, la majorité de ce qui sort sur un `make help | grep config`.
Elles peuvent fournir différents types de front-end, reprendre la même
configuration qu'actuellement mais mise à jour, une configuration avec tout ou
rien, créer une configuration random, etc.

# Exploration du noyau #

**3. Calculez la taille de chacun des répertoires à la racine. Qu'en
pensez-vous ?**

3.1G de kernel, ça fait beaucoup de sources, doc, etc
1.3G .git : des logs qui remontent à une époque ancestrale
1.1G build : normal, le kernel, même s'il est bien lourd. 
395M drivers : beaucoup, beaucoup de drivers. Dommage d'en embarquer autant.
138M arch : semble normal pour supporter autant d'architectures.
35M Documentation : une doc bien fournie \o/


