library IEEE;
use IEEE.Std_Logic_1164.all;
entity comparador_B is
port(EXT,CONTA_B: in std_logic_vector(4 downto 0);
END_BONUS: out std_logic);
end comparador_B;
architecture bhv of comparador_B is
begin	
	process(CONTA_B)
	begin
		if (CONTA_B = EXT) then
			END_BONUS <= '1';
		else
			END_BONUS <= '0';
		end if;
	end process;	
end bhv;