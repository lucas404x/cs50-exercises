import sys
import csv


def main():

    argc = len(sys.argv)

    if argc != 3:
        print("Usage: python dna.py database.csv sequence.txt")
        sys.exit(1)

    # read database
    db = open(sys.argv[1], "r")
    data = csv.reader(db)

    for row in data:
        sequences = row[1:]
        break

    # read sequence
    with open(sys.argv[2], "r") as dna_sequence:
        dna = dna_sequence.read().rstrip()
        strs = [find_str(sequence, dna) for sequence in sequences]

    print(find_person_sequence(data, strs))

    db.close()
    sys.exit(0)

def find_str(sequence, dna):
    all_sequences = []
    wait = count = 0

    for i in range(len(dna)):
        if i + len(sequence) < len(dna):
            if sequence == dna[i:i + len(sequence)] and wait == 0:
                wait = len(sequence) - 1
                count += 1
            elif wait > 0:
                wait -= 1
            else:
                all_sequences.append(count)
                count = 0

    return max(all_sequences)

def find_person_sequence(data, sequences):
    name = "No match"

    for row in data:
        person_data = row[1:]

        check = [
            d for d in range(len(person_data))
            if int(person_data[d]) == sequences[d]
        ]

        if len(check) == len(person_data):
            name = row[0]
            break

    return name

main()