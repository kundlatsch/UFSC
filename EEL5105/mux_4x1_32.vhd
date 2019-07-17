library IEEE;
use IEEE.Std_Logic_1164.all;
entity mux_4x1_32 is
port (a, b, c, d: in std_logic_vector(31 downto 0);
sel: in std_logic_vector(1 downto 0);
sai: out std_logic_vector(31 downto 0)
);
end mux_4x1_32;
architecture bhv of mux_4x1_32 is
	begin
		sai <= a when sel = "00" else
			    b when sel = "01" else 
			    c when sel = "10" else 
			    d;
end bhv;