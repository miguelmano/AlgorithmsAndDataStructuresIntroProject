#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

/*------------------CONSTANTES------------------------------------*/

#define MAXTAREFAS 10000
#define MAXDESC 100
#define MAXUSER 100
#define MAXATIVIDADE 100
#define NUMMAXUSERS 50
#define NUMMAXATIVIDADES 10
#define MAXSTRING 100

/*----------------VARIAVEIS GLOBAIS----------------------*/
typedef struct tarefa{
    long id;
    char desc[MAXDESC];
    char user[MAXUSER];
    char ativ[MAXATIVIDADE];
    int duracao,comeco;
}Tarefa;
typedef struct grupo{
    char *users[10];
    char nome_grupo[MAXUSER];
}GRUPO;
int n_tarefa=0,tempo_atual=0,n_users=0,n_atividades=3;  /*n_tarefas->numero total de tarefas,tempo_atual->tempo atual no programa,n_users=0->numero total de users*/
Tarefa tasks[MAXTAREFAS];               /*n_atividades->numero total de atividades,tasks->tabela de tarefas*/
char usernames[NUMMAXUSERS][MAXUSER]; /*tabela que guardaa os usernames*/
char atividades[NUMMAXATIVIDADES][MAXATIVIDADE]; /*tabela que guarda as atividades existentes*/


/*-------------------FUNCOES AUXILIARES---------------*/

/*retira a primeira letra da string,usado para escolher o comando a usar*/
void retira_letra(char *frase,int *index,char *character){
    size_t tamanho;
    int str;
    for (str=0,tamanho=0;tamanho < strlen(frase);tamanho++,str++){
        if (frase[str]>='a' && frase[str]<='z'){
            *index=str;
            *character=frase[str];
            return;
        } 
    }
}

/*usado para popular a instrucao sem o primeiro carater*/
void popula_instrucao(char *frase,char*destiny,int index){
    int str,i;
    size_t tamanho;
    for (tamanho=(size_t)index,i=index,str=0;tamanho<strlen(frase);i++,str++,tamanho++){
        destiny[str]=frase[i];
    }
    destiny[str]='\0';
    
}

/*verifica se a string e vazia;0-> se for,1->se nao for*/
int isPopulated(char *frase){
    size_t tamanho;
    int i;
    for(tamanho = 0, i = 0; tamanho < strlen(frase); tamanho++, i++){
        if((frase[i] >= 'a' && frase[i] <= 'z') || (frase[i] >='A' && frase[i] <= 'Z') || ((frase[i] >='0' && frase[i] <= '9')))return 1;
    }
    return 0;
}

/*usada pela funcao filtra_m para popular a instrucao */
void popula_frase(char *frase, char *destiny){
    int index;
    size_t tamanho;
    for(tamanho = 0, index = 0; tamanho < strlen(frase); tamanho++, index++){
        destiny[index]=frase[index];
    }
    destiny[index]='\0';
}

/*verifica se ha uma tarefa com o id*/
int check_id(int id){
    int indexTab;
    for ( indexTab =0;indexTab<n_tarefa;indexTab++){
        if (tasks[indexTab].id==id) return 1;
    }
    return 0;
}

/*verifica se o user existe*/
int check_user(char* user){
    int index;
    for(index=0;index<n_users;index++){
        if(!strcmp(usernames[index],user)){
            return 1;
        }

    }
    return 0;
}

/*verifica se atividade existe*/
int check_atividade(char *ativ){
    int index;
    for(index=0;index<n_atividades;index++){
        if(!strcmp(atividades[index],ativ))return 1;
    }
    return 0;
}

/*guarda o endereco das tarefas numa outra tabela*/
void fill_addresses(Tarefa tab[MAXTAREFAS],Tarefa *adress_table[MAXTAREFAS],int counter){
    int address;
    for(address = 0;address < counter;address++){
        adress_table[address] = &tab[address];
    }
}

/*algoritmo quicksort aplicado a strings*/
void quicksort_str(Tarefa *task_address[MAXTAREFAS], int comeco, int len){
    Tarefa *aux;
    int i,j;
    for(i = comeco; i < len; i++){
        for(j = i; j < len; j++){
            if(strcmp((*task_address[i]).desc,(*task_address[j]).desc)>0){
                aux = task_address[i];
                task_address[i] = task_address[j];
                task_address[j] = aux;
            }
        }
    }
}

/*algoritmo quicksort aplicado a digitos*/
void quicksort(Tarefa *task_address[MAXTAREFAS],int first,int last){
    int i, j, pivot;
    Tarefa *temp;
    if(first<last){
        pivot=first;
        i=first;
        j=last;
        while(i<j){
            while((*task_address[i]).comeco <= (*task_address[pivot]).comeco && i<last)
                i++;
            while((*task_address[j]).comeco > (*task_address[pivot]).comeco)    
                j--;
            if(i < j){
                temp=task_address[i];
                task_address[i]=task_address[j];
                task_address[j]=temp;
            }
        }
    temp=task_address[pivot];
    task_address[pivot]=task_address[j];
    task_address[j]=temp;
    quicksort(task_address,first,j-1);
    quicksort(task_address,j+1,last);
    }
}

/*filtra os numeros presentes numa string de modo a ficarmos com os digitos introduzidos pelo user*/
int *filtra_num(char* user_input, int * numeros){
    int numFlag = 0;
    int negFlag = 0;
    int counter = 1;
    int * listNum = (int* ) malloc(counter * sizeof(int));
    int numero = 0;
    int index;
    size_t tamanho;

    for(tamanho = 0, index = 0; tamanho < strlen(user_input); tamanho++, index++){
        if(user_input[index] == '-'){
            negFlag = 1;
        }
        if(user_input[index] >= '0' && user_input[index] <= '9'){
            numFlag = 1;
            numero = numero * 10 + user_input[index] - '0';
        }else{
            if(user_input[index] == ' '){
                if(numFlag){
                    numFlag = 0;
                    *numeros += 1;
                    if(negFlag){
                        listNum[counter - 1] = -1 * numero;
                    }else{
                        listNum[counter - 1] = numero;
                    }
                    counter++;
                    listNum = realloc(listNum, counter * sizeof(int *));
                    numero = 0;
                }
                negFlag = 0;

            }
        }
    }
    if(numero > 0){
        *numeros += 1;
        listNum[counter - 1] = numero;
    }

    return listNum;
}

/*filtra os carateres numa string de modo a ficarmos so com carateres introduzidos pelo user*/
char **filtra_frase(char* user_input, int * numeroFrases){
    int counter = 1;
    char **listStrings = (char** ) malloc( counter * sizeof(char *));
    int str_index = 0;
    int index;
    size_t tamanho = 0;
    listStrings[counter - 1] = malloc(MAXDESC * sizeof(char));

    
    for(tamanho = 0, index = 0; tamanho < strlen(user_input); tamanho++, index++){
        if((user_input[index] >= 'a' && user_input[index] <= 'z') || (user_input[index] >= 'A' && user_input[index] <= 'Z')){
            listStrings[counter - 1][str_index] = user_input[index];
            str_index++;
        }else{
            if(user_input[index] == ' '){
                if(str_index > 0){
                    *numeroFrases += 1;
                    listStrings[counter - 1][str_index] = '\0';
                    counter++;
                    listStrings = realloc(listStrings, counter * sizeof(char *));
                    listStrings[counter - 1] = malloc(MAXDESC * sizeof(char));
                    str_index = 0;
                }
            }
        }
    }
    if(str_index > 0){
        *numeroFrases += 1;
        listStrings[counter - 1][str_index] = '\0';
    }

    return listStrings;
}

/*cria lista com os elementos da tabela inserida*/
int *cria_lista(Tarefa *tab[MAXTAREFAS],int counter,int *dimensao){
    int *lista_rep=(int *)malloc(counter*sizeof(int));
    int elemento=(*tab[0]).comeco, indice_lista=0, contador = 1;
    int index;
    for(index=1;index<counter;index++){
        if ((*tab[index]).comeco != elemento){
            lista_rep[indice_lista]=contador;
            contador = 1;
            elemento=(*tab[index]).comeco;
            indice_lista+=1;
            *dimensao+=1;
        }else{
            contador+=1;
        }
    }
    lista_rep[indice_lista] = contador;
    *dimensao+=1;
    return lista_rep;
}

/*popula a tabela com os pointers duma outra tabela*/
void popula_tab_pointer(Tarefa* address_table[MAXTAREFAS], Tarefa* pointer_to_tab[MAXTAREFAS], int inicio, int fim, int tabuleiro){
    int address;
    for(address = inicio; address < fim; address++){
        address_table[address] = pointer_to_tab[tabuleiro];
        tabuleiro += 1;
    }
}

/*-----------------------FUNCOES PRINCIPAIS----------------------*/

/*funcao responsavel por adicionar uma nova tarefa ao sistema*/
void t(int dur,char* descricao){
    tasks[n_tarefa].id=n_tarefa+1;
    strcpy(tasks[n_tarefa].desc,descricao);
    strcpy(tasks[n_tarefa].ativ,"TO DO");
    tasks[n_tarefa].duracao=dur;
    tasks[n_tarefa].comeco=0;
    n_tarefa+=1;
    printf("task %d\n",n_tarefa);
}

/*funcao responsavel por listar as tarefas*/
void l(int id){
    Tarefa* pointer_to_tab[MAXTAREFAS];
    int i,index;
    if (id==0){
        fill_addresses(tasks,pointer_to_tab,n_tarefa+1);
        quicksort_str(pointer_to_tab,0,n_tarefa);
        for(index = 0;index<n_tarefa;index++){
            printf("%ld %s #%d %s \n",(*pointer_to_tab[index]).id,(*pointer_to_tab[index]).ativ,(*pointer_to_tab[index]).duracao,(*pointer_to_tab[index]).desc);
        }
    }
    else{
        for(i=0;i<=n_tarefa;i++){
            if (tasks[i].id==id){
                printf("%ld %s %d %s \n",tasks[i].id,tasks[i].ativ,tasks[i].duracao,tasks[i].desc);
            }
        }

    }
}

/*funcao responsavel por avanÃ§ar o tempo do sistema*/
void n(int dur){
    if(dur==0){
        printf("%d\n",tempo_atual);
    }
    else{
        tempo_atual=tempo_atual+dur;
        printf("%d\n",tempo_atual);
    }
}

/*funcao responsavel por adicionar um utilizador ou lista todos os utilizadores*/
void u(char *user){
    int i;
    if (!strcmp(user,"")){
        for (i = 0;i < n_users;i++){
            printf("%s\n",usernames[i]);
        }
        return;
    }else{
        strcpy(usernames[n_users],user);
        n_users+=1;
        return;
    }
}

/*funcao responsavel por mover uma tarefa de uma atividade para outra*/
void m(int id, char *user,char *ativ){
    int n_task,slack,gasto,i;
    for (i=0;i<n_tarefa;i++){
        if (tasks[i].id==id){
            n_task=i;
        }
    }
    if (!strcmp("DONE",ativ)){
        if((tasks[n_task].comeco==0) && !strcmp(tasks[n_task].ativ,"TO DO")){
            gasto=0;
            slack=gasto-tasks[n_task].duracao;
            strcpy(tasks[n_task].ativ,"DONE");
            printf("duration=%d slack=%d\n",gasto,slack);

        }
        else{
            gasto=tempo_atual-tasks[n_task].comeco;
            slack=gasto-tasks[n_task].duracao;
            strcpy(tasks[n_task].ativ,"DONE");
            printf("duration=%d slack=%d\n",gasto,slack);

        }
    }
    else{
        strcpy(tasks[n_task].user,user);
        strcpy(tasks[n_task].ativ,ativ);
        if (!strcmp("IN PROGRESS",ativ)){
            tasks[n_task].comeco= tempo_atual;
        }
    }
}

/*funcao responsavel por listar todas as tarefas que estejam numa dada atividade*/
void d(char* atividade){
    Tarefa tab[MAXTAREFAS];
    Tarefa *pointer_to_tab[MAXTAREFAS];
    Tarefa *pointer_auxiliar[MAXTAREFAS];
    int *lista_rep;
    int tab_index = 0;
    int address,i,counter=0,dimensao=0,index_lista=0,index_tab=0;
    for (i=0;i<=n_tarefa;i++){
        if(!strcmp(tasks[i].ativ,atividade)){
            tab[tab_index]=tasks[i];
            counter++;
            tab_index++;
        }
    }

    if(counter==0)return;
    fill_addresses(tab,pointer_to_tab,counter);          
    quicksort(pointer_to_tab, 0, counter - 1);
    lista_rep=cria_lista(pointer_to_tab,counter,&dimensao);
    while (index_lista<dimensao){
        popula_tab_pointer(pointer_auxiliar, pointer_to_tab,0,lista_rep[index_lista], index_tab);
        quicksort_str(pointer_auxiliar,0,lista_rep[index_lista]);
        for (address=0;address<lista_rep[index_lista];address++){
            printf("%ld %d %s \n",(*pointer_auxiliar[address]).id,(*pointer_auxiliar[address]).comeco,(*pointer_auxiliar[address]).desc);
        }
        index_tab+=lista_rep[index_lista];
        index_lista += 1;
    }
    free(lista_rep);
}

/*funcao responsavel por adicionar uma atividade ou lista todas */
void a(char *atividade){
    int i;
    if(!strcmp(atividade,"")){
        for (i = 0;i<n_atividades;i++){
            printf("%s\n",atividades[i]);
        }
    }
    else{
        strcpy(atividades[n_atividades],atividade);
        n_atividades++;
    
    }
}

/*funcao responsavel por filtrar o input do user uma vez que sabemos que o comando escolhido foi o t*/
void filtra_t(char *frase){
    char string[MAXDESC];
    int duracao;
    int index;
    

    sscanf(frase, "%d %s", &duracao, string);
    
    if(n_tarefa==10000){
        printf("too many tasks\n");
        return ;
    }

    for(index=0;index<n_tarefa;index++){
        if(!strcmp(tasks[index].desc,string)){
            printf("duplicate description\n");
            return ;
        }
    }
    if(duracao<=0){
        printf("invalid duration\n");
        return ;
    }

    t(duracao, string);
    return ;

}

/*funcao responsavel por filtrar o input do user uma vez que sabemos que o comando escolhido foi o l*/
void filtra_l(char *frase){
    int *lista_num,numeros=0;
    int index;
    lista_num=filtra_num(frase,&numeros);
    if (numeros==0){
        l(0);
        return ;
    }
    else{
        for(index =0;index<numeros;index++){
            if (!check_id(lista_num[index])){
                printf("%d: no such task\n",lista_num[index]);
                lista_num[index]=0;
            }
        }
    }
    for (index =0 ;index<numeros;index++){
        if(lista_num[index]>0){
            l(lista_num[index]);
        }
    }
    free(lista_num);
    return ;

}

/*funcao responsavel por filtrar o input do user uma vez que sabemos que o comando escolhido foi o n*/
void filtra_n(char *frase){
    float numero;
    sscanf(frase,"%f",&numero);
    if(numero-(int)numero>0 || numero<0){
        printf("invalid time\n");
        return ;
    }
    n(numero);
    return ;
}

/*funcao responsavel por filtrar o input do user uma vez que sabemos que o comando escolhido foi o u*/
void filtra_u(char *frase){
    char user[MAXUSER];
    
    if(!isPopulated(frase)){
        u("");
        return;
    }

    sscanf(frase, "%s", user);

    if (check_user(user)){
        printf("user already exists\n");
        return ;
    }
    if (n_users==50){
        printf("too many users\n");
        return ;
    }

    u(user);
    return ;
}

/*funcao responsavel por filtrar o input do user uma vez que sabemos que o comando escolhido foi o m*/
void filtra_m(char *frase){
    int id;
    char ativ[MAXATIVIDADE],user[MAXUSER];

    sscanf(frase, "%d %s %s", &id, user, ativ);
    
    
    if(!check_id(id)){
        printf("no such task\n");
        return ;
    }

    if(!strcmp(ativ,"TO DO") && strcmp(tasks[id].ativ,"TO DO")){
        printf("task already started\n");
        return ;
    }
    if(!check_user(user)){
        printf("no such user\n");
        return ;
    }
    if(!check_atividade(ativ)){
        printf("no such activity\n");
        return ;
    }
    m(id,user,ativ);
    return ;
}

/*funcao responsavel por filtrar o input do user uma vez que sabemos que o comando escolhido foi o d*/
void filtra_d(char *frase){
    char ativ[MAXATIVIDADE];
    sscanf(frase, "%s",ativ);
    
    if (!check_atividade(ativ)){
        printf("no such activity\n");
        return ;
    }

    d(ativ);
    return ;
}

/*funcao responsavel por filtrar o input do user uma vez que sabemos que o comando escolhido foi o a*/
void filtra_a(char *frase){
    char ativ[MAXATIVIDADE];
    int index_atividade=0;
    
    if(!isPopulated(frase)){
        a("");
        return;
    }

    sscanf(frase, "%s", ativ);

    if (check_atividade(ativ)){
        printf("duplicate activity\n");
        return ;
    }
    while (ativ[index_atividade] != '\0'){
        if(ativ[index_atividade]>='a' && ativ[index_atividade]<='z'){
            printf("invalid description\n");
            return ;
        }
        index_atividade++;
    }
    if (n_atividades==10){
        printf("too many activities\n");
        return ;
    }
    a(ativ);
    return ;
}

int *filtra_users(char* user_input, int * numeros){
    int numFlag = 0;
    int counter = 1;
    char user[MAXUSER];
    char * listNum = (char* ) malloc(counter * sizeof(char));
    int numero = 0;
    int index,index2, index_users=0;
    size_t tamanho;

    for(tamanho = 0, index = 0; tamanho < strlen(user_input); tamanho++, index++){
        if((user_input[index] >= 'A' && user_input[index] <= 'Z') || (user_input[index] >= 'a' && user_input[index] <= 'z') ){
            numFlag = 1;
            user[index_users]=user_input[index];
            index_users++;
        }else{
            if(user_input[index] == ' '){
                if(numFlag){
                    user[index_users]='\0';
                    numFlag = 0;
                    *numeros += 1;
                    listNum[counter - 1] = user;
                    counter++;
                    listNum = realloc(listNum, counter * sizeof(char *));

                    for(index2 = 0; i<strlen(user); i++){
                        user[i] = "";
                    }
                }
            }
        }
    }
    if(numero > 0){
        *numeros += 1;
        listNum[counter - 1] = numero;
    }

    return listNum;
}
void filtra_g(char *frase){
    int users=0, index0;
    Grupo users;
    char *lista_users;
    lista_users=filtra_users(frase, &users);
    if (check_user(lista_users[0])){
        printf("user already exists\n");
        return ;
    }
    if (n_users==50){
        printf("too many users\n");
        return ;
    }
    users->nome_grupo=lista_users[0];
    for (index=1; index < lenght(lista_users); index ++){
        users->users[index-1]=lista_users[index];
    }
    u(users);


}

/*funcao responsavel pelo funcionamento do programa,estando sempre sempre a ativo uma vez ligado a nao ser que o comando q seja executado*/
/*recebe o input do utilizador*/
int main(){
    int index;
    char string[MAXSTRING],option,instrucao[MAXSTRING];
    strcpy(atividades[0],"TO DO");
    strcpy(atividades[1],"IN PROGRESS");
    strcpy(atividades[2],"DONE");

    while (1){
        fgets(string,MAXSTRING,stdin);
        retira_letra(string,&index,&option);
        popula_instrucao(string,instrucao,index+1);
        switch (option)
        {
        case 'q':
            return 0;
            break;
        case 't':
            filtra_t(instrucao);
            break;
        case 'l':
            filtra_l(instrucao);
            break;
        case 'n':
            filtra_n(instrucao);
            break;
        case 'u':
            filtra_u(instrucao);
            break;
        case 'm':
            filtra_m(instrucao);
            break;
        case 'd':
            filtra_d(instrucao);
            break;
        case 'a':
            filtra_a(instrucao);
            break;
        case 'g':
            filtra_g(instrucao);
            break;
        default:
            break;
        }
    }
    return 0;
}