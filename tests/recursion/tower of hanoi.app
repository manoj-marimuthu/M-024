# Tower of Hanoi using Recursion

fn hanoi(n,src,aux,dest):
    if n == 1:
        print("Move Disk 1 from",src,"to",dest)
        return None
    hanoi(n-1,src,dest,aux)
    print("Move Disk",n,"from",src,"to",dest)
    hanoi(n-1,aux,src,dest)

hanoi(3,'A','B','C')