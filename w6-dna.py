import csv
from sys import argv, exit


def main():

    # Check for command-line usage
    if len(argv) != 3:
        print("Invalid input.")
        exit(1)

    # Read database file into a variable
    with open(argv[1], newline='') as csvfile:
        csv_db = csv.reader(csvfile)
        db = list(csv_db)

    # Read DNA sequence file into a variable
    dnafile = open(argv[2], 'r')
    sequence = dnafile.read()
    dnafile.close()

    # Find longest match of each STR in DNA sequence
    findings = []

    for STR in db[0][1:]:
        findings.append(str(longest_match(sequence, STR)))

    # Check database for matching profiles
    for record in db:
        if record[1:] == findings:
            print(record[0])
            break
    else:
        print("No match")

    return


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


main()

