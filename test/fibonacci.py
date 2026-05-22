a = 0
b = 1
count = 0
n = int(input("Enter no of terms to generate fibonacci sequence:"))

while count < n:
    print(a)
    next = a + b
    a = b
    b = next
    count = count + 1