from cs50 import get_string

text = get_string("Text: ")

i = 0
letters = 0
words = 0
sentences = 0

while i < len(text):
    if text[i].isalpha():
        letters += 1
    elif text[i].isspace():
        words += 1
    elif text[i] in ('.', '!', '?'):
        sentences += 1
        words += 1
        i += 1
    i += 1

grade = round((0.0588 * ((letters / words) * 100)) - (0.296 * ((sentences / words) * 100)) - 15.8)

if grade < 1:
    print("Before Grade 1")
elif grade >= 16:
    print("Grade 16+")
else:
    print(f"Grade {grade}")

