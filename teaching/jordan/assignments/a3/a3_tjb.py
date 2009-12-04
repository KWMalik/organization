"""
0) All of the problems on this assignment must be solved recursively.
  All of the solutions you provide in this assignment will be based on the
  List and Empty classes that were defined in lectures:

class Empty:
   pass

class List:
   def __init__(self, head, tail):
      self.hd = head
      self.tl = tail
"""



"""
1) Write a function called sum that takes a List object and returns the sum of
  the elements in the List.  For example, sum(List(1, List(4, List(7, Empty))))
  should return 12.  You may assume that List used as the argument of sum
  will only contain numbers.
"""

class Empty:
    pass

class List:
    def __init__(self, head, tail):
        self.hd = head
        self.tl = tail
    def __str__(self):
        if self.tl == Empty:
            return 'List(%d, Empty)' self.hd
        else:
            return 'List(%d, %s)' % (self.hd, self.tl.__str__())


def sum(L):
    if L.tl == Empty:
        return L.hd
    else:
        return L.hd + sum(L.tl)

#Test cases
#list = List(1, List(2, List(3, List(4, List(5, Empty)))))
#print sum(list)


"""
2) Write a function called prod_odd that takes a List object and returns the
  product of all of the odd elements in the given List.  For example,
  prod_odd(List(1, List(2, List(3, List(5, List(6, Empty)))))) should return
  1 * 3 * 5 = 15.  You may assume that the given List object contains only
  integer entries.
"""
def isOdd(x):
    if x % 2 == 0:
        return False
    return True
    
def prod_odd(L):
    if L.tl == Empty:
        return L.hd
    else:
        if isOdd(L.hd):
            return L.hd * prod_odd(L.tl)
        else:
            return prod_odd(L.tl)
        
#Test cases
#list = List(1, List(2, List(3, List(4, List(5, Empty)))))
#print prod_odd(list)


"""
3) Write a function called contains_evens that takes a List object and returns
  True if the List contains at least one even element, and False otherwise.
  (If the List is empty, the result should be False.)  You may assume that the
  given List object contains only integer entries.
"""
def isEven(x):
    if x % 2 == 0:
        return False
    return True

def contains_even(L):
    if not isOdd(L.hd):
        return True
    elif L.tl == Empty:
        return False
    else:
        return contains_even(L.tl)

list1 = List(1, List(3, List(5, List(7, List(8, Empty)))))
list2 = List(0, List(3, List(5, List(7, List(9, Empty)))))
list3 = List(1, List(3, List(100, List(7, List(8, Empty)))))
list4 = List(1, List(3, List(5, List(7, List(9, Empty)))))

#Test cases
#print "Results should be True, True, True, False"
#print contains_even(list1)
#print contains_even(list2)
#print contains_even(list3)
#print contains_even(list4)
        

"""
4) This question uses the two-dimensional Point class from lectures in addition
  to the classes Empty and List above.  Write a function called largest_coords
  that takes a List Object of Point objects as a parameter, and returns a List
  object of the larger of the two coordinates for each Point object.  For
  example,
  largest_coords(List(Point(1,4), List (Point(5,2), List(Point(8,7), Empty))))
  should return a List object equivalent to List(4, List(5, List(8, Empty))).
"""
class Point:
    def __init__(self, x, y):
        self.x = x
        self.y = y
    def __str__(self):
        return "Point(%d, %d)" % (self.x , self.y)
    def largest(self):
        if self.x >= self.y:
            return self.x
        else:
            return self.y

#Helper debugging function
def print_list(L):
    if L.tl == Empty:
        print (("List(", L.hd, ", Empty)"))
    else:
        print(("List(%d,", L.hd, print_list(L.tl)))

def largest_coords(L):
    if L.tl == Empty:
        return List(L.hd.largest(), Empty)
    return List(L.hd.largest(), largest_coords(L.tl))



list = List(Point(1,4), List(Point(5,2), List(Point(8,7), Empty)))

#point1 = Point(1,4)
#point2 = Point(3,4)
#print(point1)
#print(point2.__str__())

#print_list(list)
#print_list(largest_coords(list))

print(list)







