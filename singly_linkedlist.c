#include <stdio.h>

typedef struct node
{
	int value;
    struct node* next;
}node;

node *head = NULL;  //head는 null
void insert_node_front()
{
    node* newNode; // node를 가르키는 newNode라는 포인터 변수						
    newNode = (node*)malloc(sizeof(node));//malloc으로 node를 생성후 
    								//newNode라는 포인터 변수가 가르키게
	scanf("%d", &newNode->value); // 
    newNode->next=NULL; // 다음 노드가 없으니까 널
    if(head==NULL){ // head가 널이라면
    	head = newNode; // 첫 노드의 주소를 헤드에
        return; // 함수종료
        //현재까지상황: head도, newNode도 새로 생성된 주소를 가르키고 있는 상황
    }
    // 두번째 노드가 생성 되었을 때
    newNode->next=head; //newNode의next가 3번주소값을 가지게.(3번주소를 가르키게) 
     head = newNode; // head가 새로 생성된 4번 주소의 노드를 가르켜서 4번 노드가 첫노드가 될 수 있게
}


