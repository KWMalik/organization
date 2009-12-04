class Empty:
   pass

class List:
   def __init__(self, head, tail):
      self.hd = head
      self.tl = tail
   
def List(L):
   if L==Empty
      return Empty
   else:
      return List(L.hd + L.tl)

#Returns a List containing all natural numbers by which "n" is divisible.
#List begins from "m" and counts down until 2.      
def divisors(m,n):
   if n % m-1==0:
      return List(divisors(n/m-1)
   else:
                  pass
                        if m<=2:
                  return "finished"
   
    

#Returns True if a given number is Prime.    
def is_prime(n):
   def divisors(m,n):
                  
               
    
 #Will return items in a list up to "n".  
def take(L,n):
   return List(L.hd*n)take(L.tl)
                  if n>=L:
                     return List

   
 #Will return all numbers within a list, excluding those up to "n."  
def drop(L,n):
   return [L:n]

#Will return a List excluding the "Nth" element.
n>=0
def exclude(L,n):
   return List and not (L.hd+n-1)
   
