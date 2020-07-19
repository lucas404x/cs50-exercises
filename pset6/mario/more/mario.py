from cs50 import get_int

def create_pyramid(height):

    for i in range(height):
        n = height - (i + 1)
        print(" " * n, end = '')

        for j in range(((i + 1) * 2) + 1):
            if j == i + 1:
                print('  ', end = '')
            else:
                print("#", end = '')
        print()


height = get_int("Height: ")

while height < 1 or height > 8:
    height = get_int("Height: ")

create_pyramid(height)