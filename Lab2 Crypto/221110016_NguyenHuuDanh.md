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
<br>

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

## 2. Create Public and Private Key in container receiver :

Generate RSA private key<br>

```sh
openssl genpkey -algorithm RSA -out private_key.pem -aes256
```

Generate RSA public key

```sh
openssl rsa -pubout -in private_key.pem -out public_key.pem
```

<img width="500" alt="Screenshot" src="https://github.com/user-attachments/assets/2b64f0fa-f4f8-4a5d-9477-1b72f4b5c08a"><br>

## 3. Tranfer public key using Netcat `nc`:

In container sender <br>

```sh
nc -l -p 12345 > public_key.pem
```

In container receiver <br>

```sh
cat public_key.pem | nc sender 12345
```

<img width="500" alt="Screenshot" src="https://github.com/user-attachments/assets/44788358-e569-463c-a73a-27902c869197"><br>

## 4. Generate a random AES key for encryption (256-bit) and encrypt files txt :

Generate a random AES key <br>

```sh
openssl rand -out secret.key 32
```

<img width="500" alt="Screenshot" src="https://github.com/user-attachments/assets/21a99dfd-53d5-4b7c-8d4b-849d392dde01"><br>

Encrypt the plaintext.txt file with AES-256<br>

```sh
openssl enc -aes-256-cbc -salt -in plaintext.txt -out encrypted_file.bin -pass file:./secret.key
```

<img width="500" alt="Screenshot" src="https://github.com/user-attachments/assets/b3edfbe4-b997-4408-a658-36708fb03287"><br>

## 5. Encrypt the secret key with the public key (RSA)

```sh
openssl rsautl -encrypt -inkey public_key.pem -pubin -in secret.key -out encrypted_key.bin
```

<img width="500" alt="Screenshot" src="https://github.com/user-attachments/assets/d64c4f37-5e09-4a11-88a0-ab54dc3f7458"><br>
<br>

## 6. Send encrypted_file.bin and encrypted_key.bin from container sender to receiver:

<img width="500" alt="Screenshot" src="https://github.com/user-attachments/assets/cc7974d9-a9d9-4722-9af2-6c4c168e226d"><br>

## 7. The receiver uses the private key to decrypt the secret key and then uses the secret key to decrypt the plaintext.txt

Decrypt secret.key with private key<br>

```sh
openssl rsautl -decrypt -inkey private_key.pem -in encrypted_key.bin -out data/decrypted_key.txt
```

<img width="500" alt="Screenshot" src="https://github.com/user-attachments/assets/3fa5432c-f36a-464b-a8f6-a513d0560f15"><br>

Decrypt the encrypted_file.bin file with the secret key<br>

```sh
openssl enc -d -aes-256-cbc -in encrypted_file.bin -out decrypted_plaintext.txt -pass file:decrypted_key.txt
```

<img width="500" alt="Screenshot" src="https://github.com/user-attachments/assets/99e6b9c2-8c87-437e-bd10-52740e30cc6a"><br>

## 8. Compare file contents use `diff`

```sh
diff plaintext.txt decrypted_plaintext.txt
```

# Task 3: Firewall configuration

**Question 1**:
From VMs of previous tasks, install iptables and configure one of the 2 VMs as a web and ssh server. Demonstrate your ability to block/unblock http, icmp, ssh requests from the other host.

**Answer 1**:
