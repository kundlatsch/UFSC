library ieee ;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
entity conta_ASC is
port (CLOCK:	in std_logic;
		RESET:	in std_logic;
		EN_TIME:	in std_logic;
		SPEED: in std_logic_vector(2 downto 0);
		CONTA_U:	out std_logic_vector(9 downto 0)
		);
end conta_ASC;
architecture bhv of conta_ASC is		 	  
signal ENBL: std_logic;
signal contador: std_logic_vector(9 downto 0);
begin
ENBL <= (SPEED(2) or SPEED(1) or SPEED(0)) and EN_TIME;	 
    process(CLOCK, ENBL, RESET)
    begin
		if RESET = '1' then
			contador <= "0000000000";
		elsif (CLOCK='1' and CLOCK'event) then
			if ENBL = '1' then
				contador(4 downto 0) <= contador(4 downto 0) + '1';
				if contador(4 downto 0)  = "01111" then			
					contador(9 downto 5) <= contador(9 downto 5) + '1';
					contador(4 downto 0) <= "00000";					
				end if;
			end if;
		end if;
	CONTA_U <= contador;
 end process;	

end bhv;