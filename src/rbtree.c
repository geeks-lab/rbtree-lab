#include "rbtree.h"

#include <stdlib.h>

/**
 * @brief : 새로운 노드 node_t를 생성하고 0으로 초기화
 * @param[in] t : 노드를 생성할 rbtree 
 * @param[in] key : 해당 노드의 키 값
 * @return : 생성된 node_t의 포인터를 반환 
*/
node_t *new_node__(rbtree *t, const key_t key) {
  node_t *new_node__ = (node_t *)calloc(1, sizeof(node_t));
  new_node__->left = t->nil;
  new_node__->right = t->nil;
  new_node__->key = key;
  new_node__->color = RBTREE_RED;
  return new_node__;
}

/**
 * @brief : 새로운 rbtree를 생성하고 0으로 초기화
 * @return : 생성된 rbtree의 포인터를 반환
*/
rbtree *new_rbtree(void) {
  rbtree *t = (rbtree *)calloc(1, sizeof(rbtree));
  t->nil = (node_t *)calloc(1, sizeof(node_t));
  t->nil->color = RBTREE_BLACK;
  t->root = t->nil;
  return t;
}

/**
 * @brief : 재귀적으로 트리 안 노드 전체를 삭제
 * @param[in] t : rbtree
 * @param[in] node : 삭제를 시작할 노드
*/
void delete_rbtree_nodes__(rbtree *t, node_t *node) {
  if (node != t->nil) {
    delete_rbtree_nodes__(t, node->left);
    delete_rbtree_nodes__(t, node->right);
    free(node);
  }
}

/**
 * @brief : rbtree 전체 삭제를 수행
 * @param[in] t : rbtree를 받음
*/
void delete_rbtree(rbtree *t) {
  delete_rbtree_nodes__(t, t->root);
  free(t->nil);
  free(t);
}

void left_rotation__(rbtree *t, node_t *x) {
  node_t *y = x->right; // y 설정
  x->right = y->left; // y의 왼쪽 서브트리를 x의 오른쪽 서브트리로 옮긴다.
  if (y->left != t->nil) {
    y->left->parent = x;
  }
  y->parent = x->parent; // x의 부모를 y로 연결한다.
  if (x->parent == t->nil) {
    t->root = y;
  } else if (x == x->parent->left) {
    x->parent->left = y;
  } else {
    x->parent->right = y;
  }
  y->left = x; // x를 y의 왼쪽으로 놓는다.
  x->parent = y;
}

void right_rotation__(rbtree *t, node_t *x) {
  node_t *y = x->left;
  x->left = y->right;
  if (y->right != t->nil) {
    y->right->parent = x;
  }
  y->parent = x->parent;
  if (x->parent == t->nil) {
    t->root = y;
  } else if (x == x->parent->right) {
    x->parent->right = y;
  } else {
    x->parent->left = y;
  }
  y->right = x;
  x->parent = y;
}

void rbtree_insert_fixup__(rbtree *t, node_t *z) { // refactor - 이름 z 대신 직관적으로 바꾸기
  while (z->parent->color == RBTREE_RED) {
    if (z->parent == z->parent->parent->left) {
      node_t *y = z->parent->parent->right;
      if (y->color == RBTREE_RED) {
        z->parent->color = RBTREE_BLACK;          // case 1
        y->color = RBTREE_BLACK;                  // case 1
        z->parent->parent->color = RBTREE_RED;    // case 1
        z = z->parent->parent;                    // case 1
      } else { 
        if (z == z->parent->right) {
          z = z->parent;                            // case 2
          left_rotation__(t, z);                       // case 2
        }
        z->parent->color = RBTREE_BLACK;            // case 3
        z->parent->parent->color = RBTREE_RED;              // case 3
        right_rotation__(t, z->parent->parent);        // case 3
      }
    } else { // z->parent == z->parent->parent->right -> case 4~6 (위 while 밑 if 안에를 right를 left로 바꾼것과 같다)
      node_t *y = z->parent->parent->left;
      if (y->color == RBTREE_RED) {
        z->parent->color = RBTREE_BLACK;          // case 1
        y->color = RBTREE_BLACK;                  // case 1
        z->parent->parent->color = RBTREE_RED;    // case 1
        z = z->parent->parent;                    // case 1
      } else { 
        if (z == z->parent->left) { 
          z = z->parent;                            // case 2
          right_rotation__(t,z);                      // case 2
        }
        z->parent->color = RBTREE_BLACK;            // case 3
        z->parent->parent->color = RBTREE_RED;              // case 3
        left_rotation__(t,z->parent->parent);         // case 3
      }
    }
  }
  t->root->color = RBTREE_BLACK;
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  node_t *z = new_node__(t, key);
  node_t *y = t->nil;
  node_t *x = t->root;

  while (x != t->nil) {
    y = x;
    if (z->key < x->key) {
      x = x->left;
    } else {
      x = x->right;
    }
  }
  z->parent = y;

  if (y == t->nil) {
    t->root = z;
  } else if (z->key < y->key) {
    y->left = z;
  } else {
    y->right = z;
  }

  rbtree_insert_fixup__(t, z);

  return z;
}

/**
 * @brief : 키값에 해당하는 노드를 찾아준다.
 * @param[in] t : 트리를 받는다.
 * @param[in] key : 키를 받는다.
 * @return : 찾은 노드를 반환하고, 찾고자 하는게 없으면 NULL을 반환한다.
*/
node_t *rbtree_find(const rbtree *t, const key_t key) {
  node_t *node = t->root;

  while (node != t->nil) {
    if (key == node->key) {
      return node;
    } else if (key < node->key) {
      node = node->left;
    } else {
      node = node->right;
    }
  } 
  return NULL;
}

node_t *rbtree_min(const rbtree *t) {
  node_t *cur_node = t->root;

  while (cur_node->left != t->nil) {
    cur_node = cur_node->left;
  }
  return cur_node;
}

node_t *rbtree_max(const rbtree *t) {
  node_t *cur_node = t->root;

  while (cur_node->right != t->nil) {
    cur_node = cur_node->right;
  }
  return cur_node;
}

/**
 * @brief : delete_rbtree(t, z)에서 삭제할 노드 z와 z를 대체할 노드 y(successor)의 연결 관계를 바꿈
 * @param[in] t : 해당 트리
 * @param[in] u: 삭제할 노드
 * @param[in] v: 삭제할 노드를 대체할 노드
*/
void delete_transplant__(rbtree *t, node_t *u, node_t *v) {
  if (u->parent == t->nil) {
    t->root = v;
  } else if (u == u->parent->left) {
    u->parent->left = v;
  } else {
    u->parent->right = v;
    
  }
  v->parent = u->parent;
}

// w : sibling
void delete_fixup__(rbtree *t, node_t *x){
  while (x != t->root && x->color == RBTREE_BLACK) {
    if (x == x->parent->left) {
      node_t *w = x->parent->right; // TODO: 여기 나중에 이해하기
      if (w->color == RBTREE_RED) {
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        left_rotation__(t, x->parent);
        w = x->parent->right;
      }
      if (w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK) {
        w->color = RBTREE_RED;
        x = x->parent;
      } else {
        if (w->right->color == RBTREE_BLACK) {
          w->left->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          right_rotation__(t, w);
          w = x->parent->right;
        }
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->right->color = RBTREE_BLACK;
        left_rotation__(t,x->parent);
        x = t->root;
      }
    } else {
      node_t* w = x->parent->left;
      if (w->color == RBTREE_RED) {
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        right_rotation__(t, x->parent);
        w = x->parent->left;
      }
      if (w->left->color == RBTREE_BLACK && w->left->color == RBTREE_BLACK) {
        w->color = RBTREE_RED;
        x = x->parent;
      } else {
        if (w->left->color == RBTREE_BLACK) {
          w->right->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          left_rotation__(t, w);
          w = x->parent->left;
        }
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->left->color = RBTREE_BLACK;
        right_rotation__(t,x->parent);
        x = t->root;
      }
    }
  }
  x->color = RBTREE_BLACK;
}

/**
 * @brief : 인자로 받은 노드를 트리에서 삭제한다.
 * @param[in] t: rbtree를 받는다.
 * @param[in] p: 삭제 받을 노드를 받는다.
 * psuedo code에서 
 * z : 삭제할 대상 노드, y: 삭제 대상 노드 또는 대체할 노드, x: y가 삭제될 경우 대체할 y의 자식 노드
*/
int rbtree_erase(rbtree *t, node_t *z) {
  // psueo 따라하기
  node_t* y = z;
  node_t* x;
  color_t y_original_color = y->color;
  if (z->left == t->nil) {
    x = z->right;
    delete_transplant__(t, z, z->right);
  } else if (z->right == t->nil) {
    x = z->left;
    delete_transplant__(t, z, z->left);
  } else {
    // y = subtree 에서 min값(즉,y에 successor값 넣기) // (의사코드 9번줄)
    y = z->right;
    while (y->left != t->nil) {
      y = y->left; // 왼쪽 끝까지 내려가면 y가 successor임
    }
    y_original_color = y->color;
    x = y->right;
    if (y->parent == z) {
      x->parent = y;
    } else {   // 의사코드 14번줄 transplant(t, y, y->right)
      delete_transplant__(t, y, y->right);
      y->right = z->right;
      y->right->parent = y;
    }
    delete_transplant__(t, z, y);
    y->left = z->left;
    y->left->parent = y;
    y->color = z->color;
  } 
  if (y_original_color == RBTREE_BLACK) {
    delete_fixup__(t, x);
  }
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  return 0;
}