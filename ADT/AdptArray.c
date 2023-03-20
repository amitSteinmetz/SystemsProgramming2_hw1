#include "AdptArray.h"
#include <stdlib.h>
#include <string.h>

// FOR README: 
// what the adpt array should do:
//
//    1. create an empty array
 //   2. delete the array - release the memory (free?)
 //   3. set - receive an element and index and put a *copy* of it in the right location in the array
 //   4. get - receive an index and return a *copy* of that element at this index
 //   5. getSize - return array size (-1 if array didnt create)
  //  6. print - print the elements in the array


typedef struct AdptArray_
{
	int arr_length;
	PElement *pElem_arr;
	DEL_FUNC delFunc;
	COPY_FUNC copyFunc;
    PRINT_FUNC printFunc;
} AdptArray;

PAdptArray CreateAdptArray(COPY_FUNC copy_func, DEL_FUNC del_func,PRINT_FUNC print_func)
{
    // Allocate memory for the array
    PAdptArray pArr = (PAdptArray)malloc(sizeof(AdptArray));

    // If the allocation has failed, return null
    if (!pArr)
		return NULL;
    
    // Construct pArr fields
    pArr->arr_length = 0; // Initiall size == 0
	pArr->pElem_arr = NULL; // No elements in the arr at first
	pArr->delFunc = del_func; 
	pArr->copyFunc = copy_func;
    pArr->printFunc = print_func;

	return pArr;
}

Result SetAdptArrayAt(PAdptArray pArr, int index, PElement pNew_element)
{
    // Declare on a new arr, will replace the current arr in the AdptArray  (more comfortable to declare here)
    PElement* new_arr;

    // If the adptArray don't has allocated memory
    if (!pArr) return FAIL;

    // If the index is bigger then array size (also if equal to size, in order to avoid from out of bound error), 
    // we need to allocate new memory
    if (index >= pArr->arr_length)
    {
        // Allocate bigger memory (by 1)
		if (!(new_arr = (PElement*)calloc((index + 1),sizeof(PElement))))
			return FAIL;
        // Copy the old elements to new arr
		memcpy(new_arr, pArr->pElem_arr, (pArr->arr_length)*sizeof(PElement));
        // Free old address
		free(pArr->pElem_arr);
        // Update the field of arr in AdptArray to the address of new arr
		pArr->pElem_arr = new_arr;

        // Change arr_length to the new size
        pArr->arr_length = index + 1;
	}
  
    // Delete old element from arr (by calling delete function) - if exist
    if (pArr->pElem_arr[index]) pArr->delFunc((pArr->pElem_arr)[index]);
    
    // Save a *copy* of the new element at the same index
	(pArr->pElem_arr)[index] = pArr->copyFunc(pNew_element);

	return SUCCESS;
}

void DeleteAdptArray(PAdptArray pArr)
{
    // If there's no AdptArray to delete (we're checking if start address is null), we're stop here
	if (!pArr) return;

    // Hierarchy: first we delete every element - one by one. After that, we free the memory of array of elements,
    // and finally we free the memory of the whole struct

	for(int i = 0; i < pArr->arr_length; i++)
	{
        if (pArr->pElem_arr[i]) pArr->delFunc((pArr->pElem_arr)[i]);
	}

	free(pArr->pElem_arr);
	free(pArr);
}

PElement GetAdptArrayAt(PAdptArray pArr, int index)
{
    // If there's no element at that index, or index is out of bounds
    if ((!pArr->pElem_arr[index]) || index >= pArr->arr_length) return NULL;

    // Save a copy of requested element
    PElement element = pArr->copyFunc(pArr->pElem_arr[index]);

    return element;
}

int GetAdptArraySize(PAdptArray pArr)
{
    // If there's no allocated memory for the AdptArray
    if (!pArr) return -1;

    return pArr->arr_length;
}

void PrintDB(PAdptArray pArr)
{
    // If there's no allocated memory for the AdptArray
    if (!pArr) return;

    // Loop through all elements in the array, and print one by one, using pArr print function field
    for(int i = 0; i < pArr->arr_length; i++)
    {
        if(pArr->pElem_arr[i]) pArr->printFunc(pArr->pElem_arr[i]);
    }
}

