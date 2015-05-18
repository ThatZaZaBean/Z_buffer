for (int i = sizes[0].y; i >= sizes[1].y; i--)
{
	for (float j = top_l_x; j < top_r_x; j++)
	{
		z_pt = (-1 * (A * j) - (B * i) - D) / C;
		/*cout << "1st z_pt " << z_pt << endl;
		cout << "closest " << closest << endl;*/
		if (z_pt <= closest)
		{
			renderPixel((int)j, i, red, green, blue);
			closest = z_pt;
		}
	}
	top_l_x -= tp_m0;
	top_r_x -= r;
}
// Middle left to bottom slope
if ((sizes[1].y - sizes[2].y) == 0)
md_m0 = 0;
else
md_m0 = (float)(sizes[1].x - sizes[2].x) / (float)(sizes[1].y - sizes[2].y);

if (!isRight)
{
	float middle_x = sizes[1].x;
	// accouts for changes between first and second half
	swap(r, tp_m0);
	swap(r, md_m0);

	for (int i = sizes[1].y; i >= sizes[2].y; i--)
	{
		for (float j = top_l_x; j <= middle_x; j++)
		{
			z_pt = (-1 * (A * j) - (B * i) - D) / C;
			if (z_pt <= closest)
			{
				renderPixel((int)j, i, red, green, blue);
				closest = z_pt;
			}
		}
		top_l_x -= md_m0;
		middle_x -= r;
	}
}
else for (int i = sizes[1].y; i >= sizes[2].y; i--)
{
	for (float j = x_inc_s; j < top_r_x; j++)
	{
		z_pt = (-1 * (A * j) - (B * i) - D) / C;
		if (z_pt <= closest)
		{
			renderPixel((int)j, i, red, green, blue);
			closest = z_pt;
		}
	}
	x_inc_s -= md_m0;
	top_r_x -= r;
}