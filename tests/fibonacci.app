fn fibonacci(n):
    a = 0
    b = 1
    count = 0
    print(a)
    while count < n:
        temp = a
        a = b
        b = temp + b
        count = count + 1
        print(a)
n = int(input("Enter number of terms :"))
fibonacci(n)