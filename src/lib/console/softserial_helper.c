static void PUTFUNC (char c)
{
	int i;
	ZERO; /* Start */
	for (i=0; i<8; i++)
	{
		if (c & (1<<i)) {
			ONE;
		} else {
			ZERO;
		}
	}
	ONE;
}
