/*
 * totp.c - implementation of the OATH TOTP algorithm
 *
 *
 * https://github.com/seryl/oath-toolkit
 * Copyright (C) 2011 Simon Josefsson
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
*/

#include "oath.h"

/**
 * oath_totp_generate:
 * @secret: the shared secret string
 * @secret_length: length of @secret
 * @now: Unix time value to compute TOTP for
 * @time_step_size: time step system parameter (typically 30)
 * @start_offset: Unix time of when to start counting time steps (typically 0)
 * @digits: number of requested digits in the OTP, excluding checksum
 * @output_otp: output buffer, must have room for the output OTP plus zero
 *
 * Generate a one-time-password using the time-variant TOTP algorithm
 * described in RFC 6238.  The input parameters are taken as time
 * values.
 *
*/


int
oath_totp_generate (const char *secret,
		    size_t secret_length,
		    time_t now,
		    unsigned time_step_size,
		    time_t start_offset, unsigned digits, char *output_otp)
{
  uint64_t nts;

  if (time_step_size == 0)
    time_step_size = OATH_TOTP_DEFAULT_TIME_STEP_SIZE;

  nts = (now - start_offset) / time_step_size;

  return oath_hotp_generate (secret,
			     secret_length,
			     nts,
			     digits,
			     false, OATH_HOTP_DYNAMIC_TRUNCATION, output_otp);
}
