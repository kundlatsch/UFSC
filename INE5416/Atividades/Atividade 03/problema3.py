a = int(input())

fibonacci = lambda i: fibonacci(i - 1) + fibonacci(i - 2) if i > 2 else 1

print(fibonacci(a))