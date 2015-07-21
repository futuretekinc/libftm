#ifndef	__FTM_LIST_H__
#define __FTM_LIST_H__

#include "ftm_types.h"

typedef struct FTM_LIST_STRUCT * FTM_LIST_PTR;

typedef	FTM_INT	(*FTM_LIST_ELEM_seeker)(const FTM_VOID_PTR pElement, const FTM_VOID_PTR pIndicator);
typedef	FTM_INT	(*FTM_LIST_ELEM_comparator)(const FTM_VOID_PTR pElement, const FTM_VOID_PTR pIndicator);

struct FTM_LIST_STRUCT *FTM_LIST_create(void);

FTM_RET FTM_LIST_init(struct FTM_LIST_STRUCT * pList);
FTM_RET	FTM_LIST_destroy(struct FTM_LIST_STRUCT * pList);

FTM_RET	FTM_LIST_append(struct FTM_LIST_STRUCT * pList, FTM_VOID_PTR pItem);

FTM_RET	FTM_LIST_remove(struct FTM_LIST_STRUCT * pList, FTM_VOID_PTR pItem);
FTM_RET	FTM_LIST_removeAt(struct FTM_LIST_STRUCT * pList, FTM_ULONG ulPosition);

FTM_RET	FTM_LIST_get(struct FTM_LIST_STRUCT * pList, FTM_VOID_PTR pKey, FTM_VOID_PTR _PTR_ ppElement);
FTM_RET	FTM_LIST_getAt(struct FTM_LIST_STRUCT * pList, FTM_ULONG ulPosition, FTM_VOID_PTR _PTR_ ppElement);

FTM_RET	FTM_LIST_iteratorStart(struct FTM_LIST_STRUCT * pList);
FTM_RET	FTM_LIST_iteratorNext(struct FTM_LIST_STRUCT * pList, FTM_VOID_PTR _PTR_ ppElement);

FTM_ULONG	FTM_LIST_count(struct FTM_LIST_STRUCT * pList);

FTM_RET FTM_LIST_setSeeker(struct FTM_LIST_STRUCT * pList, FTM_LIST_ELEM_seeker fSeeker);
FTM_RET FTM_LIST_setComparator(struct FTM_LIST_STRUCT * pList, FTM_LIST_ELEM_comparator fComparator);
#endif

