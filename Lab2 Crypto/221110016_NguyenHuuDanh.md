# Task 1: Transfer files between computers

**Question 1**:
Conduct transfering a single plaintext file between 2 computers,
Using openssl to implementing measures manually to ensure file integerity and authenticity at sending side, then veryfing at receiving side.

**Answer 1**:

### 1. Create a text file named `file.txt` in container sender:

_First, we write a message and save it in a text file:_<br>

```sh
echo "I'm fullstack developer" > file.txt
```

### 2. Use bridge to tranfer file from container sender to container receiver:

```sh
scp /file.txt user@172.17.0.3:/file.txt
scp /file.hmac user@172.17.0.3:/file.hmac
```

Before : <br>
<img width="500" alt="Screenshot" src="https://github.com/user-attachments/assets/9304d044-4c5b-42ba-b8b9-4bb698307f8e"><br>

After : <br>
<img width="500" alt="Screenshot" src="https://github.com/user-attachments/assets/b59268bc-2d64-44db-991c-355698bd02f8"><br>
<br>

# Task 2: Transfering encrypted file and decrypt it with hybrid encryption.

**Question 1**:
Conduct transfering a file (deliberately choosen by you) between 2 computers.
The file is symmetrically encrypted/decrypted by exchanging secret key which is encrypted using RSA.
All steps are made manually with openssl at the terminal of each computer.

**Answer 1**:

### 1. Create a text file named `plain.txt`:

_First, we write a message and save it in a text file:_<br>

```sh
echo "Hello World I'm a student of UTE university" > plain.txt
```

_Read plain.txt file_<br>
<img width="500" alt="Screenshot" src="https://github.com/user-attachments/assets/d16416fd-c10b-45c5-8a8a-feba59fd12b0"><br>

### 2. Create Public and Private Key in container receiver :

Generate RSA private key<br>

```sh
openssl genpkey -algorithm RSA -out private_key.pem -aes256
```

Generate RSA public key

```sh
openssl rsa -pubout -in private_key.pem -out public_key.pem
```

<img width="500" alt="Screenshot" src="https://github.com/user-attachments/assets/2b64f0fa-f4f8-4a5d-9477-1b72f4b5c08a"><br>

### 3. Tranfer public key using Netcat `nc`:

In container sender <br>

```sh
nc -l -p 12345 > public_key.pem
```

In container receiver <br>

```sh
cat public_key.pem | nc sender 12345
```

<img width="500" alt="Screenshot" src="https://github.com/user-attachments/assets/44788358-e569-463c-a73a-27902c869197"><br>

### 4. Generate a random AES key for encryption (256-bit) and encrypt files txt :

Generate a random AES key <br>

```sh
openssl rand -out secret.key 32
```

<img width="500" alt="Screenshot" src="https://github.com/user-attachments/assets/21a99dfd-53d5-4b7c-8d4b-849d392dde01"><br>

Encrypt the plain.txt file with AES-256<br>

```sh
openssl enc -aes-256-cbc -salt -in plain.txt -out encrypted_file.bin -pass file:./secret.key
```

<img width="500" alt="Screenshot" src="https://github.com/user-attachments/assets/b3edfbe4-b997-4408-a658-36708fb03287"><br>

### 5. Encrypt the secret key with the public key (RSA)

```sh
openssl rsautl -encrypt -inkey public_key.pem -pubin -in secret.key -out encrypted_key.bin
```

<img width="500" alt="Screenshot" src="https://github.com/user-attachments/assets/d64c4f37-5e09-4a11-88a0-ab54dc3f7458"><br>
<br>

### 6. Send encrypted_file.bin and encrypted_key.bin from container sender to receiver:

<img width="500" alt="Screenshot" src="https://github.com/user-attachments/assets/cc7974d9-a9d9-4722-9af2-6c4c168e226d"><br>

### 7. The receiver uses the private key to decrypt the secret key and then uses the secret key to decrypt the plaintext.txt

Decrypt secret.key with private key<br>

```sh
openssl rsautl -decrypt -inkey private_key.pem -in encrypted_key.bin -out decrypted_key.txt
```

<img width="500" alt="Screenshot" src="https://github.com/user-attachments/assets/3fa5432c-f36a-464b-a8f6-a513d0560f15"><br>

Decrypt the encrypted_file.bin file with the secret key<br>

```sh
openssl enc -d -aes-256-cbc -in encrypted_file.bin -out decrypted_plaintext.txt -pass file:decrypted_key.txt
```

<img width="500" alt="Screenshot" src="https://github.com/user-attachments/assets/99e6b9c2-8c87-437e-bd10-52740e30cc6a"><br>

<br><br>

# Task 3: Firewall configuration

**Question 1**:
From VMs of previous tasks, install iptables and configure one of the 2 VMs as a web and ssh server. Demonstrate your ability to block/unblock http, icmp, ssh requests from the other host.

**Answer 1**:

### 1. Create 2 virtual machines in docker :

```sh
docker run -dit --name web_server --privileged ubuntu:latest
docker run -dit --name client --privileged ubuntu:latest
```

# 2. Configure firewall on web_server container

## 2.1 In container web_server :

### 2.1.1 Block/Unblock HTTP (port 80):

Block HTTP (port 80 )

```sh
iptables -A INPUT -p tcp --dport 80 -j DROP
```

Unblock HTTP (port 80)

```sh
iptables -D INPUT -p tcp --dport 80 -j DROP
```

### 2.1.2 Block/Unblock ICMP (ping):

Block ICMP:

```sh
iptables -A INPUT -p icmp -j DROP
```

Unblock ICMP:

```sh
iptables -D INPUT -p icmp -j DROP
```

### 2.1.3 Block/Unblock SSH (port 22):

Block SSH:

```sh
iptables -A INPUT -p tcp --dport 22 -j DROP
```

Unblock SSH:

```sh
iptables -D INPUT -p tcp --dport 22 -j DROP
```

<br><br>

## 3. Check from the client side

Ping to check ICMP connection:

```sh
ping -c 4 172.17.0.2
```

<img width="500" alt="Screenshot" src="https://github.com/user-attachments/assets/f5643aae-4db6-4d42-b7de-4c28201077e7"><br>

HTTP request using curl:

```sh
curl http://172.17.0.2
```

<img width="500" alt="Screenshot" src="https://github.com/user-attachments/assets/f2aaf6fd-17ec-438c-a982-79ed35180529"><br>
<img width="500" alt="Screenshot" src="https://github.com/user-attachments/assets/32237ffc-d45f-4ad3-bbe4-88d4e0c8bf20"><br>

SSH try connection:

```sh
ssh root@172.17.0.2
```

<img width="500" alt="Screenshot" src="https://github.com/user-attachments/assets/097b7ace-9f09-4fd8-b2a0-2ed4d4294136"><br>
