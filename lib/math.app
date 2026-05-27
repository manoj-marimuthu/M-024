# The math library provides utility functions and constants
# load it into your project using 'lib/math.app' as the path

const pi = 3.141592653589793
const e = 2.718281828459045
const tau = 2 * pi

fn isOdd(n):
    if n % 2 != 0:
        return False
    else:
        return True

fn isEven(n):
    if n % 2 == 0:
        return True
    else:
        return False

fn floor(n):
    return int(n)

fn ceil(n):
    return int(n+1)

fn factorial(n):
    if n == 0 or n == 1:
        return 1
    return factorial(n-1) * n

fn dist(x1,y1,x2,y2):
    return ((x1 - x2)^2 + (y1 -  y2)^2) ^ 0.5

fn abs(n):
    if n < 0:
        n = (0-1) * n
    return n

fn gcd(a,b):
    while b != 0:
        temp = a
        a = b
        b = temp % b
    return a

fn lcm(a,b):
    return (a*b)/gcd(a,b)

fn copysign(a,b):
    if b < 0:
        if a > 0:
            a = (0-1)*a
    else:
        if a < 0:
            a = (0-1) * a
    return a

fn rad(deg):
    return (deg * pi/180)

fn deg(rad):
    return (rad * 180 / pi)