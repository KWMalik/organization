#Defines Classes for an empty list, and lays the base for a list with at
#least one object.

class Empty:
   pass

class List:
   def __init__(self, head, tail):
      self.hd = head
      self.tl = tail

class points:
    def _init_(self,x,y,):
        self.x = x
        self.y = y
#Returns the sum of elements within a given list.        
def sum(L):
   if List==Empty:
      return 0
   else:
      return L.hd + sum(L.tl)

#Returns the product of Odd numbers within a List.

def prod_odd(L):
   if List==Empty:
      return 0
   elif L.hd % 2==1:
      return L.hd*prod_odd(L.tl)
   


#Asseses wheather a given list contains even numbers, excluding empty lists.

def contains_evens(L):
def even(e):
   if e % 2==0
   return True
else:
   return False
if even in L:
   return True
else:
   return False
      


#Returns the largest of two coordinates in a list.

def largest_coords(L(P1,P2)):
   if L==Empty:
      return none
   elif P1>P2:
      return List(lagest_coords(P1))
   else:
      return List(largest_coords(P2))
   
   
#Returns True if each element in a list is at least as large as the last. If not, returns False.   
def is_ascending(L):
   if L==Empty or sum(L.hd + L.tl)==1:
      return True
   elif L.hd>=is_ascending(L.tl):
      return True
   else:
      return False
      

