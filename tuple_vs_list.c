#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LIST_SIZE_MAX 2  // Capacité initiale par défaut pour la liste

typedef enum { TYPE_INT, TYPE_FLOAT, TYPE_CHAR } Type;

// Union pour stocker différents types de données
typedef union {
    int iValue;
    float fValue;
    char cValue;
} DataValue;

typedef struct Data {
    Type type;       // Type de la donnée
    DataValue value; // Union pour stocker la donnée
} Data;

// Structure de tuple contenant des adresses mémoire
struct tuple {
    char *name;
    Data** memory;
    unsigned int size;
};

// Structure de liste dynamique
typedef struct List {
    Data **memory;        // Tableau de pointeurs vers les éléments de la liste
    unsigned int size;    // Taille actuelle de la liste
    unsigned int capacity; // Capacité totale de la liste
} List;

// Fonction pour créer une nouvelle liste avec la capacité par défaut
List* create_list() {
    List* list = malloc(sizeof(List));
    if (list == NULL) {
        fprintf(stderr, "Erreur d'allocation de mémoire pour la liste\n");
        exit(1);
    }
    list->memory = malloc(LIST_SIZE_MAX * sizeof(Data*));
    if (list->memory == NULL) {
        fprintf(stderr, "Erreur d'allocation de mémoire pour les éléments de la liste\n");
        free(list);
        exit(1);
    }
    list->size = 0;
    list->capacity = LIST_SIZE_MAX;
    return list;
}
void displayData(struct Data* d) {
    if (d == NULL) {
        printf("Invalid Data\n");
        return;
    }

    switch (d->type) {
        case TYPE_INT:
            printf("Type: INT, Value: %d\n", d->value.iValue);
            break;
        case TYPE_FLOAT:
            printf("Type: FLOAT, Value: %.2f\n", d->value.fValue);
            break;
        case TYPE_CHAR:
            printf("Type: CHAR, Value: %c\n", d->value.cValue);
            break;
        default:
            printf("Unknown type\n");
    }
}
// Fonction pour ajouter un élément à la liste
void append(List* list, Data* d) {
    if (list->size == list->capacity) {
        list->capacity *= 2;
        list->memory = realloc(list->memory, list->capacity * sizeof(Data*));
        if (list->memory == NULL) {
            fprintf(stderr, "Erreur de redimensionnement de la liste\n");
            exit(1);
        }
    }
    list->memory[list->size] = d;
    list->size += 1;
}


void list_initialize(List* list ,Data* elements[],unsigned int size){
    list =create_list();
    for (unsigned int i = 0; i < size; ++i) {
        append(list,elements[i]);
    }
}

// Fonction pour afficher les éléments de la liste
void print_list(struct List* list) {
    for (unsigned int i = 0; i < list->size; i++) {
        Data* data = list->memory[i];
        printf("Index: %d, ",i);
        displayData(data);
        
    }
}

// Fonction pour initialiser un tuple
void tuple_initialize(struct tuple **t, Data* elements[], unsigned int size) {
    (*t)->size = size;
    (*t)->memory = malloc((*t)->size * sizeof(Data*));
    if ((*t)->memory == NULL) {
        fprintf(stderr, "Erreur d'allocation de mémoire pour le tuple\n");
        exit(1);
    }
    //copy 
    memcpy((*t)->memory, elements, size * sizeof(Data*));
}

// Fonction pour afficher une donnée

// Fonction pour afficher le contenu d'un tuple
void print_tuple(struct tuple *t) {
    for (unsigned int i = 0; i < t->size; i++) {
        printf("Index: %d, ", i);
        displayData(t->memory[i]);
    }
}

// Fonction pour libérer la mémoire de la liste
void free_list(List* list) {
    free(list->memory);
    free(list);
}
void free_tuple(struct tuple *t){
    free(t->memory);
    free(t);
}

int main() {
    struct Data dataInt = { TYPE_INT, .value.iValue = 42 };
    struct Data dataFloat = { TYPE_FLOAT, .value.fValue = 3.14 };
    struct Data dataChar = { TYPE_CHAR, .value.cValue = 'A' };

    struct tuple *t = malloc(sizeof(struct tuple));
    if (t == NULL) {
        fprintf(stderr, "Erreur d'allocation de mémoire pour le tuple\n");
        return EXIT_FAILURE;
    }
    t->name = "mytuple";

    Data *memory[] = { &dataInt, &dataFloat, &dataChar };
    tuple_initialize(&t, memory, sizeof(memory) / sizeof(memory[0]));

    print_tuple(t);

    struct List* list = create_list();
    list_initialize(list,memory,3);
    
    print_list(list);

    // Libération de la mémoire
    free_list(list);
   free_tuple(t);
    return 0;
}
