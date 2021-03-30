package br.ufsc.ine5431.praticaiv;

import java.io.*;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.nio.file.Path;

// Classe representando o conteúdo de um arquivo bitmap
public class Bitmap {
	// Campos do cabeçalho bitmap:
	private int size_of_bitmap_file; 			
	private int reserved;					
	private int offset_to_start_image;			
	private int size_of_header; //must be 40	
	private int width;
	private int height;
	private int number_of_planes;
	private int number_of_bits_per_pixel;
	private int compression_type;
	private int size_of_image_in_bytes;
	private int horizontal_pixels_per_meter;
	private int vertical_pixels_per_meter;
	private int number_of_colors;
	private int number_of_important_colors;
	// Reticulado da imagem:
	public int[][][] raster; 
        // RGB, onde raster[i][j] é a posição do pixel (i,j) 
    	// raster[i][j][0] é o componente R em (i,j)
    	// raster[i][j][1] é o componente G em (i,j)
		// raster[i][j][2] é o componente B em (i,j)
    
    // Array de bytes com dados do arquivo BMP
    private byte[] bitmapfile; 

    // Construtor de uma imagem vazia.     
    Bitmap() {
    	width = height = 0;
    }

    // Construtor para gerar objeto Bitmap a partir do arquivo bitmap de nome "filename"
    Bitmap(String filename) throws Exception{
    	
    	// Abre o arquivo BMP e lê os bytes do arquivo
    	byte[] bitmapfile = getBytes(filename);   	
    	
    	// Leitura e  verificação do cabeçalho
    	readBMPHeader(bitmapfile);     
        
    	// Criação do raster (reticulado)
    	raster=new int[height][width][3];
    	byte r, g, b;
    	int index = offset_to_start_image;
    	for (int i=0; i<height; i++){
    		for (int j=0; j<width; j++){
    	    	b = (byte)(bitmapfile[index++]&0xff);  // BMP salva no formato BGR em vez de RGB
    	        g = (byte)(bitmapfile[index++]&0xff);
    	        r = (byte)(bitmapfile[index++]&0xff);
    	        raster[i][j][0] = r; 
    	        raster[i][j][1] = g;
    	        raster[i][j][2] = b;  
    	     }
    	}
    }    
    
  
    // Converte um arquivo Cuif chamado filename em BMP.
    public void cuiftoBitmap(String filename) throws Exception {
    	
    	// abre arquivo cuif e lê bytes
    	byte[] cuiffile = getBytes(filename);
    	
    	// Leitura do cabeçalho CUIF
    	int signature = (cuiffile[0]&0xff) | (cuiffile[1]&0xff)<<8; 
    	if (signature!=5431)
           throw new IOException("File format error");
    	int version = cuiffile[2];
   	 	int number_of_students = cuiffile[3]&0xff;
   	 	width = (cuiffile[4]&0xff)|(cuiffile[5]&0xff)<<8|(cuiffile[6]&0xff)<<16| 
			   (cuiffile[7]&0xff)<<24;
   	 	height = (cuiffile[8]&0xff)|(cuiffile[9]&0xff)<<8|(cuiffile[10]&0xff)<<16| 
  			   (cuiffile[11]&0xff)<<24;
   	 	
   	 	System.out.print("Arquivo gerado pelo(s) estudante(s):");
   	 	int j=12;
   	 	for (int i=0;i<number_of_students;i++) {
   	 		int matricula = (cuiffile[j++]&0xff)|(cuiffile[j++]&0xff)<<8|(cuiffile[j++]&0xff)<<16| 
   	  			   (cuiffile[j++]&0xff)<<24;
   	 		System.out.print(matricula + ", ");
   	 	}

   	  	/* conversão do header Cuif em BMP */
   	 	convertCuif2BMPHeader();
   	 	
   	 	// gera raster, dependendo da versão do arquivo CUIF
   	 	if (version==1)
   	 		cuif1toRaster(cuiffile, number_of_students*4 + 12);  // Descompactado RGB
   	 	else if (version==2)
   	 		cuif2toRaster(cuiffile, number_of_students*4 + 12);  // Descompactado YCbCr
   	 	else if (version==3)
   	 		cuif3toRaster(cuiffile, number_of_students*4 + 12);  // Compactado YCbCr com Codificação de Huffman
   	 	else if (version==4)
   	 		cuif4toRaster(cuiffile, number_of_students*4 + 12); // Compactado RGB com Codificação RLE
   	 	else
   	 		throw new IOException("Versão não suportada");
    }
    
   	// Método que lê os pixels do arquivo Cuif1 e gera raster, incluindo no array de bytes do arquivo    
    private void cuif1toRaster(byte[] cuiffile, int index) {
    	// Leitura do Raster do arquivo CUIF1
    	readRaster(cuiffile, index);
   	}
    
   	// Método que lê os pixels do arquivo Cuif2 e gera raster, incluindo no array de bytes do arquivo    
    private void cuif2toRaster(byte[] cuiffile, int index) {
    	
    	// Primeiro é necessário gerar o raster YCbCr a partir do YCbCr do arquivo CUIF2
   	 	int[][][] ycbcr = new int[height][width][3];
   	 	raster=new int[height][width][3];
   		for (int i=0; i<height; i++)
   	 		for (int j=0; j<width; j++)
   	 			ycbcr[i][j][0] = (cuiffile[index++]&0xff);    
   		for (int i=0; i<height; i++)
   	 		for (int j=0; j<width; j++)
   	 			ycbcr[i][j][1] = (cuiffile[index++]&0xff);    
   		for (int i=0; i<height; i++)
   	 		for (int j=0; j<width; j++)
   	 			ycbcr[i][j][2] = (cuiffile[index++]&0xff);      
   		
   		// Geração do Raster RGB e escrita do raster no array representando arquivo BMP
   	 	int k=offset_to_start_image;
   	 	for (int i=0; i< height; i++){
	 		for (int j=0; j< width; j++){
   	 			// conversão YCbCr to RGB
   	 			raster[i][j] = ColorSpace.bt601_ycbcr_to_rgb(ycbcr[i][j][0], ycbcr[i][j][1], ycbcr[i][j][2]);
	 			bitmapfile[k++] = (byte)raster[i][j][2];
	 		 	bitmapfile[k++] = (byte)raster[i][j][1]; 
	 		 	bitmapfile[k++] = (byte)raster[i][j][0];
   	 		}
   	 	}
   	}
    
   	// Método que lê os pixels do arquivo Cuif3 e gera raster, incluindo no array de bytes do arquivo 
    private void cuif3toRaster(byte[] cuiffile, int index) throws IOException {

    	// Geração de um array com bytes codificados com Huffman
    	byte [] compresseddata = new byte[cuiffile.length-index];
    	int j=0;
    	for (int i=index;i<cuiffile.length;i++)
    		compresseddata[j++]=cuiffile[i];

    	// Descompacta os dados codificados usando Huffman
    	byte[] data =  br.ufsc.ine5431.huffmancoding.HuffmanCoding.decompression(compresseddata);
    	// Resto da operação é a mesma do CUIF2, pois o dado descompactado é o array YCbCr
   	 	cuif2toRaster(data, 0);
   	}
    
    
	// Método que lê os pixels do arquivo Cuif4 e gera raster, incluindo no array de bytes do arquivo
    private void cuif4toRaster(byte[] cuiffile, int index) {
    	
    	// Prepara array de bytes com os dados compactados com RLE
    	byte[] compressedRLE = new byte[cuiffile.length-index];
    	int j=0;
    	for (int i=index;i<cuiffile.length;i++)
    		compressedRLE[j++] = cuiffile[i];
    	
    	// Descompacta RLE
    	byte [] data = br.ufsc.ine5431.rlecoding.RLECoding.decompress(compressedRLE);
    	
    	// Dado descompactado em "data" é o array RGB, basta gerar raster e coloca no array de bytes represenando arquivo
    	readRaster(data, 0);
   	}

   
    
    // Leitura do Raster a partir de um array de bytes, inciando em "index"
    
    private void readRaster(byte[] data, int index) {
   	 	int r, g, b;
   	 	raster=new int[height][width][3];
   	 	   	
   	 	for (int i=0; i<height; i++){
   	 		for (int j=0; j<width; j++){
   	 			r = data[index++]&0xff;            	
   	 			raster[i][j][0] = r;    
   	 		}
   	 	}
   	 	for (int i=0; i<height; i++){
   	 		for (int j=0; j<width; j++){
   	 			g = data[index++]&0xff;            	
   	 			raster[i][j][1] = g;   
   	 		}
   	 	}
   	 	for (int i=0; i<height; i++){
   	 		for (int j=0; j<width; j++){
   	 			b = data[index++]&0xff;            	
   	 			raster[i][j][2] = b; 
   	 		}
   	 	} 
   	 	
   	 // Escrita do Raster no array de bytes representando arquivo BMP
   	  int k=offset_to_start_image;
   	  for (int i=0; i < height; i++){
 	 		for (int j=0; j<width; j++){
   		 	    bitmapfile[k++] = (byte)raster[i][j][2];  // Escreve B
   		 		bitmapfile[k++] = (byte)raster[i][j][1];  // Escreve G
   		 		bitmapfile[k++] = (byte)raster[i][j][0];  // Escreve R
   		 	}
   	    }
    }
    
    // Leitura dos campos do cabeçalho BMP
    private void readBMPHeader(byte[] bitmapfile) throws Exception{
        int signature = (bitmapfile[0]&0xff) | (bitmapfile[1]&0xff)<<8; 
        if (signature!=0x4D42)
        	throw new IOException("File format error :" + Integer.toHexString(signature));
        size_of_bitmap_file = (bitmapfile[2]&0xff)|(bitmapfile[3]&0xff)<<8|
        			             (bitmapfile[4]&0xff)<<16| (bitmapfile[5]&0xff)<<24;
        reserved = (bitmapfile[6]&0xff)|(bitmapfile[7]&0xff)<<8|(bitmapfile[8]&0xff)<<16| 
        		   (bitmapfile[9]&0xff)<<24;
        if (reserved!=0)
        	 throw new IOException("BMP Format Unsuported: Reserved");
        offset_to_start_image = (bitmapfile[10]&0xff)|(bitmapfile[11]&0xff)<<8|
        		                (bitmapfile[12]&0xff)<<16| (bitmapfile[13]&0xff)<<24;
        size_of_header = (bitmapfile[14]&0xff)|(bitmapfile[15]&0xff)<<8|
        			         (bitmapfile[16]&0xff)<<16| (bitmapfile[17]&0xff)<<24;
        width = (bitmapfile[18]&0xff)|(bitmapfile[19]&0xff)<<8|(bitmapfile[20]&0xff)<<16| 
        			(bitmapfile[21]&0xff)<<24;
        height = (bitmapfile[22]&0xff)|(bitmapfile[23]&0xff)<<8|(bitmapfile[24]&0xff)<<16| 
        		(bitmapfile[25]&0xff)<<24;	
        number_of_planes = (bitmapfile[26]&0xff)|(bitmapfile[27]&0xff)<<8;
        number_of_bits_per_pixel = (bitmapfile[28]&0xff)|(bitmapfile[29]&0xff)<<8;
        compression_type = (bitmapfile[30]&0xff)|(bitmapfile[31]&0xff)<<8|(bitmapfile[32]&0xff)<<16|
                           (bitmapfile[33]&0xff)<<24;
        if (compression_type!=0)
       	 	throw new IOException("BMP Format Unsuported: Compression type");
            	
        size_of_image_in_bytes = (bitmapfile[34]&0xff)|(bitmapfile[35]&0xff)<<8|(bitmapfile[36]&0xff)<<16| 
          		                 (bitmapfile[37]&0xff)<<24;    	 
        horizontal_pixels_per_meter = (bitmapfile[38]&0xff)|(bitmapfile[39]&0xff)<<8|(bitmapfile[40]&0xff)<<16| 
    	                      (bitmapfile[41]&0xff)<<24;
        vertical_pixels_per_meter = (bitmapfile[42]&0xff)|(bitmapfile[43]&0xff)<<8|(bitmapfile[44]&0xff)<<16| 
                    (bitmapfile[45]&0xff)<<24;
        number_of_colors = (bitmapfile[46]&0xff)|(bitmapfile[47]&0xff)<<8|(bitmapfile[48]&0xff)<<16| 
                    (bitmapfile[49]&0xff)<<24;
        number_of_important_colors = (bitmapfile[50]&0xff)|(bitmapfile[51]&0xff)<<8|(bitmapfile[52]&0xff)<<16| 
                    (bitmapfile[53]&0xff)<<24;
    }
    
    private void convertCuif2BMPHeader() throws Exception {
   	 	size_of_bitmap_file = 54 + height*width*3; // Cabeçalho de 54 bytes
   	 	int i=0;
   	 	bitmapfile = new byte [size_of_bitmap_file];
   	 	
   	 	// Assinatura
   	 	bitmapfile[i++] = 0x42;
   	 	bitmapfile[i++] = 0x4d;
   	 	//tamanho do arquivo
   	 	bitmapfile[i++] = (byte)(size_of_bitmap_file&0x000000ff);
   	 	bitmapfile[i++] = (byte)((size_of_bitmap_file&0x0000ff00)>>8);
   	 	bitmapfile[i++] = (byte)((size_of_bitmap_file&0x00ff0000)>>16);
   	 	bitmapfile[i++] = (byte)((size_of_bitmap_file&0xff000000)>>24);

   	 	// reserved
   	 	bitmapfile[i++] = bitmapfile[i++] = bitmapfile[i++] = bitmapfile[i++]=0;
	 
   	 	// offset do inicio da imagem
   	 	offset_to_start_image = 54; // 54 bytes usado no cabeçalho
   	 	bitmapfile[i++] = (byte)offset_to_start_image; 
   	 	bitmapfile[i++] = bitmapfile[i++] = bitmapfile[i++] = 0;
	 
   	 	// size_of_header
   	 	size_of_header = 40;
   	 	bitmapfile[i++] = (byte) size_of_header; 
   	 	bitmapfile[i++] = bitmapfile[i++] = bitmapfile[i++] = 0;
	 
   	 	// Width
   	 	bitmapfile[i++] = (byte)(width&0x000000ff);
   	 	bitmapfile[i++] = (byte)((width&0x0000ff00)>>8);
   	 	bitmapfile[i++] = (byte)((width&0x00ff0000)>>16);
   	 	bitmapfile[i++] = (byte)((width&0xff000000)>>24);
	 
   	 	// Height
   	 	bitmapfile[i++] = (byte)(height&0x000000ff);
   	 	bitmapfile[i++] = (byte)((height&0x0000ff00)>>8);
   	 	bitmapfile[i++] = (byte)((height&0x00ff0000)>>16);
   	 	bitmapfile[i++] = (byte)((height&0xff000000)>>24);
	 
   	 	// number of planes
   	 	number_of_planes = 1;
   	 	bitmapfile[i++] = (byte)number_of_planes;
   	 	bitmapfile[i++] = 0;
	 
   	 	// number_of_bits_per_pixel
   	 	number_of_bits_per_pixel = 24;
   	 	bitmapfile[i++] = (byte)number_of_bits_per_pixel; 
   	 	bitmapfile[i++] = bitmapfile[i++] = bitmapfile[i++] = 0;
	 
   	 	// Compression Type
   	 	compression_type = 0;
   	 	bitmapfile[i++] = (byte)compression_type; 
   	 	bitmapfile[i++] = bitmapfile[i++] = bitmapfile[i++] = 0;

   	 	// Size of image
   	 	size_of_image_in_bytes = width*height*3;
   	 	bitmapfile[i++] = (byte)(size_of_image_in_bytes&0x000000ff);
   	 	bitmapfile[i++] = (byte)((size_of_image_in_bytes&0x0000ff00)>>8);
   	 	bitmapfile[i++] = (byte)((size_of_image_in_bytes&0x00ff0000)>>16);
   	 	bitmapfile[i++] = (byte)((size_of_image_in_bytes&0xff000000)>>24);
	 
   	 	// horizontal pixels per meter
   	 	bitmapfile[i++] = (byte)(size_of_image_in_bytes&0x000000ff);
   	 	bitmapfile[i++] = (byte)((size_of_image_in_bytes&0x0000ff00)>>8);
   	 	bitmapfile[i++] = (byte)((size_of_image_in_bytes&0x00ff0000)>>16);
   	 	bitmapfile[i++] = (byte)((size_of_image_in_bytes&0xff000000)>>24);

   	 	// vertical pixels per meter
   	 	bitmapfile[i++] = (byte)(size_of_image_in_bytes&0x000000ff);
   	 	bitmapfile[i++] = (byte)((size_of_image_in_bytes&0x0000ff00)>>8);
   	 	bitmapfile[i++] = (byte)((size_of_image_in_bytes&0x00ff0000)>>16);
   	 	bitmapfile[i++] = (byte)((size_of_image_in_bytes&0xff000000)>>24);
	 
   	 	// number of colors
   	 	number_of_colors = 0;
   	 	bitmapfile[i++] = (byte)number_of_colors; 
   	 	bitmapfile[i++] = bitmapfile[i++] = bitmapfile[i++] = 0;

   	 	// number_of_important_colors
   	 	number_of_important_colors = 0;
   	 	bitmapfile[i++] = (byte)number_of_important_colors; 
   	 	bitmapfile[i++] = bitmapfile[i++] = bitmapfile[i++] = 0;
    }
    
    // Método para salvar o arquivo em "filename"
   public void save(String filename){
    	 try { 
    		    FileOutputStream fileOuputStream = new FileOutputStream(filename); 
    		    fileOuputStream.write(bitmapfile);
   			 	fileOuputStream.close();
    	 } catch (IOException ioex) {
         }
    }
   
	// Abre o arquivo e retorna os bytes do arquivo
   private byte[] getBytes(String filename) throws Exception{
   	Path path = Paths.get(filename);
   	byte[] bitmapfile = Files.readAllBytes(path);
   	return bitmapfile;
   }
   
   public int getHeigth() {
   	return height;
   }
   
   public int getWidth() {
   	return width;
   }
   
   public void printHeader() {
   	System.out.println("size_of_bitmap_file: " + size_of_bitmap_file); 			
   	System.out.println("reserved: " + reserved);					
   	System.out.println("offset_to_start_image: " + offset_to_start_image);			
   	System.out.println("size_of_header: " + size_of_header);	
   	System.out.println("width: " + width);
   	System.out.println("height: " + height);
   	System.out.println("number_of_planes: " + number_of_planes);
   	System.out.println("number_of_bits_per_pixel: " + number_of_bits_per_pixel);
   	System.out.println("compression_type: " + Integer.toHexString(compression_type));
   	System.out.println("size_of_image_in_bytes: " + size_of_image_in_bytes);
   	System.out.println("horizontal_pixels_per_meter: " + horizontal_pixels_per_meter);
   	System.out.println("vertical_pixels_per_meter: " + vertical_pixels_per_meter);
   	System.out.println("number_of_colors: " + number_of_colors);
   	System.out.println("number_of_important_colors: " + number_of_important_colors);
   }
 }
