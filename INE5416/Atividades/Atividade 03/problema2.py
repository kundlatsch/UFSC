a, b, c = map(int, input().split())

media = lambda a, b, c: "Aprovado" if ((a + b + c)/3) >= 6 else "Reprovado"

print(media(a, b, c))