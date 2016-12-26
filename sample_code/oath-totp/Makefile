ARCH=arm
NDK=/Users/berndt/Tools/android-ndk-r10e/
SYSROOT=/Users/berndt/Tools/android-ndk-r10e/platforms/android-21/arch-arm/
CC=${NDK}/toolchains/arm-linux-androideabi-4.8/prebuilt/darwin-x86_64/bin/arm-linux-androideabi-gcc-4.8 --sysroot=${SYSROOT}
CFLAGS=-s

mini-totp : mini-totp.o
	$(CC) $(CFLAGS) -o mini-totp mini-totp.o

mini-totp.o : mini-totp.c
	$(CC) $(CFLAGS) -c mini-totp.c

clean:
	rm -f *.o mini-totp
