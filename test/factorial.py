n = int(input("Enter Number:"))
if n < 0:
    print("Factorial Does Not Exist")
elif n == 1 or n == 0:
    print("Factorial of the Number",n,"is:",1)
else:
    fact = 1
    i = 1
    while i <= n:
        fact = fact*i
        i = i + 1
    print("Factorial of the Number",n,"is:",fact)