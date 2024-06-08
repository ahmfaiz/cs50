from sys import exit

card = input("Number: ")
size = len(card)
double_sum = single_sum = cur_sum = 0

# Luhn’s Algorithm
for i in range(size-2, -1, -2):
    cur_sum = (int(card[i]) * 2)
    for num in str(cur_sum):
        double_sum += int(num)

for i in range(size-1, -1, -2):
    single_sum += int(card[i])

if ((double_sum + single_sum) % 10) != 0:
    print("INVALID")
    exit(1)

# If valid as per Luhn’s Algorithm, checking card company
if size == 15 and card[0:2] in ["34", "37"]:
    print("AMEX")

elif size == 16 and card[0:2] in ["51", "52", "53", "54", "55"]:
    print("MASTERCARD")

elif size in [13, 16] and card[0] == "4":
    print("VISA")

else:
    print("INVALID")

