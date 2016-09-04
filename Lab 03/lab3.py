# -*- coding: utf-8 -*-
"""
Created on Wed May 20 21:45:54 2015

@author: briantaylor
"""

from random import randint

MIN = 0
MAX = 1000000

A10 = []
for i in range(10):
    A10.append(randint(MIN, MAX))

A100 = []
for i in range(100):
    A100.append(randint(MIN, MAX))

A1000 = []
for i in range(1000):
    A1000.append(randint(MIN, MAX))
    

def bs(A):
    """
    List -> NoneType
 
    Effect: modifies A so that elements are in order.
 
    >>> A = [4, 2, 1, 8, -3]
    >>> bs(A)
    >>> exp = [-3, 1, 2, 4, 8]
    >>> A == exp
    True
    """
    numComps = 0
    swapped = True
    while (swapped):
        swapped = False
        print("Entering loop:")
        print(A)
        for i in range(len(A)-1) :
            numComps += 1
            if (A[i] > A[i+1]) :
                A[i+1], A[i] = A[i], A[i+1]
                swapped = True
                print("A swap takes place:")
                print(A)
    print(numComps)
    
        
def bs_optimized(A):
    """
    List -> NoneType
    
    Effect: modifies A so that elements ar in order.
    
    >>> A = [4, 2, 1, 8, -3]
    >>> bs_optimized(A)
    >>> exp = [-3, 1, 2, 4, 8]
    >>> A = exp
    True
    """
    numComps = 0
    n = len(A)
    swapped = True
    while (swapped):
        n -= 1
        swapped = False
        print("Entering loop:")
        print(A)
        for i in range(n) :
            numComps += 1
            if (A[i] > A[i+1]) :
                A[i+1], A[i] = A[i], A[i+1]
                swapped = True
                print("A swap takes place:")
                print(A)
    print(numComps)
    
    
    











    

# Make sure doctest run when file is run but not imported

#if __name__=='__main__':
#    import doctest
#    EPS = 1.0e-6
#    print(doctest.testmod(verbose=False))