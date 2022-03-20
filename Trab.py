# -*- coding: utf-8 -*-
"""
Created on Sat Mar 19 20:11:06 2022

@author: sergi
"""
import sys
import math
class Vertice:
    def __init__(self, x, y):
        self.x = x
        self.y = y
        
        
######################################
#PODE REFATORAR AS COISA AI SE QUISER#
######################################


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
        list_vertice.append(Vertice(int(element[0]), int(element[1])))
    return list_vertice

# Inicializa vertices como não visitado
# Nao sei se pode iniciar com um visitado/ nao visitado na classe
# coloquei aqui como : [[Vertice,visitado],...]            
def init_false(list_vertice):
    list_aux = []
    for vertice in list_vertice:
        list_aux.append([vertice,False])
    return list_aux    

#ACHEI MUITO ESQUISITO ISSO AQUIM DEVE TER COMO FAZER MELHOR
# percorre os não visitados, acha o menor
def find_nearest(visited_list, vertice_index):
    # acha o primeiro não visitado e assume como o menor
    count = 0
    while visited_list[count][1] == True:
        count+=1
        
    min_index = count
    min_value = math.dist([visited_list[vertice_index][0].x, visited_list[vertice_index][0].y], [visited_list[min_index][0].x, visited_list[min_index][0].y])
    count2 = 0

    # percorre outros vertices procurando um menor
    for vertice in visited_list:
        if (vertice[1] == False and math.dist([visited_list[vertice_index][0].x, visited_list[vertice_index][0].y], [vertice[0].x,vertice[0].y]) < min_value):
            min_value = math.dist([visited_list[vertice_index][0].x, visited_list[vertice_index][0].y], [vertice[0].x, vertice[0].y])
            min_index = count2 
        count2+=1    
    return min_index

# Metodo construtivo 
# Neareast Neighbor
def nearest(list_vertice):
    path = []
    
    visited_list = init_false(list_vertice)
    # Inicia no primeiro vertice
    visited_list[0][1] = True
    path = [0]
    #print(visited_list)
    for i in range(len(list_vertice)-1):
        near = find_nearest(visited_list, path[i])
        path.append(near)
        visited_list[near][1] = True
    print(path)
    

def main():
    content = read_input()
    content = remove_six(content)
    list_vertice = populate(content)
    nearest(list_vertice)
    #print(list_vertice)
if __name__ == '__main__':
   main()