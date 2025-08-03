#   What is this file?
- Below are a collection of python programs. All programs below will execute
on the python interpreter, meaning that pyco must emit semantically equivilant
code for all inputs below.
    - Essentially, we use these as test programs for our compiler.
    - We also assign each input a Roman numeral to identify said input. This
    will allow us to write about certain inputs/mark them as problem inputs in
    my TODO list.

##  LOOPS
---

### INPUT I
for i in range(5):
    print(i)
for j in range(10):
    print(j)

---

### INPUT II
for i in range(5):
    for j in range(10):
        print(j)
    print(i)

---

### INPUT III
for i in range(5):
    print(i)
    for j in range(10):
        print(j)

---

### INPUT IV
for i in range(5):
    for j in range(10):
        for x in range(15):
            print(x)
        print(j)
    print(i)

---

### INPUT V
for a in range(1):
    for b in range(2):
        for c in range(3):
            for d in range(4):
                for e in range(5):
                    for f in range(6):
                        for g in range(7):
                            for h in range(8):
                                print(h)
                            print(g)
                        print(f)
                    print(e)
                print(d)
            print(c)
        print(b)
    print(a)

---

### INPUT VI
y = 0
for i in range(5):
    for j in range(10):
        print(j)
        x = 16 + 9
        print(x)
    print(i)
    y = y + i
print(y)

---

### INPUT VII
for i in range(5):
    for j in range(10):
        print(j)
    print(i)
for x in range(3):
    print(x)
for y in range(6):
    print(y)

---

### INPUT VIII
for x in range(3):
    print(x)
for y in range(6):
    print(y)
for i in range(5):
    for j in range(10):
        print(j)
    print(i)

