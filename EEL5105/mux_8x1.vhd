library IEEE;
use IEEE.Std_Logic_1164.all;
entity mux_8x1 is
port (a, b, c, d, e: in std_logic;
sel: in std_logic_vector(2 downto 0);
sai: out std_logic
);
end mux_8x1;
architecture bhv of mux_8x1 is
	begin
	sai <= b when sel = "010" else 
			 c when sel = "011" else
			 d when sel = "100" else
			 e when sel = "101" else	  
			 a;
end bhv;