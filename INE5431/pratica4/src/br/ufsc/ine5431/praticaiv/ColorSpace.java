package br.ufsc.ine5431.praticaiv;

public class ColorSpace {

	public static double rgb_int_to_double(int value)
	{
		return (double)(value/255.0);
	}

	public static int rgb_double_to_int(double value)
	{
		return (int)(value*255);
	}
	
	public static double[] rgb_to_ycbcr_base_double(double r, double g, double b) {

		double Ey, Er, Eb;

		Ey = 0.299*r + 0.587*g + 0.144*b;
		Er = 0.701*r - 0.587*g - 0.114*b;
		Eb = -.299*r - 0.587*g + 0.886*b;

		double Cr, Cb;

		Cr = Er / 1.402;
		Cb = Eb / 1.772;

		double ycbcr[] = new double[3];

		ycbcr[0] = Ey;
		ycbcr[1] = Cb;
		ycbcr[2] = Cr;

		/**
			Modifiquem o código acima para fazer a conversão corretamente!
			PS: sugiro que não se basear na conversão inversa que está otimizada... 
			Sigam a especificação do documento BT.601
		**/
		
		return ycbcr;
	}

	public static int ycbcr_y_double_to_int(double y)
	{
		return Math.round((float)(219.0*y+16.0));
		// return ((int)(219.0*y+16.0)&0xff); // pg.4 of BT.601
	}

	public static int ycbcr_chroma_double_to_int(double chroma)
	{
		return Math.round((float)(224.0*chroma+128.0));
	//	return ((int)(224.0*chroma+128.0)&0xff);
	}

	public static double ycbcr_y_int_to_double(int y)
	{
		return (double)((y-16)/219.0);
	}

	public static double ycbcr_chroma_int_to_double(int chroma) {
		return (double)((chroma-128)/224.0);
	}

	public static int[] rgb_to_ycbcr_base(	int r, int g, int b)
	{
	    double r_double = rgb_int_to_double(r);
	    double g_double = rgb_int_to_double(g);
	    double b_double = rgb_int_to_double(b);

	   	double[] ycbcr_double = rgb_to_ycbcr_base_double(r_double, g_double, b_double);
	   	
	   	int[] ycbcr_int = new int[3];

	    ycbcr_int[0] = ycbcr_y_double_to_int(ycbcr_double[0]);
	    ycbcr_int[1] = ycbcr_chroma_double_to_int(ycbcr_double[1]);
	    ycbcr_int[2] = ycbcr_chroma_double_to_int(ycbcr_double[2]);
	    return ycbcr_int;
	}

	public static double check_and_correct_rgb_range_component( double component)
	{
		if (component > 1.0)
			return 1;
		if (component < 0.0)
			return 0;
		return component;
	}

	public static double[] ycbcr_to_rgb_check_and_correct_ranges(double rgb[])
	{
		double[] rgb_corrected= new double[3];
		rgb_corrected[0] = check_and_correct_rgb_range_component(rgb[0]);
		rgb_corrected[1] = check_and_correct_rgb_range_component(rgb[1]);
		rgb_corrected[2] = check_and_correct_rgb_range_component(rgb[2]);
		return rgb_corrected;
	}

	public static double[] ycbcr_to_rgb_base_double(double kb, double kr, double y, double cb, double cr) { 
		double[] rgb = new double[3];
	    rgb[2] = cb*2*(1.0-kb)+y; // b

	    rgb[0] = cr*2*(1.0-kr)+y; // r

	    rgb[1] = (y-kr*(rgb[0])-kb*(rgb[2]))/(1.0-kr-kb);  //g

	    return ycbcr_to_rgb_check_and_correct_ranges(rgb);
	}

	public static int[] ycbcr_to_rgb_base(	double kb, double kr, int y, int cb, int cr) 
	{
	    double y_double = ycbcr_y_int_to_double(y);
		double cb_double = ycbcr_chroma_int_to_double(cb);
		double cr_double = ycbcr_chroma_int_to_double(cr);

	    double rgb_double[] = ycbcr_to_rgb_base_double(kb, kr, y_double, cb_double, cr_double);

	    int rgb[] = new int[3];
	    rgb[0] = rgb_double_to_int(rgb_double[0]);
	    rgb[1] = rgb_double_to_int(rgb_double[1]);
	    rgb[2] = rgb_double_to_int(rgb_double[2]);
	    
	    return rgb;
	}


	//ITU-R BT.601 conversion
	public static int[] bt601_rgb_to_ycbcr(int r, int g, int b)
	{
		return rgb_to_ycbcr_base(r&0xff, g&0xff, b&0xff);
	}

	public static double[] bt601_rgb_to_ycbcr_double(double r, double g, double b)
	{
		assert((r >= 0.0) && (r <= 1.0));
		assert((g >= 0.0) && (g <= 1.0));
		assert((b >= 0.0) && (b <= 1.0));

		return rgb_to_ycbcr_base_double(r, g, b);
	}

	public static int[] bt601_ycbcr_to_rgb(int y, int cb, int cr) {
		return ycbcr_to_rgb_base(0.114, 0.299, y, cb, cr);
	}

	public double[] bt601_ycbcr_to_rgb_double(double y, double cb, double cr){
		return ycbcr_to_rgb_base_double(0.114, 0.299, y, cb, cr);
	}
}
