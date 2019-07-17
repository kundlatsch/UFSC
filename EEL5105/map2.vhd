library ieee;
use ieee.std_logic_1164.all;

entity map2 is
	port
	(
		F0, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13, F14, F15: out std_logic_vector(31 downto 0)
	);
end map2;	
	
architecture map2_struct of map2 is
begin
	F0 <=  "00000000000000011100000011000000";
	F1 <=  "00000001100000001000000011000000";
	F2 <=  "00000001100000000000000011000000";
	F3 <=  "00000001100000000000000011000000";
	F4 <=  "00000001111000011100000011000000";
	F5 <=  "00000000111000000000000011000000";
	F6 <=  "00000000001000000000000011000000";
	F7 <=  "00000110001000000010000000000000";
	F8 <=  "00000110000000000000000011000000";
	F9 <=  "00000000000001110000000011000000";
	F10 <= "00000011000011100001000011000000";
	F11 <= "00000011000000000001000011000000";
	F12 <= "00000001110000000000000011000000";
	F13 <= "00000000011100000100000011000000";
	F14 <= "00000011000000000000000011000000";
	F15 <= "00000000000110000000000011000000";
end map2_struct;