import pickle
import os

def save(object):
    type = object.__class__.__name__.lower()
    file = './saves/' + object.name + '.' + type + '.save'
    
    with open(file, 'wb') as object_save:
        pickle.dump(object, object_save)

def load(type, object):
    file = object + '.' + type + '.save'
    
    with open(file, 'rb') as object_save:
        loaded = pickle.load(object_save)
        return loaded

def load_all(type):
    return_dict = {}
    if not os.path.exists('./saves'):
        os.mkdir('saves')
    else:
        files = os.listdir('saves')
        for f in files:

            splited_name = f.split('.')

            if splited_name[1] == type:
                loaded = pickle.load(open('./saves/' + f, "rb"))
                return_dict[loaded.name] = loaded

    return return_dict