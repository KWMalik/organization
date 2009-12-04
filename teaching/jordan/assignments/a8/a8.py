"""
1) In lectures, we wrote a function that computes the product of all of the
  odd elements of a built-in list.  In this question, you will write a function
  called prod_odd_positions that computes the product of all of the elements in
  odd *positions* in a built-in list.  The odd positions are positions 1, 3, 5,
  etc., where positions are counted starting from 0.  For example, the call
      prod_odd_positions([2,4,5,1,6,9])
  should produce the value 4 * 1 * 9 (i.e., 36).

  You are to accomplish this task using a loop over the elements of the list.
"""

def Odd(x):
	"""
	"""
	if x % 2 == 0:
		return False
	else:
		return True

def prod_odd_positions(L):
	"""
	"""
	result = 1
	for i in range(len(L)):
		if Odd(i):
			result = result * L[i]
	return result


#list1 = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
#list2 = [11, 345, 63456, 23, 345, 7, 4, 123, 56]
#
#print '945 == %d' % prod_odd_positions(list1)
#print '6832035 == %d' % prod_odd_positions(list2)


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

class Empty:
	pass


class List:
	def __init__(self, head, tail):
		self.hd = head
		self.tl = tail


# f = a functions, L = a list
def map(f, L):		
	if L == Empty:
		return Empty
	else:
		return List(f(L.hd), map(f, L.tl))


# write a function that doubles
def printL(x):
	print x
	return x

def print_list(L):
	return map(printL, L)

def deep_copy(x):
	return x

# this is a list of integers, so you don't need to worry about copy what's in the list!
def deep_copy_int_list(L): 
	return map (deep_copy, L)


list1 = List(0, List(1, List(2, List(3, List(4, List(5, Empty))))))
print 'list1 = '
print_list(list1)


list2 = deep_copy_int_list(list1)
print 'list2 = '
print_list(list2)




"""


"""
