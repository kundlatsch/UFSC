library IEEE;
use IEEE.Std_Logic_1164.all;
entity decod7seg is 
	port( coded: in std_logic_vector(4 downto 0);
			decoded: out std_logic_vector(6 downto 0)
			);
end decod7seg;
architecture bhv of decod7seg is
	begin 	
	decoded <= "1000000" when coded = "00000" else --0
		  "1111001" when coded = "00001" else --1
		  "0100100" when coded = "00010" else --2
		  "0110000" when coded = "00011" else --3
		  "0011001" when coded = "00100" else --4
		  "0010010" when coded = "00101" else --5
		  "0000010" when coded = "00110" else --6
		  "1111000" when coded = "00111" else --7
		  "0000000" when coded = "01000" else --8
		  "0010000" when coded = "01001" else --9
		  "0001000" when coded = "01010" else --a
		  "0000011" when coded = "01011" else --b
		  "1000110" when coded = "01100" else --c
		  "0100001" when coded = "01101" else --d
		  "0000110" when coded = "01110" else --e
		  "0001110" when coded = "01111" else --f
		  "0010000" when coded = "10000" else --g
		  "0001001" when coded = "10001" else --h
		  "1111001" when coded = "10010" else --i
		  "0101011" when coded = "10110" else --m
		  "0001100" when coded = "11000" else --p
		  "1111111" when coded = "11111" else --Espaço em branco
		  "0000111"; --t
end bhv;