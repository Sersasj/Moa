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

def populate(text):
    list_vertice = []
    text = text.split('\n')
    text.remove("")
    for element in text:
        element = element.split(" ")
        element = element[1:3]  
        list_vertice.append(Vertice(element[0], element[1]))
    return list_vertice
            
        
    


def main():
    content = read_input()
    content = remove_six(content)
    list_vertice = populate(content)
    print(list_vertice)
if __name__ == '__main__':
   main()