
"""
2) In this question, you will use the List and Empty classes from lectures.

   In question 2 of assignment 7, you implemented the first steps toward a
   program that will check whether a list contains duplicate entries.  Now
   that you know how to handle exceptions, you can finish that program.

   Begin with your solution to assignment 7, question 2 -- a function called
   insertion_sort that raises an exception if it ever encounters a duplicate.
   Use this function as a helper for your function called contains_duplicates.
   The function contains_duplicates will take a List object L as a parameter
   and return True if L contains duplicate entries, and False if it does not.
"""

class Empty:
	pass

class List:
	def __init__(self, head, tail):
		self.hd = head
		self.tl = tail

def insertion_sort(L):
   def insert(e, L):
	  if L == Empty:
		 return List(e, Empty)
	  elif e < L.hd:
		 return List(e, L)
	  else:
		 return List(L.hd, insert(e, L.tl))
   if L == Empty:
	  return Empty
   else:
	  return insert(L.hd, insertion_sort(L.tl))




 
"""
# Goes through a list and raises a Duplicate Error 
# if more than one of the same declared element is found.


def insert(e, L):
	if L == Empty:
		return List(e, Empty)
	elif e < L.hd:
		return List(e, L)
	else:
		return List(L.hd, insert(e, L.tl))



def insertion_sort(L):
	if L == Empty:
		return Empty
	else:
		return insert(L.hd, insertion_sort(L.tl))
	for e in L:
		e=e+1
		if e>=e+2:
			raise DuplicateError("Duplicates found.")

"""


"""
2) Suppose you want to check whether a list contains duplicate entries.  One
  way to do that is to attempt to sort the list by insertion sort, and for
  each item, when trying to insert it in the result, if the comparison between
  the current item and an item in the result ever comes out even, the list
  obviously contains duplicates.

  For your convenience, the insertion sort function (minus the functional
  parameter lessthan) is reproduced below:

    def insertion_sort(L):
       def insert(e, L):
          if L == Empty:
             return List(e, Empty)
          elif e < L.hd:
             return List(e, L)
          else:
             return List(L.hd, insert(e, L.tl))
       if L == Empty:
          return Empty
       else:
          return insert(L.hd, insertion_sort(L.tl))

  You will modify this implementation as described above -- if the comparison
  ever comes out equal, you will raise an exception, signalling that a
  duplication has been found.  Declare an appropriate exception class to
  describe this signal.  Include at least a message field that would be
  displayed automatically by Python.

  When we discuss how to recover from exceptions, we will see how to write
  a function that responds to the exception, and uses it to answer True to
  the question of whether the list contains duplicates.  You do not need to
  worry about this issue on this assignment.
"""














