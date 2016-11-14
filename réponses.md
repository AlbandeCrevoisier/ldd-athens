# Linux Device Drivers #

### Découverte noyau ###

#### Syntaxe et signification de la configuration ####

**D'où proviennent les fichiers `/boot/config-*` ? Dans quels cas sont-ils
utiles ?**

Configuration du noyau installé, générés lors d'un make config. Ilsrenseignent
sur la configuration actuelle du noyau, ce qui est pratique lorsque l'on compile
à nouveau le noyau pour la même machine : il suffit de copier cette
configuration.

**Existe-t-il d'autres cibles du `Makefile`  ayant un impact sur la
configuration ? Si oui lesquelles et à quoi peuvent-elles servir ?**

Oui, plein, la majorité de ce qui sort sur un `make help | grep config`.
Elles peuvent fournir différents types de front-end, reprendre la même
configuration qu'actuellement mais mise à jour, une configuration avec tout ou
rien, créer une configuration random, etc.

#### Exploration du noyau ####

**Calculez la taille de chacun des répertoires à la racine. Qu'en
pensez-vous ?**

3.1G de noyau, ça fait beaucoup de sources, doc, etc
1.3G .git : des logs qui remontent à une époque ancestrale
1.1G build : normal, le noyau, même s'il est bien lourd. 
395M drivers : beaucoup, beaucoup de drivers. Dommage d'en embarquer autant.
138M arch : semble normal pour supporter autant d'architectures.
35M Documentation : une doc bien fournie \o/

### Premier module ###

**Quels sont les avantages de l'utilisation d'un module plutôt que du code
compilé en dur directement dans l'image du noyau ?**

On gagne en souplesse, puisqu'on peut charger et décharger les modules quand on
le souhaite, cela réduit la taille du code qui tourne (et donc les bugs, failles
etc).
Il est également plus simple de n'avoir que le module à compiler,
cela doit être appréciable pour les entreprises qui payent des devs pour écrire
des pilotes pour leurs périphériques.
De plus, comme le module ne fait pas vraiment partie de Linux, il n'est pas pris
en compte par la GPLv2 de Linux, ce qui est favorable aux entreprises.

#### Votre premier module ####

**Pourquoi ne se passe-t-il rien (en apparence) ?**

Où la sortie est-elle dirigée ? Aucune raison que ça soit le stdout de
l'utilisateur.

#### Makefile ####

**Expliquez comment fonctionne le `Makefile` présenté plus haut (i.e. RTFM).**
`fneq ($(KERNELRELEASE),)`
Si la variable d'environnement KERNELRELEASE est définie, alors c'est kbuild
qui a appelé le makefile, et dans ce cas seul la ligne :
`obj-m  := first.o`
est lue, soit kbuild génère le module. Sinon,

```
KDIR ?= /lib/modules/`uname -r`/build
```
`uname -r` est remplacé par la version du noyau, cette ligne donne donc le
chemin vers le build du noyau utilisé par la distro pour compiler les modules
externes.

Le test est du au fait que le début ne doit servir qu'à kbuild (seconde passe)
alors que le milieu ne sert qu'à make (première passe), on peut également faire
deux fichiers séparés.

`$(MAKE) -C $(KDIR) M=$$PWD`
-C donne le chemin vers les sources du noyau à utiliser
M= donne le chemin vers les sources du module extérieur, ce qui au passage
indique que l'on compile un module extérieur et appelle donc kbuild.

