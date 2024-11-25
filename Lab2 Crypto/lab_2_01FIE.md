# Task 1: Transfer files between computers

**Question 1**:
Conduct transfering a single plaintext file between 2 computers,
Using openssl to implementing measures manually to ensure file integerity and authenticity at sending side,
then veryfing at receiving side.

**Answer 1**:

## 1. Create a text file named `file.txt` in container sender:

_First, we write a message and save it in a text file:_<br>

```sh
echo "I'm fullstack developer" > file.txt
```

## 2.Create HMAC for file.txt :

```sh
openssl dgst -sha256 -mac HMAC -macopt key:secretkey -out /file.hmac /file.txt
```

<img width="500" alt="Screenshot" src="https://github.com/user-attachments/assets/b817ffcc-6502-495f-9872-eb0f2ad87b86"><br>

## 3. Use bridge to tranfer file from container sender to container receiver:

```sh
scp /file.txt user@172.17.0.3:/file.txt
scp /file.hmac user@172.17.0.3:/file.hmac
```

Before : <br>
<img width="500" alt="Screenshot" src="https://github.com/user-attachments/assets/9304d044-4c5b-42ba-b8b9-4bb698307f8e"><br>

After : <br>
<img width="500" alt="Screenshot" src="https://github.com/user-attachments/assets/b59268bc-2d64-44db-991c-355698bd02f8"><br>

# Task 2: Transfering encrypted file and decrypt it with hybrid encryption.

**Question 1**:
Conduct transfering a file (deliberately choosen by you) between 2 computers.
The file is symmetrically encrypted/decrypted by exchanging secret key which is encrypted using RSA.
All steps are made manually with openssl at the terminal of each computer.

**Answer 1**:

## 1. Create a text file named `plaintext.txt`:

_First, we write a message and save it in a text file:_<br>

```sh
echo "Hello World I'm a student of UTE university" > plaintext.txt
```

_Read plain.txt file_<br>
<img width="500" alt="Screenshot" src="https://github.com/user-attachments/assets/d16416fd-c10b-45c5-8a8a-feba59fd12b0"><br>

## 2. Encrypt the file using AES-256 in ECB mode:

```sh
openssl enc -aes-256-ecb -nosalt -in plain.txt -out ecb_encrypted.txt -K 00112233445566778899AABBCCDDEEFF00112233445566778899AABBCCDDEEFF
```

## 3. View the encrypted file using `xxd`:

```sh
xxd ecb_encrypted.txt
```

<img width="500" alt="Screenshot" src="https://github.com/user-attachments/assets/f8ccb68a-f9f1-4bae-ab1d-fc16a026cc71"><br>

## 4.Transfer the ecb_encrypted.txt file to the container receiver,using Netcat:

In receiver container :

```sh
nc -l -p 1234 > ecb_encrypted.txt
```

In sender container - sending encrypted file :

```sh
cat ecb_encrypted.txt | nc receiver 1234
```

# Task 3: Firewall configuration

**Question 1**:
From VMs of previous tasks, install iptables and configure one of the 2 VMs as a web and ssh server. Demonstrate your ability to block/unblock http, icmp, ssh requests from the other host.

**Answer 1**:
