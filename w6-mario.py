from cs50 import get_int

height = get_int("Height: ")

while height < 1 or height > 8:
    print("Height should be between 1 and 8.")
    height = get_int("Height: ")

for i in range(1, height+1):
    print(" " * (height-i), "#" * i, "  ", "#" * i, sep="")

