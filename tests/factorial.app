fn factorial(n):
    if n == 0 or n == 1:
        return 1
    else:
        i = 0
        ans = 1
        while i < n:
            ans = ans * n
            n = n -1
        return ans

n = int(input("Enter number to calculate factorial:"))
print("Factorial of",n,"is :",factorial(n))
