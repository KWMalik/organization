from random import random

class Empty:
   pass

class List:
   def __init__(self, head, tail):
      self.hd = head
      self.tl = tail
   def __repr__(self):
      if self.tl == Empty:
         return 'List(' + repr(self.hd) + ', Empty)'
      else:
         return 'List(' + repr(self.hd) + ', ' + repr(self.tl) + ')'
class Card:
         def __init__(self, suit, rank):
            self.suit = suit
            self.rank = rank
#Given a hand of cards, the helper function will count how many times a given suit is seen.
#When all cards have been seen, a list containing the sum of how many times each suit occurred is generated.
#The program will end after returning a string containing the value of the suit which occured most often in the hand.            
def dominant_suit(hand):
    def count_helper(diamonds,clubs,hearts,spades):
       random.choice("diamonds","clubs","hearts","spades")
       if random.choice=="diamonds":
          return count_helper(diamonds+1,hand-1)
         elif random.choice=="clubs":
            return count_helper(clubs+1,hand-1)
         elif random.choice=="hearts"
            return count_helper(hearts+1,hand-1)
         else:
            return count_helper(spades+1,hand-1)
if hand<= 0:
   return List(sum count_helper(diamonds),sum count_helper(clubs),sum count_helper(hearts),sum count_helper(spades))
print "the dominant suit is," max(List)
         
       
#Taking 3 parameters, the Tabulate function returns the result of applying any declared function to given values "a," and "b".
# In addition, the values of"a" and "b" are changed as the function recurses.  
def Tabulate(f,a,b):
   if a and b=Empty:
      return [0]
   elif a<=b:
      return List(Tabulate(f(a),f(a+2),f(b))

#Returns the results of applying a function to the "Xth" elements within a given list.
#These results are in turn placed within another list.                  
def zipwith(f,L1,L2):
    for x in L1 and L2:
        return List(L1[f(x)] + L2[f(x)])          
               
   
   
      
            
             
           
        
            
        
    
