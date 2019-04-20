with open('file.txt','r') as file:
    lines=file.readlines()
    num=[i+1 for i in range(len(lines))]
    new_lines=list(map(lambda line,num: str(num)+'. '+line , lines,num))
    file.close()
    f=open('file.txt','w')
    f.writelines(new_lines)
    print(file)
    f.close()
