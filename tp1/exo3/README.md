# Exercice 3 - Du faux partage en Java ?

## Question 1

Code : [src/Exercice3Q1.java](src/Exercice3Q1.java)
Le temps d'execution est de 17441ms
Les champs progress et result de chaque objet ThreadInfo peuvent causer un faux partage entre threads, car les instances
de ThreadInfo peuvent être stockées de manière contiguë en mémoire.

## Question 2

Code : [src/Exercice3Q2.java](src/Exercice3Q2.java)
Le temps d'execution est de 20801ms. C'est plus lent.

En Java, le mot-clé volatile garantit la visibilité des modifications entre threads : chaque thread lira toujours la
version la plus récente de la variable dans la mémoire principale. Cependant, cela n'empêche pas les effets du faux
partage, car ce problème est lié aux caches CPU.

En C, volatile signifie que la variable doit être directement lue/écrite en mémoire et ne peut pas être optimisée par le
compilateur, ce qui est plus bas niveau.

## Question 3

Code : [src/Exercice3Q3.java](src/Exercice3Q3.java)
Le temps d'execution est de 5944ms. Il a fallu utiliser artificiellement des variables pour éviter le faux partage mais
le code est beaucoup plus rapide.