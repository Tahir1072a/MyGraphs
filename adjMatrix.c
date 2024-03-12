#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct LinkedList{
    int id;
    struct LinkedList* next;
} LinkedList;

LinkedList* createLinkedList(int id){
    LinkedList* head = (LinkedList*) malloc(sizeof (LinkedList));
    if(head == NULL) exit(1);

    head->id = id;
    head->next = NULL;

    return head;
};

void enQueue(int id, LinkedList* head){
    if(head == NULL){
        head = createLinkedList(id);
        return;
    }

    LinkedList* temp = (LinkedList*) malloc(sizeof (LinkedList));
    if(temp == NULL) exit(1);

    temp->id = id;
    temp->next = head;
    head = temp;
}

int deQueue(LinkedList* head){

    if(head == NULL){
        printf("-1 hata olarak dönüldü! Head null!");
        return -1;
    }
    int temp = head->id;

    if(head->next == NULL){
        free(head);
        return temp;
    }
    LinkedList* tempNode = head->next;
    free(head);
    head = tempNode;

    return temp;
}

typedef struct ArrayList{
    int* arr;
    int count;
    int capacity;
}ArrayList;

void isMemoryErrExist(const void* arr){
    if(arr == NULL){
        printf("Bellek allocation hatası!");
        exit(1);
    }
}

ArrayList* createArrayList(){
    ArrayList* newList = (ArrayList*) malloc(sizeof (ArrayList));
    isMemoryErrExist(newList);

    int* arr = (int*) malloc(25 * sizeof (int));
    isMemoryErrExist(arr);

    newList->arr = arr;
    newList->capacity = 25;
    newList->count = 0;

    return newList;
}

void AddToArrayList(ArrayList* list, int id){
    if(list->count == list->capacity){
        int* tempArr = (int*) realloc(list->arr, list->capacity * 2  * sizeof (list->arr));
        isMemoryErrExist(tempArr);

        list->arr = tempArr;
        list->capacity *= 2;
        list->arr[list->count] = id;
        list->count += 1;

        return;
    }

    list->arr[list->count] = id;
    list->count += 1;
}

void printArrayList(const ArrayList* list){
    for (int i = 0; i < list->count; ++i) {
        printf("%d: %d \t",i,list->arr[i]);
    }
    printf("\n");
}

// 0 -> ziyaret edilmedi, 1-> ziyaret edildi.
ArrayList* visitedArr;
// -1 -> parent yok.
ArrayList* parentArr;
// -1 -> daha belirli değil.
ArrayList* timeToVisited;

void DFSVisit(int size, int adjMatrix[size][size],int source, int time){
    visitedArr->arr[source] = 1;
    timeToVisited->arr[source] = ++time;

    for (int i = 0; i < size; ++i) {
        int currentAdj = adjMatrix[source][i];
        if(currentAdj == 1 && visitedArr->arr[i] == 0){
            parentArr->arr[i] = source;
            DFSVisit(size,adjMatrix,i,time);
        }
    }
}

void DFS(int size, int adjMatrix[size][size],int source){

    for (int i = 0; i < size; ++i) {
        AddToArrayList(visitedArr,0);
        AddToArrayList(parentArr,-1);
        AddToArrayList(timeToVisited,-1);
    }

    int time = 0;
    for (int i = 0; i < size; ++i) {
        if(visitedArr->arr[i] == 0)
            DFSVisit(size,adjMatrix,source,time);
    }
}



int main(){
    int adjMatrix[10][10] = {{0,1,1,1,1},{1,0,0,1,1},{1,1,0,1,0},{1,1,0,0,1},{1,1,0,1,0}};

    visitedArr = createArrayList();
    parentArr = createArrayList();
    timeToVisited = createArrayList();

    DFS(5,adjMatrix,0);

    printArrayList(visitedArr);
    printArrayList(parentArr);
    printArrayList(timeToVisited);

}
