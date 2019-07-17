library IEEE;
use IEEE.Std_Logic_1164.all;
entity mux_16x1 is
port (a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p:	in std_logic_vector(31 downto 0);
		sel: in std_logic_vector(3 downto 0);
		sai: out std_logic_vector(31 downto 0)
		);
end mux_16x1;
architecture bhv of mux_16x1 is
	begin
	sai <= a when sel = "0000" else
			 b when sel = "0001" else
			 c when sel = "0010" else
			 d when sel = "0011" else
			 e when sel = "0100" else
			 f when sel = "0101" else
			 g when sel = "0110" else
			 h when sel = "0111" else
			 i when sel = "1000" else
			 j when sel = "1001" else
			 k when sel = "1010" else
			 l when sel = "1011" else
			 m when sel = "1100" else
			 n when sel = "1101" else
			 o when sel = "1110" else
			 p;
end bhv;