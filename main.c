#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <windows.h>

// Windows için zaman bilgisini döndüren fonksiyon
double get_time()
{
    LARGE_INTEGER t, f;
    QueryPerformanceCounter(&t); // Sistem saatinin değerini al
    QueryPerformanceFrequency(&f); // Saat frekansını al
    return (double)t.QuadPart/(double)f.QuadPart; // Zaman bilgisini dön
}

// Bu node yapımız;
struct GraphNode {
    int data;
    int num_neighbors;
    // Node'un bellekteki yerini tutuan pointerların dizisini tutan pointer.
    struct GraphNode** neighbors;
};

// Bir graph node oluşturmamızı sağlar. Mesela bir arkadaş nesnesi üretir gibi...
struct GraphNode* createGraphNode(int data, int num_neighbors){
    struct GraphNode* newNode = (struct GraphNode*) malloc(sizeof (struct GraphNode));
    newNode->data = data;
    newNode->num_neighbors = num_neighbors;
    newNode->neighbors = (struct GraphNode**) malloc(num_neighbors * sizeof (struct GraphNode*));

    return newNode;
}

//GraphNode*'ları tutabilen bir liste
struct List{
    struct GraphNode** nodes;
    int capacity;
    int count;
};

typedef struct List List;

// Tekrar tekrar yazmak istemediğim için ekrana tahsis hatası yazan fonksiyon.
void printErr(){
    printf("Liste için bellekte alan tahsis edilemedi.");
    exit(1);
}

//Yeni boş bir liste oluşturur. Default capacity değeri 4 tür.
List* createNewList(){
    List* list = (List*) malloc(sizeof (List));
    if(list == NULL){
        printErr();
    }
    list->nodes = (struct GraphNode**) malloc(4 * sizeof(struct GraphNode*));
    if(list->nodes == NULL){
        printErr();
    }
    list->count = 0;
    list->capacity = 4;

    return list;
}

//Verilen listeye verilen düğümü ekler.
void Add(struct GraphNode* node, List* list){

    if(list->count == list->capacity){
        struct GraphNode** newArr= (struct GraphNode**) realloc(list->nodes, list->capacity * 2 * sizeof (struct GraphNode*));
        if(newArr == NULL){
            printErr();
        }
        list->nodes = newArr;
        list->capacity *= 2;

        list->nodes[list->count] = node;
        list->count += 1;
        return;
    }

    list->nodes[list->count] = node;
    list->count += 1;
}

//Verilen list içinde verilen düğümü arar.
bool Search(struct GraphNode* node,List* list){

    for (int i = 0; i < list->count; i++) {
        if(list->nodes[i] == node){
            return true;
        }
    }

    return false;
}

//Temel dfs algoritması
//void DFS(struct GraphNode* node, List* list){
//    if(Search(node,list)) return;
//    Add(node, list);
//
//    printf("%d->",node->data);
//
//    for (int i = 0; i < node->num_neighbors; ++i) {
//        DFS(node->neighbors[i],list);
//    }
//}

// Componentsleri de bulan bir algoritma.
void dfs2(struct GraphNode* node, List* list, int* components, int count){
    if(Search(node,list)) return;
    Add(node,list);

    for (int i = 0; i < node->num_neighbors; ++i) {
        dfs2(node->neighbors[i],list,components,count);
    }

    components[node->data] = count;
}

int* findComponents(List* nodeList, int* components){
    int count = 0;
    components = (int*) malloc(nodeList->count * sizeof (int));
    List* emptyList = createNewList();
    for (int i = 0; i < nodeList->count; ++i) {
        if(Search(nodeList->nodes[i],emptyList)) continue;
        count++;
        dfs2(nodeList->nodes[i],emptyList,components,count);
    }

    return components;
}

//verilen matrisi ekrana yazdırır.
void printMatrix(int satir, int sutun, int matrix[][sutun]){
    for (int i = 0; i < satir; i++) {
        for (int j = 0; j < sutun; j++) {
            printf("%d\t",matrix[i][j]);
        }
        printf("\n");
    }
}

void printArr(int size, int* arr){
    for (int i = 0; i < size; ++i) {
        printf("%d \t",arr[i]);
    }
    printf("\n");
}

// Bir komşuluk matrisi alır ve bu matris üzerinden düğümleri oluşturur ve ilişkilerini kurar.
List* createAllNodes(int size, int matrix[size][size]){
    List* list = createNewList();
    for (int i = 0; i < size; i++) {
        struct GraphNode* node = (struct GraphNode*) malloc(sizeof (struct GraphNode));
        node->num_neighbors = 0;
        node->data = i;
        if(node == NULL) printErr();
        for (int j = 0; j < size; j++) {
            if(matrix[i][j] == 1)
                node->num_neighbors++;
        }
        Add(node,list);
    }
    int k = 0;
    for (int i = 0; i < size; i++) {
        k = 0;
        list->nodes[i]->neighbors = (struct GraphNode**) malloc(list->nodes[i]->num_neighbors * sizeof (struct GraphNode*));
        for (int j = 0; j < size; j++) {
            if(matrix[i][j] == 1) {
                list->nodes[i]->neighbors[k] = list->nodes[j];
                k++;
            }
        }
    }

    return list;
}

//List yapımız oluşturuldu tekrar dfs algoritması test edilcek ve ilgili graf için otomatik matrix oluşturma işlemi yapılacak.
//int main() {
//    int matrix[6][6] = {{1,1,1,0,0,1},{0,0,1,1,0,0},{1,1,0,1,0,0},{1,0,0,0,0,0},{0,1,0,0,0,1},{0,0,0,0,1,0}};
//
//    //Çalışma zamanı testi
//    double z1 = get_time();
//    double z2 = 0;
//
//    List* list = createAllNodes(6,matrix);
//    List* list2 = createNewList();
//    DFS(list->nodes[0], list2);
//    printf("\n");
//    int* components = findComponents(list,components);
//    printArr(list->count,components);
//
//    z2 = get_time();
//    printf("Work Time :%f\n", (z2-z1));
//
//    return 0;
//}
