library ieee; use ieee.std_logic_1164.all;

entity FSM_spd is
port (CLOCK_50, RESET, EN_TIME, UP, DOWN: in std_logic;
      SPEED: out std_logic_vector(2 downto 0)
		);
end FSM_spd;
architecture FSM_bhv of FSM_spd is
	type states is (E0, E1, E2, E3, E4, E5);
	signal ENBL: std_logic;
	signal EA, PE: states;	
begin  
  ENBL <= EN_TIME;  
  P1: process(RESET, CLOCK_50)
begin
	if RESET = '0' then
		EA <= E0;
	elsif CLOCK_50'event and CLOCK_50 = '1' and ENBL = '1' then 
		EA <= PE;
	end if;
end process;
P2: process (UP, DOWN, ENBL, EA)
	begin	
      case EA is        
			when E0 => 
				if UP = '0' and DOWN = '1' then
					  PE <= E1;
				 else
					  PE <= E0;
				 end if;
				 SPEED <= "000";
			when E1 => 										--de forma semelhante a FSM_pos.
				 if UP = '0' and DOWN = '1' then    --Caso up seja pressionado, avançamos estado, aumentando a velocidade.
					  PE <= E2;
				 elsif UP = '1' and DOWN = '0' then --Se down for pressionado, retornamos ao estado anteriaor, diminuindo a 	velocidade.
					  PE <= E0;
				 else											--se nenhum for pressionado o estado atual se mantem.
					 PE <= E1;
				 end if;
				 SPEED <= "001";
			when E2 => 
				 if UP = '0' and DOWN = '1' then
					  PE <= E3;
				 elsif UP = '1' and DOWN = '0' then
					  PE <= E1;
				 else
					 PE <= E2;
				 end if;
				 SPEED <= "010";        
			when E3 => 
				 if UP = '0' and DOWN = '1' then
					  PE <= E4;
				 elsif UP = '1' and DOWN = '0' then
					  PE <= E2;
				 else
					 PE <= E3;
				 end if;
				 SPEED <= "011";        
			when E4 => 
				 if UP = '0' and DOWN = '1' then
					  PE <= E5;
				 elsif UP = '1' and DOWN = '0' then
					  PE <= E3;
				 else
					 PE <= E4;
				 end if;
				 SPEED <= "100";        
			when E5 => 
				 if UP = '1' and DOWN = '0' then
					 PE <= E4;
				 else
					 PE <= E5;
				 end if;
				 SPEED <= "101";        
      end case;      
end process;
end FSM_bhv;
	