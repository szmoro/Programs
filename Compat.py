def anagr(s1,s2):
    if sorted(s1)==sorted(s2): return 1
    return 0
    
def chdiff(s1,s2):
    l=len(s1)
    count=0
    if l!=len(s2): return 0
    for i in range(l):
        if s1[i]!=s2[i]: count+=1
        if count>1: return 0
    return 1
    
def chplusminus(s1,s2):
    l1=len(s1)
    l2=len(s2)
    if abs(l1-l2)>1: return 0
    if l1==l2: return chdiff(s1,s2)
    elif l1>l2:
        s3=s1[:-1]
        s4=s2
    elif l2>l1:
        s4=s2[:-1]
        s3=s1
    return chdiff(s3,s4)    
    
def test(s1,s2):
    if anagr(s1,s2): return 1
    if chdiff(s1,s2): return 1
    if chplusminus(s1,s2): return 1
    if chplusminus(s1,s2): return 1
    return 0
