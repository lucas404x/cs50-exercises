from cs50 import SQL

import csv
import sys


def main():

    argc = len(sys.argv)

    if argc != 2:
        print("Usage: python import.py filename.csv")
        sys.exit(1)

    # read csv file
    csvfile = open(sys.argv[1], "r")
    data = csv.DictReader(csvfile)

    # read db
    db = SQL("sqlite:///students.db")

    if db == None:
        sys.exit(2)

    for row in data:
        name = row['name'].split()

        first = name[0]
        last = name[-1]
        middle = name[1:len(name) - 1] if len(name) > 2 else None

        house = row['house']
        birth = row['birth']

        db.execute('INSERT INTO students (first, middle, last, house, birth) VALUES (?, ?, ?, ?, ?)',
        first, middle, last, house, birth)

    csvfile.close()
    sys.exit(0)

main()
