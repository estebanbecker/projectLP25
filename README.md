# Projet de C LP25

Le contexte du projet est une base de données gérée par votre programme. La base de données est constituée de plusieurs tables, chacune gérée par des fichiers portant le nom de la table gérée et une extension en fonction du rôle du fichier (structure, index, contenu). La base de données est dans un répertoire portant son nom, et chaque ensemble de fichiers d'une table est dans un sous-répertoire du même nom, contenu par le répertoire de la base de données elle-même.

Le programme qui gère la base de données gère les arguments suivants :

- `-d` suivi d'un nom, le nom de la base de données à ouvrir/créer
- `-l` suivi d'un chemin, le chemin vers le répertoire parent de la base de données (i.e. le répertoire qui contient celui de la base de données)

Pour plus de clarté, le programme est décomposé en plusieurs parties correspondant chacune à un aspect de la gestion de la base de données.

Les sections suivantes définissent les différents éléments du projet, qui seront les critères d'évaluation du projet. Ce document est actuellement incomplet et sera mis à jour dans les jours à venir.

Une base de données est un outil permettant la gestion de données structurées ou non. Dans ce projet, nous nous intéressons à créer une base de données simplifiée manipulée par un sous-ensemble de SQL.

Pour vous permettre de mener à bien ce projet, vous devez comprendre comment les données seront stockées par la base de données, quelles sont les commandes SQL que vous devrez implémenter, et comment faire l'interface entre le contenu de la base de données sur votre support de stockage et le résultat du *parsing* SQL.

Des fichiers avec les définitions du code vous sont fournis avec ce README afin de garantir que la structure de votre programme est correcte.

## Représentation de la base de données sur le support de stockage

Pour les besoins de ce projet, nous considérons qu'une base de données est stockée dans un répertoire qui lui est propre et qui porte son nom, i.e. la base de données `inventaire` est stockée dans un répertoire nommé `"inventaire"`. Une base de données est constituée de tables, qui sont des ensembles structurés de données de même nature. Dans le répertoire de la base de données, chaque table dispose de son propre répertoire. Ce répertoire de table contient 3 ou 4 fichiers relatifs à cette table : le fichier de définition, le fichier d'index, le fichier de contenu et le cas échéant, le fichier de clé.

### Le fichier de définition

Ce fichier contient la description des champs de la table. La liste des champs est définie par un champ par ligne avec son type, défini par l'énumération `field_type_t` (c.f. la définition des types utilisés par le programme). Ce fichier permet de connaître l'ordre et le type des champs stockés par la table.

Une ligne définissant un champ est écrite de la manière suivante : `N nom` où `N` est le numéro de type de champ issu de l'énumération `field_type_t`, et `nom` est le nom du champ.

### Le fichier d'index

Le fichier d'index est composé d'enregistrements de taille fixe. Chaque enregistrement est composé de 3 champs :

| Active | Offset | Length |
| ------ | ------ | ------ |

dont voici la signification :

- **active** : ce champ est défini sur un octet (type `uint8_t`). Il a pour valeur zéro si cet index n'est pas actif (il peut donc être réutilisé), et une valeur différente de zéro dans le cas contraire. Une valeur de zéro signifie que le contenu correspondant doit être ignoré en lecture et peut être réutilisé en écriture.
- **offset**: ce champ est défini sur 4 octets (type `uint32_t`). Il définit la position de ce champ dans le fichier de contenu. Il définit la position en octets à partir du début du fichier.
- **length**: ce champ est défini sur 2 octets (type `uint16_t`). Il définit la taille de l'enregistrement courant. Cette taille sera toujours la même pour une table donnée (donc chaque ligne d'index a une valeur *length* identique, pour simplifier l'accès aux données)

Ce fichier est utile pour accéder aux enregistrements de la table.

### Le fichier de contenu

Ce fichier contient les données de la table. Chaque enregistrement est stocké dans l'ordre de la définition de la table (obtenue en lisant son fichier de définition). Les données de type `float`, `int` et `primary key` sont stockées sous leur forme binaire. Les chaînes de caractères sont stockées intégralement (il s'agit de tableaux de longueur fixe). Tous les enregistrements d'une table ont donc une longueur identique.

Par exemple, avec les valeurs données ci dessous (longueur des chaînes de caractères égale à 150), les enregistrements d'une table définie par un `int` (type SQL), un `float` et deux `text` auront une taille de 316 octets, avec accès à l'entier au premier octet de l'enregistrement courant, accès au nombre réel au neuvième entier, accès à la première chaîne de caractères au 17ème octet et accès à la seconde chaîne de caractères au 167ème octet.

### Le fichier de clé

Si un champ de la table est défini du type `primary key`, un quatrième fichier est créé : il contient une valeur binaire d'un `unsigned long long` initialisé à `1` et incrémenté à chaque insertion d'un enregistrement dans la table, lorsqu'aucune valeur pour cette clé n'est spécifiée. Dans le cas contraire, la valeur est mise à jour avec la valeur maximale de ce champ dans la table, incrémentée de `1`;

## Langage SQL simplifié

Interagir avec une base de données se fait notamment avec le langage SQL (*Structured Query Language*). Dans ce projet, vous serez amenés à manipuler les requêtes (simplifiées) suivantes :

- `CREATE TABLE`
- `INSERT`
- `SELECT`
- `DELETE`
- `UPDATE`
- `DROP TABLE`
- `DROP DATABASE` ou `DROP DB`

Une requête SQL se termine toujours par un caractère `';'`.

La structure des requêtes SQL est définie ci-dessous.

### Création d'une table

La structure d'une commande création de table est la suivante : `CREATE TABLE table_name (field1_name field1_type, ...fieldN_name fieldN_type);`

Le nom d'une table, et les noms de champs obéissent aux même règles que les noms de variables en C. Les types des champs seront les suivants :

- `int` pour un entier (représenté par un `long long`)
- `primary key` pour un entier non signé (`unsigned long long`) avec incrémentation automatique.
- `float` pour un nombre flottant (type `double` dans l'implémentation)
- `text` pour du texte.

### Suppression d'une table

La suppression d'une table se fait avec la commande SQL suivante : `DROP TABLE table_name;`. Elle permet de supprimer la table nommée `table_name`. 

### Suppression d'une base de données

Le principe est similaire à celui de la suppression d'une table : on supprime une BDD avec la commande SQL suivante : `DROP DATABASE db_name;`.

### Insertion d'un enregistrement dans une table

La commande SQL utilisée est la suivante : `INSERT INTO table_name (field1, ... fieldN) VALUES (value1, ... valueN);`

Cette commande ajoute à la table nommée `table_name` les valeurs `value1` à `valueN` dans les champs `field1` à `fieldN` (donc le champ `field1` aura la valeur `value1`, le champ `field2` aura la valeur `value2` et ainsi de suite). Les valeurs sont encadrées par des quotes simples quand il s'agit de chaines de caractères.

### Sélection d'enregistrements à partir d'une table

La commande SQL utilisée est la suivante : `SELECT * FROM table_name WHERE condition;` ou `SELECT field1, ... fieldN FROM table_name [WHERE condition];`

### Suppression d'un enregistrement dans une table

La commande SQL utilisée est la suivante : `DELETE FROM table_name [WHERE condition];`

### Modification d'un enregistrement dans une table

La commande SQL utilisée est la suivante : `UPDATE table_name SET field1=value1, ..., fieldN=valueN [WHERE condition];`

### Clauses WHERE

Dans les 3 requêtes suivantes, il peut être nécessaire de filtrer des champs pour les visualiser ou les modifier. C'est le rôle de la clause facultative `WHERE` (le fait qu'elle ne soit pas requise est matérialisée ci dessous par des `[ ]` de part et d'autre de la clause `WHERE`).

Cette clause est composée du mot-clé `WHERE` suivi d'un ensemble de conditions. Nous nous contenterons ici de ne combiner ensemble soit que des `AND`, soit que des `OR`. Une condition sera donc de la forme :

- `WHERE field1=value1` pour une clause `WHERE` sur un seul champ.
- `WHERE field1=value1 AND ... AND fieldN=valueN` pour une clause `WHERE` nécessitant que toutes les conditions soient remplies.
- `WHERE field1=value1 OR ... OR fieldN=valueN` pour une clause `WHERE` nécessitant qu'au moins une des conditions soit remplie.

Pour gérer des clauses `WHERE`, vous devrez implémenter la fonction suivante : `int create_filter_from_sql(char *where_clause, s_filter *filter)`. La fonction va lire la requête, en extraire le nom de la table, et construire un filtre stocké dans la variable pointée par `filter`.

## Tâche 1 : analyse SQL

La première tâche à accomplir pour faire fonctionner votre base de données est de lui donner la capacité d'interpréter les requêtes SQL que vous lui transmettrez (au clavier). Une fois lancé, le programme se met en attente de la saisie au clavier, et va analyser les requêtes qui sont saisies. Pour cela, il va vous être nécessaire d'implémenter la fonction :

```c
query_result_t *parse(char *sql, query_result_t *result);
```

Il s'agit d'une fonction qui prend en paramètres une commande SQL à analyser (paramètre `sql`), et une pointeur sur une structure de type `query_result_t` (paramètre `result`) déjà allouée en mémoire (statiquement ou dynamiquement). L'ensemble des types de données est défini dans une section ultérieure.

La fonction retourne le pointeur sur `result` après l'avoir rempli avec les valeurs résultant de l'analyse de la requête. Elle retourne `NULL` en cas d'échec.

Pour réaliser cette fonction, vous aurez besoin des fonctions spécialisées suivantes :

```c
query_result_t *parse_select(char *sql, query_result_t *result);
query_result_t *parse_create(char *sql, query_result_t *result);
query_result_t *parse_insert(char *sql, query_result_t *result);
query_result_t *parse_update(char *sql, query_result_t *result);
query_result_t *parse_delete(char *sql, query_result_t *result);
query_result_t *parse_drop_db(char *sql, query_result_t *result);
query_result_t *parse_drop_table(char *sql, query_result_t *result);
```

Chacune de ces fonctions traite l'analyse d'un seul type de requête. Leur signature est la même que celle du parser global, ainsi que le comportement.

L'ensemble de ces fonctions s'appuie sur des fonctions d'aide pour parser les différents types de sous-chaîne SQL :

```c
char *get_sep_space(char *sql);
char *get_sep_space_and_char(char *sql, char c);
char *get_keyword(char *sql, char *keyword);
char *get_field_name(char *sql, char *field_name);
bool has_reached_sql_end(char *sql);
char *parse_fields_or_values_list(char *sql, table_record_t *result);
char *parse_create_fields_list(char *sql, table_definition_t *result);
char *parse_equality(char *sql, field_record_t *equality);
char *parse_set_clause(char *sql, table_record_t *result);
char *parse_where_clause(char *sql, filter_t *filter);
```

Chaque fonction prend comme premier paramètre la position actuelle dans la commande SQL définie par  `sql`.

**get_sep_space**
Cette fonction vérifie la présence d'une séquence de un à un nombre indéterminé d'espaces à partir de la position de `sql`. Elle renvoie un pointeur sur le premier non-espace rencontré.

**get_sep_space_and_char**
Cette fonction vérifie la présence d'une séquence de 0 ou plus espaces, puis du caractère `c` une unique fois, puis d'encore 0 ou plus espaces. Elle renvoie un pointeur sur le caractère suivant cette séquence. Cette fonction est par exemple utile pour les séquences séparées par des virgules.

**get_keyword**
Cette fonction vérifie que le mot-clé passé en paramètre est identique au mot dont le premier caractère est pointé par `sql`. La casse n'est pas prise en compte (par exemple, `SELECT` et `select` sont valides lors de l'appel de `get_keyword(sql, "select")`). Le pointeur renvoyé pointe sur le caractère qui suit le mot-clé.

**get_field_name**
Cette fonction permet d'extraire le nom d'un champ, le nom d'une table, ou la valeur d'un champ (avant sa conversion dans un type donné). Ces différentes valeurs en SQL peuvent comporter des espaces (dans le cas du texte) mais il est alors nécessaire d'encadrer le champ par des quotes simples `'`. La fonction prend en second paramètre le buffer où copier la valeur du champ. Ce dernier ne contient pas les quotes le délimitant quand il y en a. Le pointeur renvoyé pointe sur le caractère suivant le champ ou la quote de fermeture.

**has_reached_sql_end**
Cette fonction teste si le restant de la chaîne pointée par `sql` est la fin de la requête SQL, i.e. elle n'est composée que d'espaces jusqu'au caractère de fin de chaîne. Elle renvoie `true` si c'est le cas, `false` sinon.

**parse_fields_or_values_list**
Cette fonction extrait (en s'appuyant sur les précédentes) une liste de champs ou de valeurs (tels qu'on les trouve dans les requête **select** ou **insert**). Ce type de liste est composée de champs séparés par des virgules. Le résultat de cette fonction est écrit dans la structure `table_record_t` pointée par `result`. La fonction retourne un pointeur sur le caractère suivant la liste de valeurs.

**parse_create_fields_list**
Cette fonction extrait dans une structure `table_definition_t` pointée par `result` la définition d'une table. La définition est une succession de paires (nom de champ, type de champ séparés par un espace) séparées par des virgules. Cette fonction est utilisée pour la requête **create table**. La fonction retourne un pointeur sur le caractère suivant la liste de définitions de champs.

**parse_equality**
Cette fonction extrait une égalité (qui peut être une affectation selon le type de clause analysée) et la stocke dans la structure `field_record_t` pointée par `equality`, avec un type de données marqué comme `TYPE_UNKNOWN`. Ces égalités existent dans les clauses **set** et **where**. La fonction retourne un pointeur sur le caractère suivant l'égalité.

**parse_set_clause**
Cette fonction parse une clause **set**, composée d'une liste d'au moins une égalité (voir **parse_equality**). Les égalités sont séparées par des virgules lorsqu'il y en a plus d'une. Le résultat est stocké dans une structure `table_record_t` pointée par `result`. La fonction retourne un pointeur sur le caractère suivant la liste d'égalités.

**parse_where_clause**
Cette fonction parse une clause **where**, composée d'une liste d'au moins une égalité (voir **parse_equality**). Les égalités sont séparées par un opérateur logique (`OR` ou `AND`) lorsqu'il y en a plus d'une. Le résultat est stocké dans une structure `filter_t` pointée par `filter`. La fonction retourne un pointeur sur le caractère suivant la clause **where**.

**Valeur de retour**
À l'exception de la fonction `has_reached_sql_end`, chacune de ces fonctions retourne un pointeur sur le caractère de la chaîne `sql` après la fin de l'analyse du champ concerné. Toutes retournent `NULL` en cas d'échec, signifiant que la requête SQL est mal formée.

## Vérification d'une requête avec la structure de la base de données

Cette section sera renseignée prochainement.

## Exécution de la requête

Cette section sera renseignée prochainement.

# Annexes

## Convention de code

Il est attendu dans ce projet que le code rendu satisfasse un certain nombre de conventions (ce ne sont pas des contraintes du langages mais des choix au début d'un projet) :

- indentations : les indentations seront faites sur un nombre d'espaces à votre discrétion, mais ce nombre **doit être cohérent dans l'ensemble du code**.
- Déclaration des pointeurs : l'étoile du pointeur est séparée du type pointé par un espace, et collée au nom de la variable, ainsi :
	- `int *a` est correct
	- `int* a`, `int*a` et `int * a` sont incorrects
- Nommage des variables, des types et des fonctions : vous utiliserez le *snake case*, i.e. des noms sans majuscule et dont les parties sont séparées par des underscores `_`, par exemple :
	- `ma_variable`, `variable`, `variable_1` et `variable1` sont corrects
	- `maVariable`, `Variable`, `VariableUn` et `Variable1` sont incorrects
- Position des accolades : une accolade s'ouvre sur la ligne qui débute son bloc (fonction, if, for, etc.) et est immédiatement suivie d'un saut de ligne. Elle se ferme sur la ligne suivant la dernière instruction. L'accolade fermante n'est jamais suivie d'instructions à l'exception du `else` ou du `while` (structure `do ... while`) qui suit l'accolade fermante. Par exemple :

```c
for (...) {
	/*do something*/
}

if (true) {
	/*do something*/
} else {
	/*do something else*/
}

int max(int a, int b) {
	return a;
}
```

sont corrects mais :

```c
for (int i=0; i<5; ++i)
{ printf("%d\n", i);
}

for (int i=0; i<5; ++i) {
	printf("%d\n", i); }

if () {/*do something*/
}
else
{
	/*do something else*/}
```

sont incorrects.
- Espacement des parenthèses : la parenthèse ouvrante après `if`, `for`, et `while` est séparée de ces derniers par un espace. Après un nom de fonction, l'espace est collé au dernier caractère du nom. Il n'y a pas d'espace après une parenthèse ouvrante, ni avant une parenthèse fermante :
	- `while (a == 3)`, `for (int i=0; i<3; ++i)`, `if (a == 3)` et `void ma_fonction(void)` sont corrects
	- `while(a == 3 )`, `for ( i=0;i<3 ; ++i)`, `if ( a==3)` et `void ma_fonction (void )` sont incorrects
- Basé sur les exemples ci dessus, également, les opérateurs sont précédés et suivis d'un espace, sauf dans la définition d'une boucle `for` où ils sont collés aux membres de droite et de gauche.
- Le `;` qui sépare les termes de la boucle `for` ne prend pas d'espace avant, mais en prend un après.

## Évaluation du projet

Le projet est évalué sur les critères suivants :
- capacité à effectuer les traitements demandés dans le sujet,
- capacité à traiter les cas particuliers sujets à erreur (requêtes SQL mal formées, pointeurs NULL, etc.)
- Respect des conventions d'écriture de code
- Documentation du code
	- Avec des commentaires au format doxygen en entêtes de fonction
	- Des commentaires pertinents sur le flux d'une fonction (astuces, cas limites, détails de l'algorithme, etc.)
- Absence ou faible quantité de fuites mémoire (vérifiables avec `valgrind`)
- Présentation du projet lors de la dernière séance de TP

## Structures de données

On considérera pour faciliter le développement de cette base de données que certaines valeurs limites sont fixées :

```c
#define TEXT_LENGTH 150
#define MAX_FIELDS_COUNT 16
```

Les chaînes de caractères ne dépasseront donc pas 150 caractères (incluant le `\0` de fin de chaîne), et une table, ou toute requête, ne peut dépasser `16` champs.

Votre base de données nécessitera également un certain nombre de types composés pour fonctionner. Leurs définitions et utilités sont définies dans les sous sections qui suivent.

### Définition d'un champ de table

La définition d'un champ d'une table est gérée avec la structure ci dessous :

```c
typedef enum {
	TYPE_PRIMARY_KEY,
	TYPE_INTEGER,
	TYPE_FLOAT,
	TYPE_TEXT
} field_type_t;

typedef struct {
	char column_name[TEXT_LENGTH];
	field_type_t column_type;
} field_definition_t;

typedef struct {
	int fields_count;
	field_definition_t definitions[MAX_FIELDS_COUNT];
} table_definition_t;
```

C'est elle qui permet de définir le nom d'un champ ainsi que son type parmi l'énumération des types supportés. Cette structure est utilisée pour créer une table, pour en lister des données ainsi que pour s'assurer que l'ajout ou la modification de données est conforme à la structure de la table.

### Valeur de champ de table

Une valeur de champ de table est quant à elle la définition d'une valeur contenue dans un enregistrement de la table. Cette valeur doit donc associer un nom de champ avec une valeur, en s'assurant que le type de la valeur est conforme au type du champ dans la table.

```c
typedef struct {
    char column_name[TEXT_LENGTH];
    field_type_t field_type;
    union {
        double float_value;
        long long int_value;
        unsigned long long primary_key_value;
        char text_value[TEXT_LENGTH];
    } field_value;
} field_record_t;
```

L'accès à la valeur se fera par un `switch` sur le type de donnée du champ `field_type`.

### Définition d'un enregistrement

Un enregistrement de table se présente sous la forme suivante :

```c
typedef struct {
    int fields_count;
    field_record_t fields[MAX_FIELDS_COUNT];
} table_record_t;
```

Le champ `fields_count` indique combien d'éléments du tableau `fields` sont affectés pour être utilisés dans une requête.

### Définition d'une clause `WHERE`

La clause `WHERE` est définie sur 1 à N champs. On considère dans ce projet qu'elle ne peut contenir qu'un seul opérateur logique, qui sera soit `OR`, soit `AND` (qui sera répété N-1 fois pour N critères de filtre).

La structure à utiliser est la suivante :

```c
typedef enum {
    OP_OR,
    OP_AND,
    OP_ERROR
} operator_t;

typedef struct {
    table_record_t values;
    operator_t logic_operator;
} filter_t;
```

### Parsing SQL

Les types nécessaires au parsing sont les suivants :

```c
typedef enum {
    QUERY_NONE,
    QUERY_CREATE_TABLE,
    QUERY_DROP_TABLE,
    QUERY_SELECT,
    QUERY_UPDATE,
    QUERY_DELETE,
    QUERY_INSERT,
    QUERY_DROP_DB,
} query_type_t;

typedef struct {
    char table_name[TEXT_LENGTH];
    table_definition_t table_definition;
} create_query_t;

typedef struct {
    char table_name[TEXT_LENGTH];
    filter_t where_clause;
} drop_query_t;

typedef struct {
    char table_name[TEXT_LENGTH];
    table_record_t fields_names;
    table_record_t fields_values;
} insert_query_t;

typedef struct {
    char table_name[TEXT_LENGTH];
    table_record_t set_clause;
    filter_t where_clause;
} update_or_select_query_t;

typedef struct {
    query_type_t query_type;
    union {
        char table_name[TEXT_LENGTH];
        char database_name[TEXT_LENGTH];
        create_query_t create_query;
        drop_query_t drop_query;
        insert_query_t insert_query;
        update_or_select_query_t update_query;
        update_or_select_query_t select_query;
    } query_content;
} query_result_t;
```

Le type `query_result_t` est renvoyé par toutes les fonctions de parsing de haut niveau. Il comporte un champ indiquant le type de requête, puis une union à laquelle on accédera en fonction de la valeur du champ `query_type`. Chaque membre de l'union définit les éléments nécessaires pour effectuer la requête, notamment la table ou la base de données cible, les champs affectés et leurs valeurs, ainsi que l'éventuelle clause where.