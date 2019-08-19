import os
import sys

shellcode = ""
hexDmp = os.popen('hexdump -C ' + sys.argv[1]).read()
lines = hexDmp.split('\n')
for line in lines :
    line = line[8:].split('  |')[0]
    if len(line) == 0 :
        continue
    shellcode += ' '.join(line.split()).replace(' ','\\x') + "\\x"
print("\\x"+shellcode[:-2])