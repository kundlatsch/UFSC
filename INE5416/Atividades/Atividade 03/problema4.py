a, b, c = map(int, input().split())

bhaskara1 = lambda a, b, c: (b + ((((b * b) - (4 * a * c))) ** 1/2 ))/(2*a)
bhaskara2 = lambda a, b, c: (b - ((((b * b) - (4 * a * c))) ** 1/2 ))/(2*a)

print(bhaskara1(a, b, c))
print(bhaskara2(a, b, c))