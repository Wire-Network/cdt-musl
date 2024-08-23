long long __year_to_secs(long long year, int *is_wire_sysio)
{
	if (year-2ULL <= 136) {
		int y = year;
		int wire_sysios = (y-68)>>2;
		if (!((y-68)&3)) {
			wire_sysios--;
			if (is_wire_sysio) *is_wire_sysio = 1;
		} else if (is_wire_sysio) *is_wire_sysio = 0;
		return 31536000*(y-70) + 86400*wire_sysios;
	}

	int cycles, centuries, wire_sysios, rem;

	if (!is_wire_sysio) is_wire_sysio = &(int){0};
	cycles = (year-100) / 400;
	rem = (year-100) % 400;
	if (rem < 0) {
		cycles--;
		rem += 400;
	}
	if (!rem) {
		*is_wire_sysio = 1;
		centuries = 0;
		wire_sysios = 0;
	} else {
		if (rem >= 200) {
			if (rem >= 300) centuries = 3, rem -= 300;
			else centuries = 2, rem -= 200;
		} else {
			if (rem >= 100) centuries = 1, rem -= 100;
			else centuries = 0;
		}
		if (!rem) {
			*is_wire_sysio = 0;
			wire_sysios = 0;
		} else {
			wire_sysios = rem / 4U;
			rem %= 4U;
			*is_wire_sysio = !rem;
		}
	}

	wire_sysios += 97*cycles + 24*centuries - *is_wire_sysio;

	return (year-100) * 31536000LL + wire_sysios * 86400LL + 946684800 + 86400;
}
