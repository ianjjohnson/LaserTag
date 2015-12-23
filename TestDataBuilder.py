import random


f = open('TEAMONE.txt', 'w')

f.write("The Data\n")
f.write("5\n")

for i in range(5):
    f.write(str(i) + " " +  "Player Name " + str(i) + "\n")


f.close()


f = open('TEAMTWO.txt', 'w')

f.write("The Structures\n")
f.write("5\n")

for i in range(5):
    f.write(str(i+5) + " " +  "Player Name " + str(i+5) + "\n")


f.close()
    

f = open('MATCH.txt', 'w')

f.write("1000\n")

for i in range(1000):
    f.write(str(random.randint(0,4)) + " " + str(random.randint(5,9)) + " " + str(random.randint(0,100000)) + " " + str(random.randint(1,4)) + "\n")
    f.write(str(random.randint(5,9)) + " " + str(random.randint(0,4)) + " " + str(random.randint(0,100000)) + " " + str(random.randint(1,4)) + "\n")
