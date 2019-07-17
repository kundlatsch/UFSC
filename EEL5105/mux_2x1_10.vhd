library IEEE;
use IEEE.Std_Logic_1164.all;
entity mux_2x1_10 is
port (a, b: in std_logic_vector(9 downto 0);
		sai: out std_logic_vector(9 downto 0);
		sel: in std_logic
		);
end mux_2x1_10;
architecture bhv of mux_2x1_10 is
begin
	sai <= a when 
		sel = '1' 
	else 
		b;
end bhv;