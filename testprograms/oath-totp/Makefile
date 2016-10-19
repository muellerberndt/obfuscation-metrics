ARCH=arm
NDK=/Users/berndt/Tools/android-ndk-r10e/
SYSROOT=/Users/berndt/Tools/android-ndk-r10e/platforms/android-21/arch-arm/
CC=${NDK}/toolchains/arm-linux-androideabi-4.8/prebuilt/darwin-x86_64/bin/arm-linux-androideabi-gcc-4.8 --sysroot=${SYSROOT}
CFLAGS=-s

testbin : sha1.o hmac-sha1.o hotp.o totp.o test.o
	$(CC) $(CFLAGS) sha1.o hmac-sha1.o hotp.o totp.o test.o -o testbin

test.o : test.c
	$(CC) $(CFLAGS) -c test.c -o test.o

totp.o : totp.c
	$(CC) $(CFLAGS) -c totp.c -o totp.o

hotp.o : hotp.c
	$(CC) $(CFLAGS) -c hotp.c -o hotp.o

hmac-sha1.o : hmac-sha1.c
	$(CC) $(CFLAGS) -c hmac-sha1.c -o hmac-sha1.o

sha1.o : sha1.c
	$(CC) $(CFLAGS) -c sha1.c -o sha1.o

clean:
	rm -f testbin *.o
