all:
	gcc -o main main.c rsa.c -lgmp
	gcc -o generate_rsa generate_rsa.c rsa.c -lgmp

debug:
	gcc -o main main.c rsa.c -lgmp -g
	gcc -o generate_rsa generate_rsa.c rsa.c -lgmp -g

clean:
	rm main generate_rsa

