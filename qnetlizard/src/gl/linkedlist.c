#include "linkedlist.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define NEW_LIST_NODE(p) \
    list_node_t *p = malloc(sizeof(list_node_t)); \
memset(p, 0, sizeof(list_node_t)); \
p->prev = NULL; \
p->next = NULL; \
p->data = NULL;

#define NEW_NODE_DATA(p, d, size) \
{ \
	p->data = malloc(size); \
	memcpy(p->data, d, size); \
}

#define LIST_FOREACH(decl, lst) \
    for(decl = (lst)->data; decl; decl = decl->next)

#define LIST_FOREACH_WITH_COUNTER(decl, i, lst) \
    for(decl = (lst)->data, i = 0; decl; decl = decl->next, i++)

static int List_DataCmp(const linkedlist_t *lst, const void *pdata, const void *d)
{
    return((pdata == NULL && d == NULL)
            || ((pdata && d) && (memcmp(pdata, d, lst->size) == 0)))
            ? 1 : 0;
}

linkedlist_t * List_Make(linkedlist_t *lst, unsigned int size)
{
    lst->size = size;
    lst->data = NULL;
    lst->count = 0;
    lst->vv = List_Append;
    lst->vvv = List_AppendV;
    lst->last = NULL;
    return lst;
}

int List_PushBackV(linkedlist_t *lst, void *d)
{
	if(!lst->data)
	{
		NEW_LIST_NODE(n);
		if(d)
			n->data = d;
		lst->data = n;
        lst->last = n;
        lst->count++;
		return 0;
	}
	else
	{
        list_node_t *p = lst->last;
        NEW_LIST_NODE(n);
        if(d)
            n->data = d;
        p->next = n;
        n->prev = p;
        lst->last = n;
        lst->count++;
        return lst->count - 1;
	}
}

int List_PushBack(linkedlist_t *lst, const void *d)
{
	void *pdata;

    if(!d)
        return List_PushBackV(lst, NULL);

	pdata = malloc(lst->size);
	memcpy(pdata, d, lst->size);
	return List_PushBackV(lst, pdata);
}

linkedlist_t * List_Append(linkedlist_t *lst, const void *d)
{
	List_PushBack(lst, d);
	return lst;
}

linkedlist_t * List_AppendV(linkedlist_t *lst, void *d)
{
	List_PushBackV(lst, d);
	return lst;
}

int List_PushFrontV(linkedlist_t *lst, void *d)
{
	if(!lst->data)
	{
		NEW_LIST_NODE(n);
		if(d)
			n->data = d;
		lst->data = n;
        lst->last = n;
		lst->count++;
	}
	else
	{
		NEW_LIST_NODE(n);
		if(d)
			n->data = d;
        list_node_t *f = lst->data;
		lst->data = n;
		n->next = f;
		f->prev = n;
		lst->count++;
	}
	return 0;
}

int List_PushFront(linkedlist_t *lst, const void *d)
{
	void *pdata;

    if(!d)
        List_PushFrontV(lst, NULL);
	pdata = malloc(lst->size);
	memcpy(pdata, d, lst->size);
	return List_PushFrontV(lst, pdata);
}

const void * List_GetConstDataByIndex(const linkedlist_t *lst, int index)
{
    const list_node_t *p;
    unsigned int i;
	if(index >= lst->count)
        return NULL;
    int nindex = index < 0 ? lst->count + index : index;
    if(nindex >= lst->count || nindex < 0)
        return NULL;
    LIST_FOREACH_WITH_COUNTER(p, i, lst)
	{
        if(i == nindex)
            break;
	}
	return p ? p->data : NULL;
}

int List_GetDataByData(const linkedlist_t *lst, const void *d)
{
    const list_node_t *p;
    unsigned int i;
    LIST_FOREACH_WITH_COUNTER(p, i, lst)
	{
        if(List_DataCmp(lst, p->data, d))
            return i;
	}
	return -1;
}

int List_InsertIntoV(linkedlist_t *lst, int index, void *d)
{
    list_node_t *p;
    int nindex = index < 0 ? lst->count + index : index;
    if(nindex >= lst->count)
        return List_PushBackV(lst, d);
    if(nindex < 0)
        return List_PushFrontV(lst, d);

    unsigned int i;
    LIST_FOREACH_WITH_COUNTER(p, i, lst)
	{
		if(i == index)
		{
			NEW_LIST_NODE(n);
			if(d)
				p->data = d;
            list_node_t *old_p = p->prev;
			n->prev = old_p;
			n->next = p;
			p->prev = n;
			if(old_p)
				old_p->next = n;
			if(p == lst->data)
				lst->data = n;
			lst->count++;
			return i;
        }
	}
	return -1;
}

int List_InsertInto(linkedlist_t *lst, int index, const void *d)
{
	void *pdata;

    if(!d)
        return List_InsertIntoV(lst, index, NULL);
	pdata = malloc(lst->size);
	memcpy(pdata, d, lst->size);
	return List_InsertIntoV(lst, index, pdata);
}

void List_DeleteAll(linkedlist_t *lst)
{
    if(!lst)
        return;
    list_node_t *p = lst->data;
    while(p)
    {
        list_node_t *next = p->next;
        if(p->data)
            free(p->data);
        free(p);
        p = next;
    }
    lst->data = NULL;
    lst->last = NULL;
    lst->count = 0;
}

int List_DeleteOneByIndex(linkedlist_t *lst, int index)
{
    list_node_t *p;
    unsigned int i;
	if(index >= lst->count)
        return 0;
    int nindex = index < 0 ? lst->count + index : index;
    if(nindex >= lst->count || nindex < 0)
        return 0;
    LIST_FOREACH_WITH_COUNTER(p, i, lst)
	{
        if(i == nindex)
        {
            list_node_t *last = p->prev;
            list_node_t *next = p->next;
			if(last)
				last->next = next;
			if(next)
                next->prev = last;
			if(i == 0)
				lst->data = next;
            if(p == lst->last)
                lst->last = last;
            free(p->data);
            free(p);
			lst->count--;
            return 1;
        }
	}
    return 0;
}

int List_DeleteOneByNode(linkedlist_t *lst, list_node_t *node)
{
    list_node_t *p;
    LIST_FOREACH(p, lst)
    {
        if(node == p)
        {
            list_node_t *last = p->prev;
            list_node_t *next = p->next;
            if(last)
                last->next = next;
            if(next)
                next->prev = last;
            if(p == lst->data)
                lst->data = next;
            if(p == lst->last)
                lst->last = last;
            free(p->data);
            free(p);
            lst->count--;
            return 1;
        }
    }
    return 0;
}

int List_DeleteOneByData(linkedlist_t *lst, const void *d)
{
    list_node_t *p;
    LIST_FOREACH(p, lst)
	{
        if(List_DataCmp(lst, p->data, d))
		{
            list_node_t *last = p->prev;
            list_node_t *next = p->next;
			if(last)
				last->next = next;
			if(next)
				next->prev = last;
            if(p == lst->data)
                lst->data = next;
            if(p == lst->last)
                lst->last = last;
			free(p->data);
            free(p);
			lst->count--;
            return 1;
        }
	}
    return 0;
}

void List_ForEachConst(const linkedlist_t *lst, linkedlist_foreach_const_func_f f)
{
    const list_node_t *p;
    unsigned int i;
    LIST_FOREACH_WITH_COUNTER(p, i, lst)
    {
        f(i, p->data);
	}
}

void List_ForEach(linkedlist_t *lst, linkedlist_foreach_func_f f)
{
    list_node_t *p;
    unsigned int i;
    LIST_FOREACH_WITH_COUNTER(p, i, lst)
    {
        f(i, p->data);
	}
}

void * List_GetDataByIndex(linkedlist_t *lst, int index)
{
    list_node_t *p;
    unsigned int i;
	if(index >= lst->count)
        return NULL;
    int nindex = index < 0 ? lst->count + index : index;
    if(nindex >= lst->count || nindex < 0)
        return NULL;
    LIST_FOREACH_WITH_COUNTER(p, i, lst)
	{
        if(i == nindex)
            break;
	}
	return p ? p->data : NULL;
}

void * List_TakeOneByIndex(linkedlist_t *lst, int index)
{
    list_node_t *p;
    unsigned int i;
	if(index >= lst->count)
        return NULL;
    int nindex = index < 0 ? lst->count + index : index;
    if(nindex >= lst->count || nindex < 0)
        return NULL;
    LIST_FOREACH_WITH_COUNTER(p, i, lst)
	{
        if(i == nindex)
		{
            list_node_t *last = p->prev;
            list_node_t *next = p->next;
			if(last)
				last->next = next;
			if(next)
				next->prev = last;
            if(i == 0)
                lst->data = next;
            if(p == lst->last)
                lst->last = last;
			void *r = p->data;
            free(p);
			lst->count--;
			return r;
        }
	}
	return NULL;
}

void * List_TakeOneByData(linkedlist_t *lst, const void *d)
{
    list_node_t *p;
    LIST_FOREACH(p, lst)
    {
        if(List_DataCmp(lst, p->data, d))
		{
            list_node_t *last = p->prev;
            list_node_t *next = p->next;
			if(last)
				last->next = next;
			if(next)
				next->prev = last;
            if(p == lst->data)
                lst->data = next;
            if(p == lst->last)
                lst->last = last;
			void *r = p->data;
            free(p);
			lst->count--;
			return r;
        }
	}
	return NULL;
}

void * List_TakeOneByNode(linkedlist_t *lst, const list_node_t *node)
{
    list_node_t *p;
    LIST_FOREACH(p, lst)
    {
        if(p == node)
        {
            list_node_t *last = p->prev;
            list_node_t *next = p->next;
            if(last)
                last->next = next;
            if(next)
                next->prev = last;
            if(p == lst->data)
                lst->data = next;
            if(p == lst->last)
                lst->last = last;
            void *r = p->data;
            free(p);
            lst->count--;
            return r;
        }
    }
    return NULL;
}

void * List_Front(linkedlist_t *lst)
{
    if(!lst->data)
		return NULL;

	return lst->data->data;
}

void * List_Back(linkedlist_t *lst)
{
    if(!lst->last)
        return NULL;

    return lst->last->data;
}

void * List_Find(linkedlist_t *lst, linkedlist_cmp_func_f cmp, const void *user_data, int *index)
{
    list_node_t *p;
	void *r;
    int i;

	r = NULL;
    LIST_FOREACH_WITH_COUNTER(p, i, lst)
	{
		if(cmp(p->data, user_data) == 0)
		{
			r = p->data;
			break;
        }
	}

	if(index)
		*index = i;
	return r;
}

int List_PopFront(linkedlist_t *lst)
{
    list_node_t *p;

    p = lst->data;
    if(!p)
		return 0;

	lst->data = p->next;
	if(p->next)
		p->next->prev = NULL;
    if(p == lst->last)
        lst->last = NULL;

	free(p->data);
	free(p);
	lst->count--;

	return lst->count;
}

int List_PopBack(linkedlist_t *lst)
{
    list_node_t *p;

    p = lst->last;
    if(!p)
        return 0;

    list_node_t *last = p->prev;
    if(last)
        last->next = NULL;
    lst->last = last;
    if(p == lst->data)
        lst->data = NULL;

    free(p->data);
    free(p);
    lst->count--;
	return lst->count;
}

void * List_ToArray(linkedlist_t *lst)
{
    const list_node_t *p;
	char *r;
	const void *d;
	unsigned int i;

    r = NULL;

    if(lst->count == 0)
		return r;

	r = calloc(lst->count, lst->size);
    LIST_FOREACH_WITH_COUNTER(p, i, lst)
    {
        d = p->data;
        memcpy(r + (lst->size * i), d, lst->size);
	}

	return r;
}

unsigned int List_ToArrayV(linkedlist_t *lst, void *r, unsigned int max)
{
    list_node_t *p;
	unsigned int count;
	const void *d;
	char *cr;
	unsigned int i;

	cr = r;
    count = 0;

    if(lst->count == 0)
		return count;

    LIST_FOREACH_WITH_COUNTER(p, i, lst)
    {
        if(count + lst->size > max)
            break;
        d = p->data;
        memcpy(cr + (lst->size * i), d, lst->size);
        count += lst->size;
    }

	return count;
}
