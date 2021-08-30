first = True

with open('home.htm') as f:
    for line in f.readlines():
        line = line.rstrip()
        if '"' in line:
            print("#### ERROR: line contains double quotes")
        if first:
            first = False
            print("const char* homePage = \"" + line + '  \\r\\n\\')
        else:
            print(line + '  \\r\\n\\')

print('";')
