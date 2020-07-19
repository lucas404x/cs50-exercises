from cs50 import SQL

import sys

def main():

    argc = len(sys.argv)
    if argc != 2:
        print("Usage: python roster.py house's-name")
        sys.exit(1)

    db = SQL("sqlite:///students.db")
    rows = db.execute('SELECT first, middle, last, birth FROM students WHERE house = ? ORDER BY last, first', sys.argv[1])

    for row in rows:
        first = row['first'] + ' '
        middle = '' if row['middle'] == None else row['middle'] + ' '
        last = row['last']
        birth = row['birth']

        print('{}{}{}, born {}'
        .format(first, middle, last, birth))

    sys.exit(0)

main()
