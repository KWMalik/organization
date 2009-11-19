"""
	Helper Functions
"""		
def Test(result):
	if not result:
		print 'Test Failed.'

"""
	Question #1
"""		
def Odd(x):
	if x % 2 == 0:
		return False
	else:
		return True

def func(L):
	result = 1
	for i in range(len(L)):
		if Odd(i):
			result = result * L[i]
	return result

#L = [1, 2, 45, 5, 67]
#print func(L)


"""
	Question #2
	
	2) In this question, you will use the List and Empty classes from lectures.

   Rewrite the function deep_copy_int_list from lectures using map instead
  of explicit recursion.
  
  
Let's look at shallow and deep copies of lists that we created.

Shallow copy:

def shallow_copy(L):
	return List(L.hd, L.tl)

The results of this will not be an alias, but the pieces inside of it will be.

Deep copy:

def deep_copy_int_list(L): # this is a list of integers, so you don't need to worry about copy what's in the list!
	if L == Empty
		return Empty
	else:
		return List(L.hd, deep_copy_int_list(L.tl))

This is probably one of the simplest recursive list function we've ever built! Hooray1

If they were not integers, we would have to be deep copying all the elements of the list! Eeek!
"""

def map (f, L):		# f = a functions, L = a list
	if L == Empty:
		return Empty
	else:
		return List (f (L.hd), map (f, L.tl))




















"""
	Question #3
	
	3) In this question, you will use the List and Empty classes from lectures.

   Write a general deep copying routine for List objects that also creates
  deep copies of the elements in the List, in addition to the List structure
  itself.  Because the exact steps needed to make a deep copy of a data
  structure depend on what the data structure itself looks like (for example,
  deep copying a Contact is not the same as deep copying a LineSeg), your
  function will take an additional function as parameter, which knows how to
  create deep copies of the elements of the list.  For example, the call

   deep_copy_list(List(LineSeg(Point(1,2), Point(3,4)), \
      List(LineSeg(Point(5,6), Point(7,8)), Empty)), deep_copy_lineseg)

  would create a deep copy of the given List of LineSeg objects.
"""

