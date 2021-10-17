#include <iostream>
using namespace std;
typedef int Status;
#define TRUE 1
#define FALSE 0

/* 二叉排序树因为树的结构不稳定（取决于根节点在序列中位置的影响，
   导致树的深度不确定，造成对树的查找等操作时间复杂度从O(logn)~O(n)不定。
   引出 平衡二叉树(AVL) ———— 每一个节点的左子树和右子树的高度差至多为1，
        最小不平衡子树 ———— 距离插入结点最近的，且平衡因子的绝对值大于1的节点为根的子树 */
/* 平衡二叉树的构造逻辑：每当插入一个结点时，先检查是否因插入而破坏了树的平衡性，若是，则找出最 小不平衡子树。在保持二叉排序树特性的前提下，调整最小不平衡子树
中各结点之间的链接关系，进行相应的旋转，使之成为新的平衡子树
   算法核心：当最小不平衡子树根 结点的平衡因子BF是大于1时，就右旋，小于-1时就左旋；
   插入结点后，最小不平衡子树的BF与它的子 树的BF符号相反时，就需要
   对结点先进行一次旋转以使得符号相同后，再反向旋转一次才能够完成平衡操作 */
typedef struct BiTNode_AVL
{
    int data;
    int bf; // 结点的平衡因子
    struct BiTNode_AVL *lchild, *rchild;
} BiTNode_AVL, *BiTree_AVL;

/* 右旋操作 */
void R_Rotate(BiTree_AVL *P)
{
    BiTree_AVL L; // 新根节点指针
    L = (*P)->lchild;
    (*P)->lchild = L->rchild;
    L->rchild = *P;
    *P = L;
}

/* 类似的有左旋操作 */
void L_Rotate(BiTree_AVL *P)
{
    BiTree_AVL R; // 新根节点指针
    R = (*P)->rchild;
    (*P)->rchild = R->lchild;
    R->lchild = *P;
    *P = R;
}
/* 有了左旋和右旋基本操作之后，AVL的平衡分为四种类型：LL, LR, RL, RR */
/* 左平衡旋转处理函数 */
#define LH 1
#define EH 0
#define RH -1
void LeftBalance(BiTree_AVL *T)
{
    BiTree_AVL Lr, L;
    L = (*T)->lchild;
    switch (L->bf)
    {
    /* 单右旋LL */
    case LH:
        (*T)->bf = L->bf = EH;
        R_Rotate(T);
        break;
    /* 双旋LR：先左旋，再右旋转 */
    case RH:
        Lr = L->rchild;
        switch (Lr->bf)
        {
        case LH:
            (*T)->bf = RH;
            L->bf = EH;
            break;
        case EH:
            (*T)->bf = L->bf = EH;
            break;
        case RH:
            (*T)->bf = EH;
            L->bf = LH;
            break;
        }
        Lr->bf = EH;
        L_Rotate(&(*T)->lchild);
        R_Rotate(T);
    }
}

/* 同理有 右平衡旋转处理函数 */
void RightBalance(BiTree_AVL *T)
{
    BiTree_AVL Rr, R;
    R = (*T)->rchild;
    switch (R->bf)
    {
    /* 单左旋RR */
    case RH:
        (*T)->bf = R->bf = EH;
        L_Rotate(T);
        break;
    /* 双旋RL：先右旋，再左旋转 */
    case LH:
        Rr = R->lchild;
        switch (Rr->bf)
        {
        case RH:
            (*T)->bf = LH;
            R->bf = EH;
            break;
        case EH:
            (*T)->bf = R->bf = EH;
            break;
        case LH:
            (*T)->bf = EH;
            R->bf = RH;
            break;
        }
        Rr->bf = EH;
        R_Rotate(&(*T)->rchild);
        L_Rotate(T);
    }
}

/* 若在平衡的二叉排序树T中不存在和e有相同关键字的结点，则插入一个数据元素为e的新结点并返回1，否则返回0。
   若因插入而使二叉排序树 失去平衡，则作平衡旋转处理，布尔变量taller 反映T长高与否。 */
Status InsertAVL(BiTree_AVL *T, int e, Status *taller)
{
    if (!*T)
    {
        *T = (BiTree_AVL)malloc(sizeof(BiTree_AVL));
        (*T)->data = e;
        (*T)->lchild = (*T)->rchild = NULL;
        (*T)->bf = EH;
        *taller = TRUE;
    }
    else
    {
        if (e == (*T)->data)
        {
            *taller = FALSE;
            return FALSE;
        }
    }

    if (e < (*T)->data)
    {
        if (!InsertAVL(&(*T)->lchild, e, taller))
            return FALSE;
        if (*taller) // 已经将e插入到平衡二叉树T中
        {
            switch ((*T)->bf)
            {
            // 本来就是左子树更高，现在插入新元素后更高，需要对T进行左平衡旋转处理
            case LH:
                LeftBalance(T);
                *taller = FALSE;
                break;
            case EH:
                (*T)->bf = LH;
                *taller = TRUE;
                break;
            case RH:
                (*T)->bf = EH;
                *taller = FALSE;
                break;
            }
        }
        else
        {
            if (InsertAVL(&(*T)->rchild, e, taller))
                return FALSE;
            switch ((*T)->bf)
            {
            case LH:
                (*T)->bf = EH;
                *taller = FALSE;
                break;
            case EH:
                (*T)->bf = RH;
                *taller = TRUE;
                break;
            case RH:
                RightBalance(T);
                *taller = FALSE;
                break;
            }
        }
    }
    return TRUE;
}

void pre_order(BiTree_AVL *T)
{
    BiTree_AVL *p = T;
    if (*p != NULL)
    {
        cout << (*p)->data << " ";
        pre_order(&(*p)->lchild);
        pre_order(&(*p)->rchild);
    }
}

int main()
{
    int i;
    int a[10] = {3, 2, 1, 4, 5, 6, 7, 10, 9, 8};
    BiTree_AVL *T = NULL;
    Status taller;
    for (i = 0; i < 10; i++)
        InsertAVL(T, a[i], &taller);
    pre_order(T);
    return 0;
}