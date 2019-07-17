def convert_to_boolean(string):
    return_bool = False
    if string == 'True':
        return_bool == True
    return return_bool
    
a, b = input().split()
bool1 = convert_to_boolean(a)
bool2 = convert_to_boolean(b)

result = lambda a, b: a ^ b

print(result(bool1, bool2))
