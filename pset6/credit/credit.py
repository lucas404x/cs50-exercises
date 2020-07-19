from cs50 import get_int
from sys import exit


def main():
    card_number = str(get_int("Number: "))
    result = checksum(card_number)

    if not result:
        print("INVALID")
        exit(0)

    first_digits = int("".join(card_number[0:2]))

    if len(card_number) == 15 and first_digits in [37, 37]:
        print("AMEX")

    elif len(card_number) == 16 and first_digits in [51, 52, 53, 54, 55]:
        print("MASTERCARD")

    elif len(card_number) in [13, 16] and str(first_digits)[0] == "4":
        print("VISA")

    exit(0)

def checksum(card_number):
    n = len(card_number) - 2
    first_sum = second_sum = 0

    for i in range(n, -1, -2):
        digits = str(int(card_number[i]) * 2)
        first_sum += sum([int(digit) for digit in digits])
        second_sum += int(card_number[i + 1])

        if i - 1 == 0 and n % 2 != 0:
            second_sum += int(card_number[0])

    return True if (first_sum + second_sum) % 10 == 0 else False

main()