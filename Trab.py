# -*- coding: utf-8 -*-
"""
Created on Sat Mar 19 20:11:06 2022

@author: sergi
"""
import sys

class Vertice:
    def __init__(self, x, y):
        self.x = x
        self.y = y



# Armazena entrada 
def read_input():
    with open(sys.argv[1], 'r') as f:
        content = f.read()
    return content

# Remove 6 primeiras linhas e EOF / horripilante
def remove_six(text):  
    count_n = 0
    count_qnt = 0
    for x in text:
        count_qnt += 1
        if x == '\n':
            count_n += 1
        if count_n == 6:
            break
        
    return text[count_qnt:len(text)-3]

# Cria lista com os Vertices
def populate(text):
    list_vertice = []
    text = text.split('\n')
    text.remove("")
    for element in text:
        element = element.split(" ")
        element = element[1:]  
        list_vertice.append(Vertice(element[0], element[1]))
    return list_vertice

# Inicializa vertices como não visitado
# Nao sei se pode iniciar com um visitado/ nao visitado na classe
# coloquei aqui como : [[Vertice,visitado],...]            
def init_false(list_vertice):
    list_aux = []
    for vertice in list_vertice:
        list_aux.append([vertice,False])
    return list_aux    

# percorre os não percorridos, acha o menor
def find_nearest(visited_list):
    
    
    print("a")                
    
# Metodo construtivo 
# Neareast Neighbor
def nearest(list_vertice):
    path = []
    
    visited_list = init_false(list_vertice)
    # Inicia no primeiro vertice
    visited_list[0][1] = True
    path = [0]
    print(visited_list)
    #while len(path) < len(list_vertice):
       # print("a")
    

def main():
    content = read_input()
    content = remove_six(content)
    list_vertice = populate(content)
    nearest(list_vertice)
    #print(list_vertice)
if __name__ == '__main__':
   main()