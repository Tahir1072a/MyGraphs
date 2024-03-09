#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>



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
        free(list->nodes);
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

// Kendi yaptığım depht first search algoritması. Verilen düğüme göre komşuları en soldan başlayarak sağa doğru yazdırır. En son kendisini yazdırır.
void DFS(struct GraphNode* node, List* list){
    // Bu düğüm eğer daha önceden ziyaret edinmiş ise fonksiyon geri döner.
    if(Search(node, list)) return;
    //Düğüm önceden ziyaret edilmediği için ve artık ilgili düğüm ile işlemler yapılacağından dolayı düğüm ziyaret edilenler listesine eklenir.
    Add(node,list);
    // İlgili düğümün komşusu var mı diye kontrol edilir.
    if(node->num_neighbors != 0){
        for (int i = 0; i < node->num_neighbors; ++i) {
            //En sağ düğümden başlayarak sola doğru tarama yapar.
            DFS(node->neighbors[i],list);
        }
    }
    //İlgili düğümün artık komşusu yoksa veya komşuları daha önceden ziyaret edilmiş ise içindeki datayı ekrana yazdırır.
    printf("%d\n",node->data);
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
int main() {
    int matrix[4][4] = {{1,1,1,0},{0,0,1,1},{1,1,0,1},{1,0,0,0}};

    List* list = createAllNodes(4,matrix);
    List* list2 = createNewList();
    struct GraphNode* node2 = list->nodes[0]->neighbors[2];
    DFS(list->nodes[0], list2);

    return 0;
}
