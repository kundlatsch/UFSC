package br.ufsc.ine5431.praticaiv;

import java.io.*;
import java.io.IOException;

public class Cuif {
	// Campos do cabeçalho CUIF:
	private int signature;
	private int version; 			
	private int number_of_students;					
	private int width;
	private int height;
	private int[] identifier;
	
	private byte[] header; // É o array de bytes com header do arquivo em memória
	private byte[] data; // É o array de bytes com dados do arquivo cuif em memória

	private int[][][] raster; 
    // Raster contém o bitmap RGB, onde raster[i][j] é a posição do pixel (i,j) 
	// raster[i][j][0] é o componente R em (i,j)
	// raster[i][j][1] é o componente G em (i,j)
	// raster[i][j][2] é o componente B em (i,j)
    private int[][][] ycbcr; // raster salvo em ycbcr;
    
    // Construtor Cuif que gera objeto a partir de uma imagem Bitmap
    Cuif(Bitmap bmp, int ver, int nstudents, int[] ids) throws Exception {
    	signature = 5431; // assinatura 5431d = 1537h
    	version = ver;
    	
    	// Geração do cabeçalho CUIF
    	number_of_students = nstudents;
    	identifier = ids;
    	width = bmp.getWidth();
    	height = bmp.getHeigth();
    	int offset=0;
    	header = new byte[12+number_of_students*4];  // new byte[width*height*3 + 12 + number_of_students*4];
    	header[offset++]= (byte)(signature&0x000000ff); // 0x37; 
    	header[offset++]=  (byte)((signature&0x0000ff00)>>8); // 0x15;
    	header[offset++]= (byte)version; // version
    	header[offset++]=(byte)number_of_students;
    	header[offset++] = (byte)(width&0x000000ff);
    	header[offset++] = (byte)((width&0x0000ff00)>>8);
    	header[offset++] = (byte)((width&0x00ff0000)>>16);
    	header[offset++] = (byte)((width&0xff000000)>>24);    
    	header[offset++] = (byte)(height&0x000000ff);
    	header[offset++] = (byte)((height&0x0000ff00)>>8);
    	header[offset++] = (byte)((height&0x00ff0000)>>16);
    	header[offset++] = (byte)((height&0xff000000)>>24);
    	for (int i=0;i<number_of_students;i++) {
    		header[offset++] = (byte)(identifier[i]&0x000000ff);
    		header[offset++] = (byte)((identifier[i]&0x0000ff00)>>8);
    		header[offset++] = (byte)((identifier[i]&0x00ff0000)>>16);
    		header[offset++] = (byte)((identifier[i]&0xff000000)>>24);
    	}    
    	
    	// Leitura do raster do arquivo
    	if (version==1)
    		generateCUIF1(bmp.raster);  // Descompactado em formato RGB
    	else if (version==2)
    		generateCUIF2(bmp.raster);  // Descompactado em formato YCbCr
    	else if (version==3)
    		generateCUIF3(bmp.raster);  // Compactado em formato YCBCr usando codificação de Huffman
    	else if (version==4)
    		generateCUIF4(bmp.raster); // Compactado em formato RGB usando codificação RLE
    	else
    		throw new IOException("Unsupported version");
    }
    
    // Método que gera CUIF1 simplesmente lendo raster RGB e convertendo em formato RGB do CUIF1
    private void generateCUIF1(int[][][] raster) {
    	readRaster(raster);
    }
    
 // Método que gera CUIF2 lendo o RGB do BitMap e convertendo em YCbCr
    private void generateCUIF2(int[][][] rasterbmp) {
    	
    	// Converte RGB do BMP para o raster YCbCr  
    	generateYCbCr(rasterbmp);
    	
    	// Salva raster YCbCr no array "data" que representa os dados do arquivo
    	int index=0;
    	data = new byte[height*width*3];
  		// Coloca Y no array "data" 
   	 	for (int i=0; i<height; i++)  
           for (int j=0; j<width; j++) 
           		data[index++] = (byte)(ycbcr[i][j][0]&0xff);           		
  		// Coloca Cb no array "data" 
  		for (int i=0; i<height; i++)
            for (int j=0; j<width; j++)
           		data[index++] = (byte)(ycbcr[i][j][1]&0xff);
  		// Coloca Cr no array "data" 
   	 	for (int i=0; i<height; i++)
            for (int j=0; j<width; j++)
           		data[index++] = (byte)(ycbcr[i][j][2]&0xff); 	 
    }
    
    // Método que lê o RGB do BitMap e converte em YCbCr e depois compacta usando codificação de Huffman
    private void generateCUIF3(int[][][] rasterbmp) throws IOException {
    	
    	// Converte RGB do BMP para o raster YCbCr  
    	generateYCbCr(rasterbmp);
    	
    	// Gera raster YCbCr a ser compactado com codificação de Huffman
    	int index=0;
    	byte[] source = new byte[height*width*3];
  		// Coloca Y no array "source" 
   	 	for (int i=0; i<height; i++)  
            for (int j=0; j<width; j++)
            	source[index++] = (byte)(ycbcr[i][j][0]&0xff);
  		// Coloca Cb no array "source" 
  	 	for (int i=0; i<height; i++) 
   	 		for (int j=0; j<width; j++)
   	 			source[index++] = (byte)(ycbcr[i][j][1]&0xff);
  		// Coloca Cr no array "source" 
   	 	for (int i=0; i<height; i++) 
	 		for (int j=0; j<width; j++)
	 			source[index++] = (byte)(ycbcr[i][j][2]&0xff);
   	 	
   	 	// Gera array "data" com o array YCbCr codificado com Huffman
   	 	data = br.ufsc.ine5431.huffmancoding.HuffmanCoding.compression(source);  
    }

    // Método que lê o RGB do BitMap e  depois compacta usando codificação RLE
    private void generateCUIF4(int[][][] rasterbmp) {	
    	// Leitura do Raster do arquivo BMP e geração do raster RGB CUIF
    	readRaster(rasterbmp);
    	
    	System.out.println("Tamanho do dado original: " + data.length);
    	byte[] dataRLE = br.ufsc.ine5431.rlecoding.RLECoding.compress(data);
    	System.out.println("Tamanho do dado compactado RLE: " + dataRLE.length);
    	data = dataRLE;
    }
    
    // Leitura do raster do arquivo bitmap para salvar no raster CUIF
    // Durante a leitura, o método também salva no array de bytes "data" 
    // que representa os dados do arquivo cuif
    private void readRaster(int[][][] rasterbmp) {
    	int r, g, b; 
    	
    	raster=new int[height][width][3];
    	data = new byte[height*width*3];
    	int index=0;
   	 	for (int i=0; i<height; i++){
           for (int j=0; j<width; j++){
           		r = rasterbmp[i][j][0];
           		data[index++] = (byte)(r&0xff);
           		raster[i][j][0] = r;
           }
   	 	}
   	 	for (int i=0; i<height; i++){
            for (int j=0; j<width; j++){
            		g = rasterbmp[i][j][1];            	
               		data[index++] = (byte)(g&0xff);
               		raster[i][j][1] = g;
            }
    	 }
   	 	for (int i=0; i<height; i++){
            for (int j=0; j<width; j++){
            		b = rasterbmp[i][j][2];            	
               		data[index++] = (byte)(b&0xff);
               		raster[i][j][2] = b;
            }
    	 }
    }
    
    // Gera raster YCbCr e atualiza raster RGB após conversão
    // A entrada é a matriz rasterbmp, é o raster original, sendo que rasterbmp[1][2][0] é o R na posição (1,2)
    // o rasterbmp passa para um processo de conversão YCbCr para gerar o raster ycbcr[][][]
    // O atributo raster do objeto é o raster RGB obtido pela conversão do YCbCr
    private void generateYCbCr(int[][][] rasterbmp) {
    	ycbcr = new int[height][width][3];
    	int[] rgbconverted = new int[3];
    	int r, g, b;
    	raster=new int[height][width][3];
   		int pixelserror = 0;
   		
   	 	for (int i=0; i<height; i++) { 
           for (int j=0; j<width; j++){ 
          	   	r = (rasterbmp[i][j][0]&0xff);
           		g = (rasterbmp[i][j][1]&0xff);
           		b = (rasterbmp[i][j][2]&0xff);
           		           		
           		// Converte RGB para YCbCr
           		ycbcr[i][j] = ColorSpace.bt601_rgb_to_ycbcr(r, g, b);
           		
           		// Converte YCbCr para RGB para salvar no raster
           		rgbconverted = ColorSpace.bt601_ycbcr_to_rgb(ycbcr[i][j][0], ycbcr[i][j][1], ycbcr[i][j][2]);
           	
           		// Atualiza o raster RBG do arquivo
           		raster[i][j][0] = rgbconverted[0];
           		raster[i][j][1] = rgbconverted[1];
           		raster[i][j][2] = rgbconverted[2];
        
           		if ((r!=rgbconverted[0])||(g!=rgbconverted[1])||(b!=rgbconverted[2])) {
           			pixelserror++;
           		}
           	}
   	 	}
  		System.out.println("Número de pixels diferentes na conversão para YCbCr: " + pixelserror + " diferentes de " + height*width);
    }
    // Salva a imagem

   public void save(String filename){
    	 try { 
    		 	System.out.println("Salvando arquivo");
    		    System.out.println("Tamanho cabeçalho: " + header.length + " Bytes");
    		    System.out.println("Tamanho dado: " + data.length + " Bytes");
    		    FileOutputStream fileOuputStream = new FileOutputStream(filename); 
    		    fileOuputStream.write(header);
    		    fileOuputStream.write(data);
   			 	fileOuputStream.close();
    	 } catch (IOException ioex) {
         }
    }
 }
