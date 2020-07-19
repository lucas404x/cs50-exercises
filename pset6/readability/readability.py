from cs50 import get_string
from sys import exit

import re

def main():
    text = get_string("Text: ")
    grade = round(coleman_liau_index(L(text), S(text)))

    if grade < 1:
        print("Before Grade 1")
    elif grade < 16:
        print(f"Grade {grade}")
    else:
        print("Grade 16+")

    exit(0)

def coleman_liau_index(L, S):
    return 0.0588 * L - 0.296 * S - 15.8

def L(text):
    words = len([
        word for word in re.split("[^A-z-']", text) if word != ''
        ])

    letters = sum([
        len(letter) for letter in re.split("[^A-z]", text)
        ])

    return (letters / words) * 100

def S(text):
    words = len([
        word for word in re.split("[^A-z-']", text) if word != ''
        ])

    sentences = len(re.findall("([\.\!]$|[\.\?\!]\s)", text))

    return (sentences / words) * 100

main()