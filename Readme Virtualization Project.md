# *Migration de machines virtuelles dans un environnement heterogene*

Ce dépôt contient les instructions et les scripts  et autres fichiers nécessaires afin de permettre la migration de machines virtuelles d'environnments heterogenes en utilisant l'hyperviseur XEN.


## Prérequis

-   Avoir installé l'hyperviseur Xen et configuré [xl](https://xen-org.atlassian.net/wiki/spaces/XL/pages/3882940/xl) sur les deux environnements.
-   Les deux environnements doivent avoir une version compatible de Xen.
-   Connection du domU au dom0 et a internet.


## Instructions

1. Permettre l'echange de fichier entre le domU et le dom0 (dans notre cas, via une connexion ssh)
2. Téléchargez les scripts de migration sur l'hôte source (domU).
3.  Exécutez les differents scripts de notre conception afin de recuperer les features essentielles du processeur de la machine hote, etablir une connection internet avec le domU, configurer la migration et effectuer les tests suites.


## Notes

-   Assurez-vous de sauvegarder toutes les données importantes de la machine virtuelle avant de lancer la migration.
-   La migration peut prendre du temps en fonction de la taille de la machine virtuelle et de la bande passante disponible entre les hôtes.

## Auteurs

-   Tanankem Menkem Ramses
-   Ndoum Said
-   Maimouna Bia
-   Ndongmo Mbipbipe Ariane
-   Nima Embiele Johanna
-   Ndjog Allan