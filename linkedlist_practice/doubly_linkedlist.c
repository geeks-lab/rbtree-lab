#include <stddef.h>
#include <stdlib.h>

typedef struct node_t {
    int value;
    struct node_t* prev;
    struct node_t* next;
} node_t;

typedef struct {
    node_t* head;
    size_t len;
} list;

// list pointer를 반환
list* list_init(){
    // 빈 리스트를 초기화
    list* p = (list*)calloc(1, sizeof(list));
    p->head = NULL;
    p->len = 0;
    return p;
}

// 노드 포인터를 반환
node_t* make_node__(int val) {
    // 노드 초기화
    node_t* p = (node_t*)calloc(1, sizeof(node_t));
    p->value = val;
    p->prev = NULL;
    p->next = NULL;
    return p;
}

void list_append_right(list* list, int value) {
    node_t* new_node_p = make_node__(value);
    node_t* cur = list->head;
    if (list->head == NULL) { // 이거 안넣으면 head가 널이라서 밑에서 에러남
        list->head = new_node_p;
        list->len += 1;
        return;
    }

    while (cur->next != NULL) {
        cur = cur->next;
    }

    cur->next = new_node_p;
    new_node_p->prev = cur;
    list->len += 1;
}

void list_append_left(list* list, int value) {
    node_t* new_node_p = make_node__(value);
    // head가 new_node_p가 된다
    if (list->head == NULL) {
        list->head = new_node_p;
        list->len += 1;
        return;
    }

    new_node_p->prev = NULL;
    new_node_p->next = list->head;
    list->head->prev = new_node_p;
    list->head = new_node_p;
    list->len += 1;
}


node_t* find(list* list, int value) {
    // list->head->next를 하나씩 전부 뒤져가며 같은 val 찾기
    node_t* cur_node = list->head;
    // node의 next가 널이 아닐때까지
    while (cur_node != NULL) {
        if (cur_node->value == value) {
            return cur_node;
        } else {
            cur_node = cur_node->next; // 여기 못함
        }
    }

    return NULL;
}

// head node 지우기
void delete_left(list* list) {
    // 헤드노드의 다음 노드의 prev를 NULL로 바꾸기
    node_t* cur_node = list->head;
    if (cur_node == NULL) {
        return;
    }

    if (cur_node->next == NULL) {
        free(cur_node);
        list->head = NULL; // <- next가 없으면
        list->len -= 1;
        return;    
    }

    cur_node->next->prev = NULL;
    list->head = cur_node->next;
    list->len -= 1;
    free(cur_node);
}

// 맨끝 노드 지우기
void delete_right(list* list) {
    node_t* cur_node = list->head;
    if (cur_node == NULL) {
        return;
    }


    while (cur_node->next != NULL){
        cur_node = cur_node->next;
    }

    if (cur_node->prev == NULL) {
        list->head = NULL; // 원소가 하나일떄
        list->len -= 1;
        free(cur_node);
        return;
    }
    // cur_node 끝까지 옴
    // cur 전 노드의 넥스트를 널로
    cur_node->prev->next = NULL;
    list->len -= 1;
    free(cur_node);
}

// n노드 오른쪽에 insert하기
void insert_node(list* list, node_t* n, int value) {
    node_t* inserting_node = make_node__(value);
    
    inserting_node->next = n->next;
    if (n->next != NULL) {
        n->next->prev = inserting_node;
    }   
    n->next = inserting_node;
    inserting_node->prev = n;

    list->len += 1;

}

int main() {
    list* lst = list_init();
    list_append_left(lst, 99);
    list_append_right(lst, 111);
    list_append_right(lst, 111111);
    list_append_left(lst, 99123);
    node_t* found_one = find(lst, 99);
    insert_node(lst, found_one, 88);
    delete_left(lst);
    delete_right(lst);
}