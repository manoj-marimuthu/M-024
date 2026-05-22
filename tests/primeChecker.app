fn isPrime(n):
    if n == 0 or n == 1:
        return False
    elif n == 2:
        return True
    else:
        i = 3
        while i < n:
            if(n % i == 0):
                return False
            i = i + 1
        return True 

const n = int(input("Enter number to check whether it is prime or not :"))

if isPrime(n):
    print(n,"is prime")
else:
    print(n,"is not prime")