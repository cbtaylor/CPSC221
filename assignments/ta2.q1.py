# -*- coding: utf-8 -*-
"""
Created on Tue Jun  9 17:10:39 2015

@author: briantaylor
"""


def flip_n_pancakes(stack, n):
    """
    List Int -> NoneType
    
    Effect: returns stack so that top n elements have been flipped
    
    >>> A = [1, 2, 3, 4, 5]
    >>> flip_n_pancakes(A, 3)
    [3, 2, 1, 4, 5]

    """
    top = stack[:n]
    bottom = stack[n:]
    top.reverse()
    stack = top + bottom
    return stack
    
def find_largest_out_of_place(stack):
    """
    List -> Int
    
    Effect: returns index of largest element that has a smaller
            element below it
            returns -1 if elements are in order
    
    >>> A = [1, 4, 3, 2, 5]
    >>> find_largest_out_of_place(A)
    1
    
    >>> B = [3, 5, 6]
    >>> find_largest_out_of_place(B)
    -1
    """
    
    index_of_max = -1
    maxnum = 0
    for i in range(len(stack)-1):
        if stack[i] > min(stack[i+1:]):
            if stack[i] > maxnum:           
                maxnum = stack[i]
                index_of_max = i
    return index_of_max
    
def sort_stack(stack):
    """
    List -> List, Int
    
    Effect: returns a tuple of a sorted list of pancakes
            and the number of flips that took place
            assumes that pancakes are a permutation of
            the first n natural numbers (i.e. no zero)
    
    >>> A = [5, 3, 4, 1, 2]
    >>> sort_stack(A)
    ([1, 2, 3, 4, 5], 6)
    """
    numflips = 0
    sorted_stack = sorted(stack)
    if stack == sorted_stack:
        return stack, 0
    else:
        while stack != sorted_stack:
            n = find_largest_out_of_place(stack)
            stack = flip_n_pancakes(stack, n+1)
            numflips+=1
            stack = flip_n_pancakes(stack, stack[0])
            numflips+=1
        return stack, numflips
            
    
    
    
    
    
    
    
    
# Make sure doctest run when file is run but not imported

if __name__=='__main__':
    import doctest
    EPS = 1.0e-6
    print(doctest.testmod(verbose=False))