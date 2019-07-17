library IEEE;
use IEEE.Std_Logic_1164.all;
entity comparador_T is
port(EXT, CONTA_U: in std_logic_vector(9 downto 0);
LIMIT: out std_logic);
end comparador_T;
architecture bhv of comparador_T is
begin	
	process(CONTA_U)
	begin
		if (CONTA_U = EXT) 
		then
			LIMIT <= '1';
		else
			LIMIT <= '0';
		end if;
	end process;	
end bhv;