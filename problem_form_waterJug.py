# -*- coding: utf-8 -*-

"""
Water Jug Problem
We have two jugs, A and B, with capacities A liters and B liters respectively, A < B.
There are no measurements on the jugs. If we asssume infinite water supply, then how can we
measure exactly 'x' liters in jug-b where A<=x<=B?

Solution with AI
We can represent each state with ordered pair (juga,jugb) that indicate the current amount
of water in the jugs. Initially, we have (0,0) as a node. Our goal is (juga,x), where
0<=juga<=A.
Transition Model
1) (a,b) -> (A, b)
2) (a,b) -> (a,B)
3) (a,b) -> (a-d,b),a>0
4) (a,b) -> (a,b-d),b>0
5) (a,b) -> (0,b),a>0
6) (a,b) -> (a,0),b>0
7) (a,b) -> (A,b-(A-a)),a+b>=A,b>0
8) (a,b) -> (a-(B-b),B),a+b>=B,a>0
9) (a,b) -> (0,a+b),a+b<=B,a>0
10) (a,b) -> (a+b,0),a+b<=A,b>0
"""
def pour_water(juga, jugb):
    A, B, goal = 3,4,2
    print("%d\t%d" % (juga,jugb))
    if jugb==goal:
        return
    elif jugb == B:
        pour_water(0, juga)
    elif juga != 0 and jugb == 0:
        pour_water(0, juga)
    elif juga == goal:
        pour_water(juga, 0)
    elif juga < A:
        pour_water(A, jugb)
    elif juga < (B-jugb):
        pour_water(0,juga+jugb)
    else:
        pour_water(juga-(B-jugb), B)
    
print("JUG-A\tJUG-B")
pour_water(0,0)



