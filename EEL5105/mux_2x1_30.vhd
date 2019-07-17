library IEEE;
use IEEE.Std_Logic_1164.all;
entity mux_2x1_30 is
port (a, b: in std_logic_vector(29 downto 0);
sai: out std_logic_vector(29 downto 0);
sel: in std_logic
);
end mux_2x1_30;
architecture circuito of mux_2x1_30 is
begin
	sai <= a when 
		sel = '0' 
	else 
		b;
end circuito;