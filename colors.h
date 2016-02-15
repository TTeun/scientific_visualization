float absolute(float a){
	return (a > 0)? a : (-a);
}

void summer(float value,float* R,float* G,float* B)
{
	*B = 1;
	*G = 1-value;
	*R = 1-value;
	
	if ((0.3<value) && (0.35 > value)){
		*G -= 40*(absolute(value - 0.325) - 0.025); 
		*R -= 40*(absolute(value - 0.325) - 0.025); 
		*B -= 1 - 10*(absolute(value - 0.325) - 0.025);
	}

	if ((0.55<value) && (0.6 > value)){
		*G -= 20*(absolute(value - 0.575) - 0.025); 
	}
}

void psychedelic1(float value,float* R,float* G,float* B)
{
		
	*G = *R = *B = .95;
	if (value < 0.25)
	{
		*B = 4*value;
	} 
	else if (value < 0.5)
	{
		*G = 4*(value - 0.25);
		*B = 2 - 4*value;
	}
	else if (value < 0.75)
	{
		*R = 4*(value - 0.5);
		*G = 3 - 4*value;
	}
	else if (value <= 1)
	{
		*R = 4 - 4*value;
	}
}


void psychedelic2(float value,float* R,float* G,float* B)
{
		
	*G = *R = *B = .2;
	if (value < 0.25)
	{
		*B = 4*value;
	} 
	else if (value < 0.5)
	{
		*G = 4*(value - 0.25);
		*B = 2 - 4*value;
	}
	else if (value < 0.75)
	{
		*R = 4*(value - 0.5);
		*B = 3 - 4*value;
	}
	else if (value <= 1)
	{
		*R = 4 - 4*value;
		*G = 0;
		*B = 0;
	}
}

void rainbow_long(float value,float* R,float* G,float* B)
{
	*G = *R = *B = 0.0;
	if (value < 0.25)
	{
		*B = 0.75*4*value;
	} 
	else if (value < 0.5)
	{
		*G = 4*(value - 0.25);
		*B = 2 - 4*value;
	}
	else if (value < 0.75)
	{
		*R = 4*(value - 0.5);
		*G = 3 - 4*value;
	}
	else if (value <= 1)
	{
		*R = 4 - 4*(value - 0.75);
	}
}
