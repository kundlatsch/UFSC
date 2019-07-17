import math

p1 = [int(n) for n in input().split()]
p2 = [int(n) for n in input().split()]


distance = lambda p1, p2: math.sqrt(((p2[0] - p1[0]) ** 2) + ((p2[1] - p1[1]) ** 2) + ((p2[2] - p1[2]) ** 2))

print(distance(p1, p2))