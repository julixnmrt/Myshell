# Myshell - Mini-Shell en C

## Introduction

Ce projet consiste à créer un **mini-shell** en langage C, inspiré du fonctionnement des shells Unix classiques comme `bash` ou `sh`.  
L'objectif est de comprendre les concepts fondamentaux des shells :

- Lecture et parsing des commandes  
- Gestion d'une boucle de prompt interactive  
- Exécution de commandes internes ou externes  
- Création et gestion de processus avec `fork` et `exec`  

Le projet est construit **progressivement**, en ajoutant des fonctionnalités étape par étape.


## 1 : Boucle interactive simple

La première étape consiste à implémenter une **boucle infinie** qui :

1. Affiche un **prompt** (`myshell> `)  
2. Lit la ligne entrée par l'utilisateur  
3. Affiche cette ligne (ou ses arguments) dans le terminal  

### Fonctionnement

- Le shell attend que l'utilisateur tape une commande et appuie sur Entrée.  
- La commande est lue avec [`getline`](http://manpagesfr.free.fr/man/man3/getline.3.html).
- Pour l'instant, le shell se contente d'afficher ce que l'utilisateur a entré

### Exemple d'utilisation
```bash
myshell> ls -al \tmp
ls -al \tmp
myshell> ceci est un test
ceci est un test
```

<details>
  <summary>Indice</summary>

  ```C
  int main(void){
        //pointeur line

        while (1)
        {
            printf("myshell> ");
            fflush(stdout);

            if (/*getline*/)
            {
                printf("\n");
                break;
            }
            printf("%s", line);
        }
    }
  ```
</details>


<details>
  <summary>Afficher le code</summary>

  ```C
  int main(void){
        char *line = NULL;
        size_t len = 0;

        while (1)
        {
            printf("myshell> ");
            fflush(stdout);

            if (getline(&line, &len, stdin) == -1)
            {
                printf("\n");
                break;
            }
            printf("%s", line);
        }
    }
  ```
</details>

## 2: Parsing 