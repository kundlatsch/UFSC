library ieee; use ieee.std_logic_1164.all;

entity FSM_Ctrl is
port ( END_BONUS, END_TIME, ENBL, LIMIT, CLOCK_50,  RESET: in std_logic;
STATE: out std_logic_vector(4 downto 0);
SEL_DISPLAY: out std_logic_vector(1 downto 0);
SEL_LED, SET_ROL, EN_TIME, RST: out std_logic );
end FSM_Ctrl;
architecture FSM_bhv of FSM_Ctrl is
	type states is (E0, E1, E2, E3);
	signal EA, PE: states;	
begin
P1: process(CLOCK_50,  RESET)
begin--reset assincrono do controlador
	if  RESET = '0' then
		EA <= E0;
	elsif CLOCK_50'event and CLOCK_50 = '1' then 
		EA <= PE;
	end if;
end process;

P2: process (EA, ENBL, LIMIT, END_BONUS, END_TIME)
	begin

	case EA is
	when E0 => 
		SEL_LED <= '1';
		SEL_DISPLAY <= "10";
		SET_ROL <= '0';
		STATE <= "00000";		
		EN_TIME <= '0';
		RST <= '1';
		if ENBL = '0' then
			PE <= E1;
		else
			PE <= E0;
		end if;		
	when E1 =>
		SEL_LED <= '1';
		SEL_DISPLAY <= "01";
		SET_ROL <= '1';
		STATE <= "00001";		
		EN_TIME <= '0';
		RST <= '0';
		if ENBL = '0' then
			PE <= E2;
		else
			PE <= E1;
		end if;		
	when E2 => 
		SEL_LED <= '0';
		SEL_DISPLAY <= "00";
		SET_ROL <= '0';
		STATE <= "00010";		
		EN_TIME <= '1';
		RST <= '0';
		if (LIMIT ='1' or END_BONUS = '1' or END_TIME='1') then --Condições de ganhar, acabar o tempo, ou acabar o bonus
			PE <= E3;
		else
			PE <= E2;
		end if;		
	when E3 => 
		SEL_LED <= '1';
		SEL_DISPLAY <= "11";
		SET_ROL <= '0';
		STATE <= "00011";		
		EN_TIME <= '0';
		RST <= '0';
		if ENBL = '0' then
			PE <= E0;
		else
			PE <= E3;
		end if;	
		
	end case;
end process;

end FSM_bhv; -- fim da architecture
	