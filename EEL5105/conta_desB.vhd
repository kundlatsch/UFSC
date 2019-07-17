library ieee ;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
entity conta_desB is
port(	CLOCK:	in std_logic;
		RESET:	in std_logic;
		EN_TIME:	in std_logic;
		INPUT: in std_logic_vector(31 downto 0);
		CONTA_B:	out std_logic_vector(4 downto 0)
		);
end conta_desB;
architecture bhv of conta_desB is		 	  
signal ENBL: std_logic;
signal count: std_logic_vector(4 downto 0);
	begin
	ENBL <= INPUT(31) and EN_TIME;
		 process(CLOCK, RESET, ENBL)
		 begin
			if RESET = '1' then
			 count <= "00011";
			elsif (CLOCK='1' and CLOCK'event) then
				if ENBL = '1' then
					count <= count - '1';
				end if;
			end if;
			CONTA_B <= count;
		 end process;
end bhv;