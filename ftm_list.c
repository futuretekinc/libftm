#include "ftm_error.h"
#include "ftm_debug.h"
#include "ftm_list.h"
#include "ftm_mem.h"
#include "simclist.h"

typedef	struct FTM_LIST_STRUCT
{
	list_t		xList;	
	FTM_BOOL	bIterator;
}	FTM_LIST;


FTM_LIST_PTR FTM_LIST_create(void)
{
	FTM_LIST_PTR	pList;

	pList = FTM_MEM_malloc(sizeof(FTM_LIST));
	if (pList == NULL)
	{
		return	NULL;
	}

	list_init(&pList->xList);
	pList->bIterator = FTM_FALSE;
	
	return	pList;
}

FTM_RET FTM_LIST_init(struct FTM_LIST_STRUCT * pList)
{
	ASSERT(pList != NULL);

	list_init(&pList->xList);
	pList->bIterator = FTM_FALSE;
	
	return	FTM_RET_OK;
}

FTM_RET	FTM_LIST_destroy(struct FTM_LIST_STRUCT * pList)
{
	ASSERT(pList != NULL);

	list_destroy(&pList->xList);

	return	FTM_RET_OK;
}


FTM_RET	FTM_LIST_append(struct FTM_LIST_STRUCT * pList, FTM_VOID_PTR pItem)
{
	FTM_INT	nRet;

	ASSERT((pList != NULL) && (pItem != NULL));

	nRet = list_append(&pList->xList, pItem);
	if (nRet < 0)
	{
		ERROR("list_append error[%d]\n", nRet);
		return	FTM_RET_ERROR;	
	}

	return	FTM_RET_OK;
}

FTM_RET	FTM_LIST_remove(struct FTM_LIST_STRUCT * pList, FTM_VOID_PTR pItem)
{
	ASSERT((pList != NULL) && (pItem != NULL));

	if (list_delete(&pList->xList, pItem) < 0)
	{
		return	FTM_RET_ERROR;
	}

	return	FTM_RET_OK;
}

FTM_RET	FTM_LIST_removeAt(struct FTM_LIST_STRUCT * pList, FTM_ULONG ulPosition)
{
	ASSERT(pList != NULL);

	if (list_delete_at(&pList->xList, ulPosition) < 0)
	{
		return	FTM_RET_ERROR;	
	}

	return	FTM_RET_OK;
}

FTM_RET	FTM_LIST_get(struct FTM_LIST_STRUCT * pList, FTM_VOID_PTR pKey, FTM_VOID_PTR _PTR_ ppElement)
{
	FTM_VOID_PTR	pElement;

	ASSERT((pList != NULL) && (pKey != NULL));

	pElement = list_seek(&pList->xList, pKey);
	if (pElement == NULL)
	{
		return	FTM_RET_OBJECT_NOT_FOUND;	
	}

	if (ppElement != NULL)
	{
		*ppElement = pElement;
	}

	return	FTM_RET_OK;
}

FTM_RET	FTM_LIST_getAt(struct FTM_LIST_STRUCT * pList, FTM_ULONG ulPosition, FTM_VOID_PTR _PTR_ ppElement)
{
	FTM_VOID_PTR	pElement;

	ASSERT(pList != NULL);

	pElement = list_get_at(&pList->xList, ulPosition);
	if (pElement == NULL)
	{
		return	FTM_RET_OBJECT_NOT_FOUND;	
	}

	if (ppElement != NULL)
	{
		*ppElement = pElement;
	}

	return	FTM_RET_OK;
}

FTM_RET	FTM_LIST_iteratorStart(struct FTM_LIST_STRUCT * pList)
{
	ASSERT(pList != NULL);

	if (pList->bIterator == FTM_TRUE)
	{
		list_iterator_stop(&pList->xList);
		pList->bIterator = FTM_FALSE;
	}

	if (list_iterator_start(&pList->xList) < 0)
	{
		return	FTM_RET_OBJECT_NOT_FOUND;	
	}
	pList->bIterator = FTM_TRUE;

	return	FTM_RET_OK;
}

FTM_RET	FTM_LIST_iteratorNext(struct FTM_LIST_STRUCT * pList, FTM_VOID_PTR _PTR_ ppElement)
{
	ASSERT((pList != NULL) && (ppElement != NULL));
	
	*ppElement = list_iterator_next(&pList->xList);
	if (*ppElement == NULL)
	{
		list_iterator_stop(&pList->xList);
		pList->bIterator = FTM_FALSE;

		return	FTM_RET_OBJECT_NOT_FOUND;	
	}

	return	FTM_RET_OK;
}

FTM_ULONG FTM_LIST_count(struct FTM_LIST_STRUCT * pList)
{
	ASSERT(pList != NULL);
	
	return	list_size(&pList->xList);
}

FTM_RET FTM_LIST_setSeeker(struct FTM_LIST_STRUCT * pList, FTM_LIST_ELEM_seeker fSeeker)
{
	ASSERT(pList != NULL);
	ASSERT(fSeeker != NULL);

	list_attributes_seeker(&pList->xList, fSeeker);

	return	FTM_RET_OK;
}


FTM_RET FTM_LIST_setComparator(struct FTM_LIST_STRUCT * pList, FTM_LIST_ELEM_comparator fComparator)
{
	ASSERT(pList != NULL);
	ASSERT(fComparator != NULL);

	list_attributes_comparator(&pList->xList, fComparator);

	return	FTM_RET_OK;
}


