# ISMIN Blockchain

Ce projet illustre le fonctionnement d'une blockchain, à savoir l'algorithme de Proof of Work permettant d'assurer la validité de la blockchain.
L'implémentation de transactions entre pairs n'a pas été réalisée car inutilise sans réseau P2P.
Le consensus du réseau et le fonctionnement décentralisé n'a pas pu être implémenté même si des idées sont proposées dans la partie 'Améliorations possibles'

## Constantes 
---
- HASH_SIZE -> Nombre de bits de stockage du hash dans la librairie openssl
- HASH_HEX_SIZE -> Chaque block a un hash de 65 caractères
- BINARY_SIZE -> Taille nécéssaire pour stocker le hash hexadécimal en binaire
- MESSAGE_SIZE -> Taille maximale d'un message stocké dans la blockchain
- BLOCK_STR_SIZE -> Taille maximale de la chaîne de caractère représentant le block
- BLOCK_GENERATION_INTERVAL -> Combien de temps séparent chaque block
- DIFFICULTY_ADJUSMENT_INTERVAL -> Intervalle d'ajustement de la difficulté de "mining"

## Structure de données utilisées
---
### Block :
- index : indice du block
- hash : hash du block
- previousHash : hash précedent
- timestamp : date de création du block
- message : donnée du block
- difficulty : difficultée de "mining" pour ce bloc
- nonce : nombre "poubelle" permettant d'avoir un hash différent en changant ce nombre
- next : pointeur vers le prochain bloc (e.g liste chaînée)

### Blockchain :
- head : tête de la liste chaînée
- length : taille de la liste


## Algorithmes utilisées
---
La plupart du travail est réalisé dans la fonction *generateNextBlock*.
On y implémente un algorithme de ProofOfWork classique, utilisant le bruteforce:
- Calcul d'un hash pour le block tant que celui-ci ne remplit pas la condition de difficulté
- Pour savoir si le hash calculé contient le bon nombre de '0' attendus au début, il faut d'abord le convertir en binaire.
Ensuite la fonction *hashMatchesDifficulty* fait le travail.
- Si le hash ne remplit pas la condition, on incrémente le nombre "poubelle" et on recommence...

Lorsque un hash satisfaisant est trouvé, on peut ajouter le block à la Blockchain (par la tête).

---
La validation d'un Block se fait en 4 étapes :
- Validation de l'indice (indice précédent + 1)
- Validation du hash précédent qui doit bien correspondre
- Recalcul du hash pour être sur que le block n'a pas été modifié entre temps
- Validation de la date de création du block

La validation de la Blockchain consiste simplement à valider chaque bloc un par un en commencant par le dernier ajouté.

## Améliorations possibles
---

Il est possible de simuler un fonctionnement décentralisé en distribuant le calcul dans plusieurs threads, simulant ainsi plusieurs machines qui "minent" des blocks. Le premier thread trouvant un block le communique alors aux autres threads et si celui-ci est valide, il peut alors être ajouté à la blockchain.

Quand deux blocks sont trouvés en même temps il faut établir un consensus, afin de choisir la blockchain que l'on conserve.
L'algorithme de consensus utilisé aurait été celui du Bitcoin, à savoir prendre la blockchain ayant la somme des 2^difficulté la plus élevée. (Consensus de Nakamoto)

Une interface graphique aurait pu alors être ajoutée pour permettre à l'utilisateur d'envoyer ses messages dans le réseau et d'observer la blockchain.


*Crédits : Louis Mannix et Matthias Hallgren.*
