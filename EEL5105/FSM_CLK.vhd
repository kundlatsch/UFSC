library ieee; 
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;

entity FSM_CLK is
port ( CLOCK_50: in std_logic;
		CLK1, CLK2, CLK3, CLK4, CLK5: buffer std_logic
		);
end FSM_CLK;

architecture FSM_bhv of FSM_CLK is
	type states is (E0, E1, E2, E3);
	signal EA, PE: states;
	signal count1,count2,count3,count4,count5: std_logic_vector(27 downto 0):= x"0000000";	
begin
	P1: process(CLOCK_50)
		begin
		if CLOCK_50'event and CLOCK_50= '1' then
			count1 <=count1+'1';
			if count1 = x"2FAF07F" then
				CLK1 <= '1';
				count1 <= x"0000000";				
			else
				CLK1 <= '0';
			end if;
		end if;
			
		end process;
	
		P2: process(CLOCK_50)
		begin
		if CLOCK_50'event and CLOCK_50= '1' then
			count2 <=count2+'1';
			if count2 = x"17D783F" then
				CLK2 <= '1';
				count2 <= x"0000000";				
			else
				CLK2 <= '0';
			end if;
		end if;		
		end process;
		
		P3: process(CLOCK_50)
		begin
		if CLOCK_50'event and CLOCK_50= '1' then
			count3 <=count3+'1';
			if count3 = x"FE502A" then
				CLK3 <= '1';
				count3 <= x"0000000";			
			else
				CLK3 <= '0';
			end if;
		end if;
		end process;
		
		P4: process(CLOCK_50)
		begin
		if CLOCK_50'event and CLOCK_50= '1' then
			count4 <=count4+'1';
			if count4 = x"BEBC1F" then
				CLK4 <= '1';
				count4 <= x"0000000";			
			else
				CLK4 <= '0';
			end if;
		end if;
		end process;
		
		P5: process(CLOCK_50)
		begin
		if CLOCK_50'event and CLOCK_50= '1' then
			count5 <=count5+'1';
			if count5 = x"98967F" then
				count5 <= x"0000000";
				if CLK5 = '0' then
				CLK5 <= '1';
				else
				CLK5 <= '0';
				end if;
			end if;
		end if;
		end process;
end FSM_bhv; 
	