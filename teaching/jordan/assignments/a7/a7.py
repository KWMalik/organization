class Empty:
   pass

class List:
   def __init__(self, head, tail):
      self.hd = head
      self.tl = tail

 class Contact:
         def __init__(self, name, address):
            self.name = name
            self.addr = addr
         def __repr__(self):
            return 'Contact(' + self.name + ', ' + self.address + ')'

class DuplicateError (Exception):
		def __init__ (self, msg):
			self.msg = msg
		def __str__ (self):
			return self.msg
		def __repr__ (self):
			return 'DuplicateError(' + msg + ')'

def map(f,L):
if L == Empty:
return Empty
else:
return List(f(L.hd),map(f,L.tl))

def filter(f, l):
      if l == Empty:
             return Empty
      elif f(l.hd): 
             return List(l.hd, filter(l, l.tl)
      else:
             return filter(l, l.tl)  

#Looks up a name in an address book and returns all names associated with that name.         
def lookup(L,name):
   if L==Empty:
      return List(Empty)
   elif L.hd.name==name:
      return List(L.hd.addr,Empty)
   elif L.hd.addr>1:
                         return filter(lookup,L) 
   
#Goes through a list and raises a Duplicate Error if more than one of the same declared element is found.    
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
      for e in L:
                         e=e+1
      if e>=e+2:
                         raise DuplicateError("Duplicates found.")




def unzip(x,L):
     for x in L:
                         
                         
                         


def builtin_unzip(L):
def insert(e,L):
                            
           
