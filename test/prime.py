n = int(input("Enter Number:"))
isPrime = True

if n <= 1:
    isPrime = False
else:
    i = 2
    while i < n:
        if n % i == 0:
            isPrime = False
        i = i + 1

if isPrime:
    print(n,"is prime")
else:
    print(n,"is not prime")
