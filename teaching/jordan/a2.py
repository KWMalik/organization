# Takes 2 values and returns True if only one parameter is true.

def but_not_both (x,y):
    if x and not y >=10:
        return True
    else:
        return False

but_not_both(10,9)

#Defines a Class that will initialize fields x,y,and z.
class points:
    def _init_(self,x,y,z):
        self.x = x
        self.y = y
        self.z = z

#Takes 2 points and calculates the distance between them known as "k".
    def interpolate(p1,p2,k):
        return points(k*(p1.x-p2.y)+p1.x+p1.y)

interpolate (3,4,0.5)

#Accepts 3-dementional Points and returns the type of triangle they form.

def classify_triangle (p1,p2,p3):
    if p1 != p2 and p3:
        return "scalene"
    if p1 == p2 or p3:
        return "isosceles"
    if p1== p2 and p3:
        return "equilateral"
    else:
        return False
    


    


        




