#!/usr/bin/python -tt

import Levenshtein
import Compat

def dictimp(address):
    f=open(address,'rU')
    diz={}
    lista=[]
    for word in f:
        lista.append(word[:-1])
    for word in sorted(lista):
        diz[word]=0
    return diz
    f.close()
    
def bestword(diz,last, end):
    dist=100
    nowdist=0
    ans=''
    for word in diz:
        if diz[word]==0:
            if Compat.test(last,word):
                nowdist=Levenshtein.distance(word,end)
                if nowdist<dist:
                    ans=word
                    diz[word]=1
                    dist=nowdist
                    
    return ans
    
def main():
    diz={}
    diz=dictimp('dizionario.txt')
    begin='prova'
    end='proviamo'
    solution=[]
    solution.append(begin)
    while not Compat.test(solution[-1],end):
        next=bestword(diz,solution[-1], end)
        while next=='':
            if solution[-1]==begin: return 0
            del solution[-1]
            next=bestword(diz,solution[-1], end)
        solution.append(next)
        print solution    
        

if __name__=='__main__':
    main()
