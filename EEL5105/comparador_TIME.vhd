library IEEE;
use IEEE.Std_Logic_1164.all;
use ieee.std_logic_unsigned.all;

entity comparador_TIME is

port(EXT: in std_logic_vector(9 downto 0);
CONTA_D: in std_logic_vector(9 downto 0);
END_TIME: out std_logic);
end comparador_TIME;
architecture bhv of comparador_TIME is
begin	
	P1 : process(CONTA_D, EXT)
	begin
		if (CONTA_D = EXT) then
			END_TIME <= '1';
		else
			END_TIME <= '0';
		end if;
	end process;	
end bhv;