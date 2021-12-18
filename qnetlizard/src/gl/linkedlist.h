#ifndef _KARIN_LINKEDLIST_H
#define _KARIN_LINKEDLIST_H

#ifdef __cplusplus
extern "C" {
#endif

#define List_FrontT(lst, T) (T *)List_Front(lst)
#define List_BackT(lst, T) (T *)List_Back(lst)
#define List_FrontValueT(lst, T) *((T *)List_Front(lst))
#define List_BackValueT(lst, T) *((T *)List_Back(lst))

#define List_FindT(lst, cmp, user_data, index, T) (T *)List_Find(lst, cmp, user_data, index)
#define List_FindValueT(lst, cmp, user_data, index, T) *((T *)List_Find(lst, cmp, user_data, index))

#define List_GetDataByIndexT(lst, index, T) (T *)List_GetDataByIndex(lst, index)
#define List_GetConstDataByIndexT(lst, index, T) (const T *)List_GetDataByIndex(lst, index)
#define List_TakeOneByDataT(lst, data, T) (T *)List_TakeOneByData(lst, data)
#define List_TakeOneByIndexT(lst, index, T) (T *)List_TakeOneByIndex(lst, index)

#define List_GetDataByIndexValueT(lst, index, T) *((T *)List_GetDataByIndex(lst, index))
#define List_GetConstDataByIndexValueT(lst, index, T) *((const T *)List_GetDataByIndex(lst, index))
#define List_TakeOneByDataValueT(lst, data, T) *((T *)List_TakeOneByData(lst, data))
#define List_TakeOneByIndexValueT(lst, index, T) *((T *)List_TakeOneByIndex(lst, index))

#define list_node(T) list_node_t
#define linkedlist(T) linkedlist_t
#define LIST(T) {sizeof(T), NULL, 0, NULL, List_Append, List_AppendV}

#define LISTV_DATA_FOREACH(decl, d, lst) \
    for(decl = (lst)->data, d = decl ? decl->data : NULL; decl; decl = decl->next, d = decl ? decl->data : NULL)

#define LISTV_DATA_FOREACH_WITH_COUNTER(decl, d, i, lst) \
    for(decl = (lst)->data, i = 0, d = decl ? decl->data : NULL; decl; decl = decl->next, i++, d = decl ? decl->data : NULL)

#define LIST_DATA_FOREACH(decl, d, lst) \
    for(decl = (lst).data, d = decl ? decl->data : NULL; decl; decl = decl->next, d = decl ? decl->data : NULL)

#define LIST_DATA_FOREACH_WITH_COUNTER(decl, d, i, lst) \
    for(decl = (lst).data, i = 0, d = decl ? decl->data : NULL; decl; decl = decl->next, i++, d = decl ? decl->data : NULL)

#define LISTV_DATA_FOREACH_T(T, decl, d, lst) \
    for(decl = (lst)->data, d = (T *)(decl ? decl->data : NULL); decl; decl = decl->next, d = (T *)(decl ? decl->data : NULL))

#define LISTV_DATA_FOREACH_WITH_COUNTER_T(T, decl, d, i, lst) \
    for(decl = (lst)->data, i = 0, d = (T *)(decl ? decl->data : NULL); decl; decl = decl->next, i++, d = (T *)(decl ? decl->data : NULL))

#define LIST_DATA_FOREACH_T(T, decl, d, lst) \
    for(decl = (lst).data, d = (T *)(decl ? decl->data : NULL); decl; decl = decl->next, d = (T *)(decl ? decl->data : NULL))

#define LIST_DATA_FOREACH_WITH_COUNTER_T(T, decl, d, i, lst) \
    for(decl = (lst).data, i = 0, d = (T *)(decl ? decl->data : NULL); decl; decl = decl->next, i++, d = (T *)(decl ? decl->data : NULL))

#define List_DeleteNode(lst, p) {\
    list_node_t *last = p->prev; \
    list_node_t *next = p->next; \
    if(last) last->next = next; \
    if(next) next->prev = last; \
    if(p == lst->data) lst->data = next; \
    if(p == lst->last) lst->last = last; \
    free(p->data); free(p); \
    lst->count--; \
}

#define List_TakeNode(lst, p) {\
    list_node_t *last = p->prev; \
    list_node_t *next = p->next; \
    if(last) last->next = next; \
    if(next) next->prev = last; \
    if(p == lst->data) lst->data = next; \
    if(p == lst->last) lst->last = last; \
    free(p); \
    lst->count--; \
}

#define List_Empty(list) ((list)->count == 0)
#define List_Size(list) ((list)->count)

#define LIST_NODE_NEXT(p) (p)->next
#define LIST_NODE_PREV(p) (p)->prev
#define LIST_NODE_DATA(p) (p)->data
#define LIST_NODE_DATA_T(T, p) (T *)((p)->data)

#define LIST_FRONT_NODE(list) (list).data
#define LISTV_FRONT_NODE(list) (list)->data
#define LIST_BACK_NODE(list) (list).last
#define LISTV_BACK_NODE(list) (list)->last

struct linkedlist_s;
typedef struct linkedlist_s * (* linkedlist_vvv_func_f)(struct linkedlist_s *lst, void *data);
typedef struct linkedlist_s * (* linkedlist_vv_func_f)(struct linkedlist_s *lst, const void *data);
typedef int (* linkedlist_cmp_func_f)(const void *lst_data, const void *data);

typedef struct list_node_s
{
    struct list_node_s *prev;
	void *data; // allow NULL
    struct list_node_s *next;
} list_node_t;

typedef struct linkedlist_s
{
	unsigned int size; // sizeof()
    list_node_t *data; // first node pointer
	unsigned int count;

    list_node_t *last; // last node pointer

	// function
    linkedlist_vv_func_f vv;
    linkedlist_vvv_func_f vvv;
} linkedlist_t;

// 遍历函数指针 参数为索引和数据 const和非const版本
typedef void (*linkedlist_foreach_func_f)(unsigned int index, void *data);
typedef void (*linkedlist_foreach_const_func_f)(unsigned int index, const void *data);

linkedlist_t * List_Make(linkedlist_t *lst, unsigned int size);
// 添加一个新数据到表尾
int List_PushBack(linkedlist_t *lst, const void *d);
int List_PushBackV(linkedlist_t *lst, void *d);
linkedlist_t * List_Append(linkedlist_t *lst, const void *d);
linkedlist_t * List_AppendV(linkedlist_t *lst, void *d);
// 添加一个新数据到表头
int List_PushFront(linkedlist_t *lst, const void *d);
int List_PushFrontV(linkedlist_t *lst, void *d);
// 根据索引获取数据 const和非conat
const void * List_GetConstDataByIndex(const linkedlist_t *lst, int index);
void * List_GetDataByIndex(linkedlist_t *lst, int index);
// 根据数据获取索引
int List_GetDataByData(const linkedlist_t *lst, const void *d);
// 插入新数据到表中
int List_InsertInto(linkedlist_t *lst, int index, const void *d);
int List_InsertIntoV(linkedlist_t *lst, int index, void *d);
// 通过索引删除一个数据
int List_DeleteOneByIndex(linkedlist_t *lst, int index);
// 删除一个
int List_DeleteOneByNode(linkedlist_t *lst, list_node_t *node);
// 清空列表
void List_DeleteAll(linkedlist_t *lst);
// 通过数据删除一个数据
int List_DeleteOneByData(linkedlist_t *lst, const void *d);
// const遍历
void List_ForEachConst(const linkedlist_t *lst, linkedlist_foreach_const_func_f f);
// 非const遍历
void List_ForEach(linkedlist_t *lst, linkedlist_foreach_func_f f);
// 根据数据从表中取出数据
void * List_TakeOneByData(linkedlist_t *lst, const void *d);
// 根据索引从表中取出数据
void * List_TakeOneByIndex(linkedlist_t *lst, int index);
// 根据数据从表中取出数据
void * List_TakeOneByNode(linkedlist_t *lst, const list_node_t *node);
// 头，尾
void * List_Front(linkedlist_t *lst);
void * List_Back(linkedlist_t *lst);
// 删除头，尾
int List_PopFront(linkedlist_t *lst);
int List_PopBack(linkedlist_t *lst);
// 查找
void * List_Find(linkedlist_t *lst, linkedlist_cmp_func_f cmp, const void *user_data, int *index);
// 转换数组
void * List_ToArray(linkedlist_t *lst);
unsigned int List_ToArrayV(linkedlist_t *lst, void *r, unsigned int max);

#ifdef __cplusplus
}
#endif

#endif // _KARIN_LINKEDLIST_H
