#include <stdio.h>
#include "oath.h"

static char secret[] = "1234567890123456";

int main(int argc, char *argv[]) {

	char output_otp[7];

	oath_totp_generate(secret, 16, time(NULL), 30, 0, 6, output_otp);
	output_otp[6] = 0;

	// printf("OTP: %s\n", output_otp);

	return 0;
}
