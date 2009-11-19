from random import random


def roulette_bust(n): #Represents an arbitrary amount of money you start with,"n".
    def monte_carlo(x): #Will generate a random number "x" On the roullet board.
        if n==0:  #If you have no money you cannot play.
            return 0
        else:
            x = random() #Otherwise, wins and losses in $1 at a time are calculated "n-1","n+1."
            if x <= 18.0/38.0:
                return monte_carlo(n + 1)
            else:
                return monte_carlo(n - 1)
if n <=0: #When money runs out, will calculate the grand total of wins and losses, giving total times played.
    return sum(monte_carlo(n-1) + sum(monte_carlo(n+1))
               
               
            

class Empty:
   pass

class List:
   def __init__(self, head, tail):
      self.hd = head
      self.tl = tail

class Point:
   def __init__(self, x, y):
      self.x = x
      self.y = y
            
def map(f,L):
    if L == Empty:
        return Empty
    else:
        return List(f(L.hd),map(f,L.tl))

def XS(L):
    return List(Point.x)#Returns a list containing X coordinates from a set of Point Objects.
def XS2(L):
    return map(lambda x : Point.x,L)#Recurses upon the XS function.
               
               
def YS(L):
    return List(Point.y)#Returns a list containing Y coordinates from a set of Point Objects.
def YS2(L):
    return map(lambda x : Point.y,L)#Recurses upon the YS function.

def Sum_Coords(L):
    return List(sum(Point.x + Point.y) #Returns the sum of x+y coordinates within a given list.
def Sum_Coords2(L):
    return map(lambda x : sum(Point.x + Point.y),L) #Recurses upon the Sum_Coords function.            
            
def Max_Coords(L):
    if Point.x > Point.y:
                return List(Point.x)
    else:
                return List(Point.y)
def reverse_coords(L):
    return List(list.reverse)            
