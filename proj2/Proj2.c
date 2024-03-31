#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*------------------CONSTANTES------------------------------------*/

#define REALLOC 500
#define MAX_CAR 65536

/*------------------ESTRUTURAS------------------------------------*/

typedef struct folder{
    char name[150];
    char *value;                /*CONTEUDO DAS SUB-DIRETORIAS*/
}FOLDER;

typedef struct tree{             
    FOLDER file;
    int elements;                     /*ARVORES -> SUB-DIRETORIAS*/
    struct tree **tree_children;             
}TREE;

typedef struct node{                     
    TREE data;                   /*LINKA AS ARVORES*/
    struct node* next_tree;    
}NODE;

/*------------------CONSTANTES------------------------------------*/

/* reallocList -> Recebe um  triplo pointer para uma lista e o novo numero de elementos que desejamos 
que o bloco de memoria da lista tenha como argumentos, caso nao seja possivel dar o realloc da print "No memory" */
int reallocList(char ***list, size_t elements){
    if((*list = (char **)realloc(*list, elements * sizeof(char *))) == NULL){
        printf("No memory\n");
        return 0;
    }
    return 1;
}

/* allocString -> Recebe um duplo pointer para uma string e o numero de elementos que desejamos 
que o bloco de memoria da lista tenha como argumentos, caso nao seja possivel dar o alloc da print "No memory" */
int allocString(char **string, size_t elements){
    if((*string = (char *)malloc(sizeof(char) * elements)) == NULL){
        printf("No memory\n");
        return 0;
    }
    return 1;
}

/* firstSpace -> Recebe um pointer para uma string como argumento e retorna o indice da posicao onde se encontra o primeiro espaco */
size_t firstSpace(char *sentence){
    size_t index = 0;
    for(index = 0; index < strlen(sentence); index++){
        if(sentence[index] == ' ')return index;
    }
    return 0;
}

/* lastSpace -> Recebe um pointer para uma string como argumento e retorna o indice da posicao onde se encontra o segundo espaco */
size_t lastSpace(char *sentence){
    int first = 0;
    size_t index = 0;
    first = firstSpace(sentence);
    for(index = first + 1; index < strlen(sentence); index++){
        if(sentence[index] == ' ')return index;
    }
    return index;
}

/* separateInput -> Recebe um pointer para uma string e retorna uma lista de strings composta por por todos os elementos
de sentence que estao separados por espacos, retorna tambem o numero de elementos de List por referencia */
char **separateInput(char *sentence, int *elements){
    size_t first;
    size_t last;
    int n_strings = 0, n_car = 0;
    size_t index = 0;
    char **list = NULL;      
    char string[MAX_CAR];
    first = firstSpace(sentence);
    last = lastSpace(sentence);
    list = (char **)malloc(sizeof(char *));
    if(list == NULL){
        printf("No memory\n");
        return NULL;
    }
    if(first == 0){
        for(index = 0; index < strlen(sentence); index++){
            string[n_car] = sentence[index];
            n_car++;
        }
        if(n_car != 0){
            string[n_car] = '\0';
            if(!reallocList(&list, n_strings + 1))return NULL;
            if(!allocString(&list[n_strings], strlen(string) + 1))return NULL;
            strcpy(list[n_strings], string);
            *elements = 1;
            return list;
        }
    }
    for(index = 0; index < first; index++){
        string[n_car] = sentence[index];
        n_car++;
    }
    if(n_car != 0){
        string[n_car] = '\0';
        if(!reallocList(&list, n_strings + 1))return NULL;
        if(!allocString(&list[n_strings], strlen(string) + 1))return NULL;
        strcpy(list[n_strings], string);
        n_strings++;
        n_car = 0;
    }

    for(index = first + 1; index < last; index++){
        string[n_car] = sentence[index];
        n_car++;
    }

    if(n_car != 0){
        string[n_car] = '\0';
        if(!reallocList(&list, n_strings + 1))return NULL;
        if(!allocString(&list[n_strings], strlen(string) + 1))return NULL;
        strcpy(list[n_strings], string);
        n_strings++;
        n_car = 0;
    }

    for(index = last + 1; index < strlen(sentence); index++){
        string[n_car] = sentence[index];
        n_car++;
    }

    if(n_car != 0){
        string[n_car] = '\0';
        if(!reallocList(&list, n_strings + 1))return NULL;
        if(!allocString(&list[n_strings], strlen(string) + 1))return NULL;
        strcpy(list[n_strings], string);
        n_strings++;
        n_car = 0;
    }

    *elements = n_strings;
    return list;
}

/* parsePath -> Recebe um path(caminho) como argumento e divide esse path nos seus componentes returnando uma lista 
composta por todos os seus componentes, retorna tambem o numero de elementos dessa lista por referencia(elements)*/
char **parsePath(char *path, int *elements){
    size_t index = 0;
    int n_car = 0, n_strings = 0;
    char **list = NULL, sub_path[200] = "\0"; 

    list = (char **)malloc(sizeof(char *));
    if(list == NULL){
        printf("No memory\n");
        return 0;
    }

    for(index = 0; index < strlen(path); index++){
        if(path[index] == '/' || path[index] == ' '){
            sub_path[n_car] = '\0';
            if(strlen(sub_path) != 0){
                if(!reallocList(&list, n_strings + 1))return NULL;
                if(!allocString(&list[n_strings], strlen(sub_path) + 1))return NULL;
                strcpy(list[n_strings], sub_path);
                n_strings++;
                n_car = 0;
            }
        }else{
            sub_path[n_car] = path[index];
            n_car++;
        }
    }

    sub_path[n_car] = '\0';
    if(strlen(sub_path) != 0){
        if(!reallocList(&list, n_strings + 1))return 0;
        if(!allocString(&list[n_strings], strlen(sub_path) + 1))return 0; 
        strcpy(list[n_strings], sub_path);
        n_strings++;
    }

    *elements = n_strings;
    return list;
}

/* free_list -> Recebe um duplo pointer para uma lista e o seu numero de elementos como argumentos, libertando o bloco de memoria associado a 
cada a elemento e por fim o bloco de memoria da lista em si*/
void free_list(void **list, size_t elements){
    size_t aux;
    for(aux = 0; aux < elements; aux++){
        free(list[aux]);
    }
    free(list);
}

/* createFolder -> Recebe um apontadar para on novo folder(conteudo de um caminho), o nome do folder(caminho) e o valor 
a ser guardado, cria o folder caso seja possivel se nao acusa "No memory"*/
int createFolder(FOLDER *new_folder, char *name, char *value){
    strcpy(new_folder->name, name);
    new_folder->value = NULL;
    if(value != NULL){
        if(!allocString(&new_folder->value, strlen(value) + 1))return 0;
        strcpy(new_folder->value, value);
    }
    return 1;
}

/* initTree -> Recebe um apontador para uma arvore(Diretoria principal) e folder(conteudo associado a esse caminho) 
como argumentos, inicializa uma arvore nova*/
void initTree(TREE *new_Tree, FOLDER new_file){
   new_Tree->file = new_file;
   new_Tree->elements = 0;
   new_Tree->tree_children = NULL;
}

/* addTreeToList ->  Recebe um duplo pointer para uma arvore e a raiz da arvore, linka a arvore as restantes e retorna
um pointer pora un node(ponteiro para arvore)*/
NODE *addTreeToList(NODE **head, TREE head_tree){
    
    NODE *aux = (NODE *)malloc(sizeof(NODE));
    
    if(aux == NULL){
        printf("No memory\n");
        return 0;
    }

    aux->data = head_tree;
    aux->next_tree = NULL;
    
    if(*head == NULL){
        *head = aux;
    }else{
        NODE *percorre = *head;
        while(percorre->next_tree != NULL)
            percorre = percorre->next_tree;
        percorre->next_tree = aux;
    }
    return aux;
}

/* addChildToTree -> Recebe 2 arvores como argumentos, linkando a new_tree a update_tree como child da ultima*/
int addChildToTree(TREE *update_tree, TREE new_tree){
    if(update_tree->elements == 0){
        update_tree->tree_children = (TREE **)malloc(sizeof(TREE *));
        if(update_tree->tree_children == NULL){
            printf("No Memory\n");
            return 0;
        }
    }else{
        update_tree->tree_children = (TREE **)realloc(update_tree->tree_children, (update_tree->elements + 1) * sizeof(TREE *));
        if(update_tree->tree_children == NULL){
            printf("No Memory\n");
            return 0;
        }
    }

    update_tree->elements++;
    update_tree->tree_children[update_tree->elements - 1] = (TREE *)malloc(sizeof(TREE));
    if(update_tree->tree_children[update_tree->elements - 1] == NULL){
        printf("No memory\n");
        return 0;
    }
    
    *(update_tree->tree_children[update_tree->elements - 1]) = new_tree;
    
    return 1;
}

/* createNewPath -> Recebe um duplo pointer para um node(conjunto de arvores), duplo pointer para string que contem o caminho sub_dividido,
o numero de elementos da string e o valor a ser guardado nesse caminho, linka a nova arvore as restantes arvores */
int createNewPath(NODE **head, char **path_list, int elements, char *value){
    int index;
    NODE *aux;
    TREE *update;
    TREE new_tree;
    FOLDER new_folder;

    if(elements == 1){
        createFolder(&new_folder, path_list[0], value);
    }else{
        createFolder(&new_folder, path_list[0], NULL);
    }
    initTree(&new_tree, new_folder);
    aux = addTreeToList(head, new_tree);
    if(aux == NULL)return 0;
    update = &aux->data;

    for(index = 1; index < elements; index++){
        if(index == elements - 1){
            createFolder(&new_folder, path_list[index], value);
        }else{
            createFolder(&new_folder, path_list[index], NULL);
        }
        initTree(&new_tree, new_folder);
        if(!addChildToTree(update, new_tree))return 0;
        update = update->tree_children[update->elements - 1];
    }
    return 1;
}

/* isFolderInChildren -> recebe um pointer para uma arvore e uma string,verifica se na arvore ha algum caminho com folder_name */
TREE *isFolderInChildren(TREE *main_tree, char *folder_name){
    int index = 0;
    for(index = 0; index < main_tree->elements; index++){
        if(strcmp(main_tree->tree_children[index]->file.name, folder_name) == 0)return main_tree->tree_children[index];
    }
    return NULL;
}

/* isFolderInList -> recebe um pointer para uma arvore e uma string como argumentos,verifica se no conjunto de arvores ha algum caminho com folder_name*/
NODE *isFolderInList(NODE *head, char *folder_name){
    NODE *aux = head;
    while(aux != NULL){
        if(strcmp(aux->data.file.name, folder_name) == 0)return aux;
        aux = aux->next_tree;
    }
    return NULL;
}

/* updatePath -> recebe um pointer para uma arvore, lista de strings, um inteiro que indica onde comecar a atualizar o caminho
  ,o numero de elementos da arvore e um pointer para string que eh o valor do caminho, atualiza o sub_caminho com o caminho e o seu valor*/
int updatePath(TREE *update_tree, char **path_list, int start_path, int elements, char *value){
    TREE *update = update_tree;
    TREE new_tree;
    FOLDER new_folder;
    int index;

    for(index = start_path; index < elements; index++){
        if(index == elements - 1){
            if(!createFolder(&new_folder, path_list[index], value))return 0;
        }else{
            if(!createFolder(&new_folder, path_list[index], NULL))return 0;
        }
        initTree(&new_tree, new_folder);
        if(!addChildToTree(update, new_tree))return 0;
        update = update->tree_children[update->elements - 1];
    }
    return 1;
}

/* updateTree -> Recebe um pointer para arvore, lista de strings que contem caminhos, o numero de elementos da arvore
, o sitio de comeco da arvore e o valor a ser associado a tal caminho, a funcao eh responsavel por por dar update a arvore mae*/
int updateTree(TREE *tree_in_list, char **path_list, int elements, int start_path, char *value){
    int index = 0;
    TREE *aux;

    if(start_path == elements){
        if(!allocString(&tree_in_list->file.value, strlen(value) + 1))return 0;
        strcpy(tree_in_list->file.value, value);
        return 1;
    }

    for(index = start_path; index < elements; index++){
        if((aux = isFolderInChildren(tree_in_list, path_list[index])) != NULL){
            return updateTree(aux, path_list ,elements, start_path + 1, value);
        }else{
            if(!updatePath(tree_in_list, path_list, start_path, elements, value))return 0;
            return 1;
        }
    }
    return 1;

}

/* updateList -> recebe um duplo pointer para um node(raiz de uma arvore), pointer para string que contem o caminho e outro pointer para string 
que contem o valor do caminho, a funcao e responsavel pela atualizacao da lista de arvores*/
int updateList(NODE **head, char *path, char *value){
    NODE *node_in_list = NULL;
    int elements = 0;
    char **path_list = parsePath(path, &elements);

    if((node_in_list = isFolderInList(*head, path_list[0])) == NULL){
        if(!createNewPath(head, path_list, elements, value))return 0;
    }else{
        if(!updateTree(&node_in_list->data, path_list, elements, 1, value))return 0;
    }

    free_list((void **)path_list, elements);
    return 1;

}

/* printTreeSiblings -> Recebe uma arvore e um pointer  para string que contem o nome da raiz como argumentos e da print
do nome de todos os filhos de tal raiz */
void printTreeSiblings(TREE tree_print, char *root_name){
    int index = 0;
    char *root_copy = NULL;
    if(strlen(root_name) != 0){
        root_copy = (char *)malloc(strlen(root_name) + 1);
        strcpy(root_copy, root_name);
    }
    
    if(tree_print.file.value != NULL){
        printf("/%s %s\n", root_name, tree_print.file.value);
    }
    for(index = 0; index < tree_print.elements; index++){
        strcat(root_name, "/");
        printTreeSiblings(*tree_print.tree_children[index], strcat(root_name, tree_print.tree_children[index]->file.name));
        strcpy(root_name, root_copy);
    }
    
    free(root_copy);

}

/* printTree -> Recebe uma arvore como argumento e da print em toda a arvore */
void printTree(TREE tree_print){
    printTreeSiblings(tree_print, tree_print.file.name);
}

/* printList -> Recebe um pointer para um node(Raiz da arvore) e da print em todas as arvores pertencentes a lista dde arvores*/
void printList(NODE *head){
    NODE *aux = head;

    while(aux != NULL){
        printTree(aux->data);
        aux = aux->next_tree;
    }
}

/* freeSiblings -> Recebe um duplo pointer para para uma parte de uma arvore e o seu numero de elementos como argumentos
e liberta toda a memoria associada a essa arvore e os seus filhos*/
void freeSiblings(TREE **children, int elements){
    int index = 0;
    
    if(children == NULL)return;
    for(index = 0; index < elements; index++){
        if(children[index]->file.value != NULL)
            free(children[index]->file.value);
        freeSiblings(children[index]->tree_children, children[index]->elements);
        free(children[index]);
    }

    free(children);
}

/* free_Tree -> Recebe um pointer para uma arvore e liberta toda a memora associada a esta desde a sua raiz*/
void free_Tree(TREE *data){
    if(data == NULL)return;
    if(data->file.value != NULL)
        free(data->file.value);
    freeSiblings(data->tree_children, data->elements);
}

/* free_LinkedList -> Recebe um pointer para um node(estrutura que contem cada arvore) e liberta toda a memoria 
associada ao conjunto de todas as arvore*/
void free_LinkedList(NODE *head){
    NODE *aux = head;
    while(head != NULL){
        free_Tree(&head->data);
        head = head->next_tree;
        free(aux);
        aux = head;
    }
}

/* set -> recebe um duplo pointer para um node, um pointer para string que contem o caminho e outro pointer para string que contem 
o valor a ser associado,atualiza a lista com o novo caminho e o seu respetivo valor*/
int set(NODE **head, char *path, char *value){
    if(!updateList(head, path, value))return 0;
    return 1;
}

/* searchTree -> recebe uma arvore como argumento e um pointer para string e verifica se em tal arvore existe certo caminho */
TREE *searchTree(TREE tree_to_search, char *folder_name){
    TREE *next_tree;
    next_tree = isFolderInChildren(&tree_to_search, folder_name);
    if(next_tree == NULL){
        printf("not found\n");
        return NULL;
    }
    return next_tree;
}

/* find -> recebe um pointer para um node e um pointer para string que contem o caminho e retorna o valor desse caminho 
caso ele exista se nao retorna erro*/
int find(NODE *head, char *path){
    int index = 0;
    int elements = 0;
    TREE *next_tree = NULL;
    NODE *node_in_list = NULL;
    char **path_list = parsePath(path, &elements);
    
    if(path_list == NULL)return 0;

    node_in_list = isFolderInList(head, path_list[0]);
    if(node_in_list == NULL){
        printf("not found\n");
        free_list((void **)path_list, elements);
        return 1;
    }
    if(elements == 1){
        if(node_in_list->data.file.value == NULL){
            printf("no data\n");
            free_list((void **)path_list, elements);
            return 1;
        }else{
            printf("%s\n", node_in_list->data.file.value);
            free_list((void **)path_list, elements);
            return 1;
        }
    }
    next_tree = &node_in_list->data;
    index = 1;
    while(index < elements){
        if((next_tree = isFolderInChildren(next_tree, path_list[index])) != NULL){
            index++;
        }else{
            printf("not found\n");
            free_list((void**)path_list, elements);
            return 1;
        }
    }
    if(next_tree->file.value != NULL){
        printf("%s\n", next_tree->file.value);
        
    }else{
        printf("no data\n");
    }
    free_list((void**)path_list, elements);
    return 1;
}

/* alfaSortList -> recebe uma lista de strings e o seu numero de elementos como argumentos e ordena a lista por 
ordem alfabetica*/
int alfaSortList(char **list, int elements){
    char *aux = NULL;
    int i,j;
    for(i = 0; i < elements; i++){
        for(j = i; j < elements; j++){
            if(strcmp(list[j], list[i]) > 0){
                aux = (char *)malloc(strlen(list[i]) + 1);
                if(aux == NULL)return 0;
                strcpy(aux, list[i]);
                if(!allocString(&list[i], strlen(list[j]) + 1))return 0;
                strcpy(list[i], list[j]);
                if(!allocString(&list[j], strlen(aux) + 1))return 0;
                strcpy(list[j], aux);
                free(aux);
            }
        }
    }
    return 1;
}

/* listTreeComponents -> Recebe uma arvore com argumento e da print dos componentes da arvore por ordem alfabetica depois 
de os ordenar */
int listTreeComponents(TREE tree_in_list){
    int index = 0;
    char **StringList = (char **)malloc(sizeof(char *) * tree_in_list.elements);
    if(StringList == NULL)return 0;
    for(index = 0; index < tree_in_list.elements; index++){
        StringList[index] = (char *)malloc(strlen(tree_in_list.tree_children[index]->file.name) + 1);
        if(StringList[index] == NULL)return 0;
        strcpy(StringList[index], tree_in_list.tree_children[index]->file.name);
    }

    if(!alfaSortList(StringList, tree_in_list.elements)){
        free_list((void **)StringList, tree_in_list.elements);
        return 0;
    }

    for(index = 0; index < tree_in_list.elements;index++){
        printf("%s\n", StringList[index]);
    }
    free_list((void **)StringList, tree_in_list.elements);
    return 1;
}

/* listRootComponents -> Recebe um pointer para um node(raiz da arvore) e da print a todas as raizes de todas as arvores existentes
depois de as ordenar por ordem alfabetica*/
int listRootComponents(NODE *head){
    NODE *aux = head;
    int index = 0;
    int number = 0;
    char **Stringlist;
    Stringlist = (char **)malloc(sizeof(char *));
    if(Stringlist == 0)return 0;

    while(aux != NULL){
        Stringlist = (char **)realloc(Stringlist, (number + 1) * sizeof(char *));
        Stringlist[number] = (char *)malloc(strlen(aux->data.file.name) + 1);
        strcpy(Stringlist[number], aux->data.file.name);
        number++;
        aux = aux->next_tree;
    }
    if(!alfaSortList(Stringlist, number)){
        free_list((void**)Stringlist, number);
        return 0;
    }
    for(index = 0; index < number; index++){
        printf("%s\n", Stringlist[index]);
    }
    free_list((void**)Stringlist, number);
    return 1;
}

/* listComponents -> Recebe um pointer para um node(raiz de uma arvore) e um pointer para string que contem um caminho
,a funcao da print em todos os componentes(diretorio) caso path nao seja string vazia caso contrario da print em todas as raizes*/
int listComponents(NODE *head, char *path){
    int elements = 0, index = 0;
    TREE *next_tree = NULL;
    NODE *node_in_list = NULL;
    char **path_list = parsePath(path, &elements);

    if(strlen(path) == 0){
        if(!listRootComponents(head)){
            free_list((void**)path_list, elements);
            return 0;
        }
    }else{
        node_in_list = isFolderInList(head, path_list[0]);
        if(node_in_list == NULL){
            printf("not found\n");
            free_list((void**)path_list, elements);
            return 1;
        }else{
            next_tree = &node_in_list->data;
            index = 1;
            while(index < elements){
                if((next_tree = isFolderInChildren(next_tree, path_list[index])) != NULL){
                    index++;
                }else{
                    printf("not found\n");
                    free_list((void**)path_list, elements);
                    return 1;
                }
            }
            if(!listTreeComponents(*next_tree)){
                free_list((void**)path_list, elements);
                return 0;
            }
        }
    }
    free_list((void**)path_list, elements);
    return 1;
}   

/* searchTreeValue -> Recebe uma arvore e outro pointer para string com o valor como argumentos,e procura na arvore o caminho que tenha esse valor,
da print no caminho e passa o caminho por referencia em root_name*/
int searchTreeValue(TREE search_tree, char *root_name, char *value){
    int index = 0;
    char *root_copy = NULL;
    if(!allocString(&root_copy, strlen(root_name) + 1))return -1;
    strcpy(root_copy, root_name);

    if(search_tree.file.value != NULL){
        if(strcmp(search_tree.file.value, value) == 0){
            printf("/%s\n", root_name);
            free(root_copy);
            return 1;
        }
    }
    for(index = 0; index < search_tree.elements; index++){
        strcat(root_name, "/");
        if(searchTreeValue(*search_tree.tree_children[index], strcat(root_name, search_tree.tree_children[index]->file.name),value) == 1){
            free(root_copy);
            return 1;
        }
        strcpy(root_name, root_copy);
    }

    free(root_copy);
    return 0;
}

/* search -> recebe um pointer para um node(raiz de uma arvore) e um pointer para string que contem o valor,e vai procurar por todas
as arvores qual delas o contem dando print no primeiro caminho que encontrar,caso nao encontre da erro*/
int search(NODE *head, char *value){
    NODE *aux = head;
    char root[MAX_CAR];
    int keep;
    while(aux != NULL){
        strcpy(root, aux->data.file.name);
        keep = searchTreeValue(aux->data, root, value);
        if(keep == 1)return 1;
        if(keep == -1)return 0;
        aux = aux->next_tree;
    }
    printf("not found\n");
    return 1;
}

/* delete_children -> recebe um pointer para uma arvore e trata de eliminar a arvore e dar free da memoria ocupada pela mesma*/
void delete_children(TREE *delete_tree){
    int index = 0;

    if(delete_tree->file.value != NULL)free(delete_tree->file.value);
    
    for(index = 0; index < delete_tree->elements; index++){
        delete_children(delete_tree->tree_children[index]);
        free(delete_tree->tree_children[index]);
    }
    free(delete_tree->tree_children);
    delete_tree->elements = 0;
    delete_tree->tree_children = NULL;
}

/* target_delete -> recebe um pointer para uma arvore e um pointer para string que contem um caminho e da free na tree children
que contenha esse caminho*/
void target_delete(TREE *destiny, char *folder_name){
    int index = 0;
    int variable;
    while(strcmp(destiny->tree_children[index]->file.name, folder_name) != 0)index++;

    free(destiny->tree_children[index]);
    for(variable = index; variable < destiny->elements - 1; variable++){
        destiny->tree_children[variable] = destiny->tree_children[variable+1];
    }
    destiny->elements--;
}

/* delete_node -> recebe um duplo pointer para um node e outro pointer para outro node,e da delete do node "delete" da lista de nodes
libertando toda a memoria associada ao node eliminado*/
void delete_node(NODE **head, NODE *delete){
    NODE *aux = *head;
    if((*head) == delete){
        *head = delete->next_tree;
        delete_children(&delete->data);
        free(delete);
        return;
    }
    while(aux->next_tree != delete){
        aux = aux->next_tree;
    }
    aux->next_tree = delete->next_tree;
    delete_children(&delete->data);
    free(delete);

}

/* delete -> recebe um duplo pointer para um node e um pointer para string que contem um caminho e apaga o caminho e todos os outros
caminhos que o tenham como sub_caminho,se for invocado sem path elimina todas as arvores*/
int delete(NODE **head, char *path){
    int index = 1;
    int elements = 0;
    char **path_list;
    NODE *node_in_list = NULL;
    TREE *previous_tree = NULL;
    TREE *next_tree = NULL;
    if(strlen(path) == 0){
        free_LinkedList(*head);
        return 1;
    }
    path_list = parsePath(path, &elements);
    if(path_list == NULL)return 0;

    if((node_in_list = isFolderInList(*head, path_list[0])) == NULL){
        printf("not found\n");
        free_list((void**) path_list, elements);
        return 1;
    }
    if(elements == 1){
        delete_node(head, node_in_list);
        free_list((void**) path_list, elements);
        return 1;
    }
    previous_tree = &node_in_list->data;
    next_tree = &node_in_list->data;
    while(index < elements){
        if((next_tree = isFolderInChildren(next_tree, path_list[index])) == NULL){
            printf("not found\n");
            free_list((void**)path_list, elements);
            return 1;
        }
        if(index == elements - 2)
            previous_tree = next_tree;
        index++;
    }

    delete_children(next_tree);
    target_delete(previous_tree, path_list[elements - 1]);
    free_list((void**)path_list, elements);
    return 1;
}

/* quit -> recebe um pointer para head mata o programa libertando a memoria do mesmo*/
void quit(NODE *head){
    free_LinkedList(head);
    exit(EXIT_SUCCESS);
}

/* help -> Imprime todos os comandos disponiveis e as suas funcoes*/
void help(void){
    printf("help: Imprime os comandos disponíveis.\n");
    printf("quit: Termina o programa.\n");
    printf("set: Adiciona ou modifica o valor a armazenar.\n");
    printf("print: Imprime todos os caminhos e valores.\n");
    printf("find: Imprime o valor armazenado.\n");
    printf("list: Lista todos os componentes imediatos de um sub-caminho.\n");
    printf("search: Procura o caminho dado um valor.\n");
    printf("delete: Apaga um caminho e todos os subcaminhos.\n");
}

/* evaluateOption -> recebe um duplo pointer para un node, uma lista de strings e o  numero de elementos da listade arvores,
consoante seja a string guardada em list a funcao a ser executada eh diferente */
int evaluateOption(NODE **head, char **list, int elements){
    if(strcmp(list[0], "help") == 0){
        help();
        return 1;
    }
    if(strcmp(list[0], "quit") == 0){
        free_list((void**)list, elements);
        quit(*head);
    }
    if(strcmp(list[0], "set") == 0){
        if (strcmp(list[2],"")){
            return set(head, list[1], "");
        }
        else{
        return set(head, list[1], list[2]);
        }
    }   
    if(strcmp(list[0], "print") == 0){
        printList(*head);
        return 1;
    }
    if(strcmp(list[0], "find") == 0){
        return find(*head, list[1]);
    }
    if(strcmp(list[0], "list") == 0){
        if(elements == 1){
            return listComponents(*head, "");
        }else{
            return listComponents(*head, list[1]);
        }

    }
    if(strcmp(list[0], "search") == 0){
        
        if(elements == 2)return search(*head, list[1]);
        return search(*head, strcat(strcat(list[1], " "), list[2]));
    }
    if(strcmp(list[0], "delete") == 0){
        if(elements == 1){
            return delete(head, "");
        }else{
           return delete(head, list[1]);
        }
    }
    return 1;
}

/* main -> funcao responsavel pelo funcionamento integral do programa*/
int main(){
    char string[MAX_CAR];
    int elements = 0;
    char **list = NULL;
    NODE *head = NULL;
   
    while(1){
        fgets(string, MAX_CAR, stdin);
        string[strcspn(string, "\t\r\n")] = '\0';
        list = separateInput(string, &elements);
        if(list == NULL){
            free_LinkedList(head);
            exit(EXIT_SUCCESS);
        }
        if(!evaluateOption(&head, list, elements)){
            free_LinkedList(head);
            exit(EXIT_SUCCESS);
        }
        free_list((void **)list, elements);
    }
    return 1;
}