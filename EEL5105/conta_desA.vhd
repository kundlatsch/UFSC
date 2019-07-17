library ieee ;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
entity conta_desA is
port(	CLOCK:	in std_logic;
		RESET:	in std_logic;
		ENBL:	in std_logic;
		CONTA_D:	out std_logic_vector(9 downto 0)
		);
end conta_desA;
architecture bhv of conta_desA is		
signal count: std_logic_vector(9 downto 0); 	  
begin
    process(CLOCK, ENBL, RESET)
    begin
	if RESET = '1' then
 	   count <= "0100101001";
	elsif (CLOCK='1' and CLOCK'event) then
	    if ENBL = '1' then
			count(4 downto 0) <= count(4 downto 0) - '1';
			if count(4 downto 0) = "00000" then			
				count(4 downto 0) <= "01001";
				count(9 downto 5) <= count(9 downto 5) - '1';
			end if;
	    end if;
	end if;
    end process;		 
	 CONTA_D <= count;

end bhv;