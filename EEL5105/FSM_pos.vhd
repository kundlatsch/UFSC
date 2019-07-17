library ieee; use ieee.std_logic_1164.all;

entity FSM_pos is
port ( CLOCK, RESET, EN_TIME, SW0, SW1: in std_logic;
      SPEED: in std_logic_vector(2 downto 0);
      STATE: out std_logic_vector(3 downto 0)
		);
end FSM_pos;
architecture FSM_bhv of FSM_pos is
	type states is (E0, E1, E2, E3, E4, E5, E6, E7, 
						E8, E9, E10, E11, E12, E13, E14, E15);
	signal ENBL: std_logic;
	signal EA, PE: states;	
begin  
  ENBL <= (SPEED(2) or SPEED(1) or SPEED(0)) and EN_TIME;  
  
P1: process(CLOCK, RESET)
begin
	if RESET = '0' then
		EA <= E7;
	elsif CLOCK'event and CLOCK = '1' and ENBL = '1' then 
		EA <= PE;
	end if;
end process;
P2: process (SW0, SW1, ENBL, EA)
	begin  
      case EA is        
      when E0 => 
        if SW0 = '0' and SW1 = '1' then
              PE <= E1;
          else
              PE <= E0;
          end if;
          STATE <= "0000";
			 
      when E1 => 
          if SW0 = '0' and SW1 = '1' then			--se a SW1 for usada, avançamos de estado
              PE <= E2;
          elsif SW0 = '1' and SW1 = '0' then		--se a a SW0 for usada, retornamos
              PE <= E0;
          else												--sem nenhuma Switch, mantemos o estado
             PE <= E1;
          end if;
          STATE <= "0001";
			 
      when E2 => 
          if SW0 = '0' and SW1 = '1' then
              PE <= E3;
          elsif SW0 = '1' and SW1 = '0' then
              PE <= E1;
          else
             PE <= E2;
          end if;
          STATE <= "0010"; 
			 
      when E3 => 
          if SW0 = '0' and SW1 = '1' then
              PE <= E4;
          elsif SW0 = '1' and SW1 = '0' then
              PE <= E2;
          else
             PE <= E3;
          end if;
          STATE <= "0011"; 
			 
      when E4 => 
          if SW0 = '0' and SW1 = '1' then
              PE <= E5;
          elsif SW0 = '1' and SW1 = '0' then
              PE <= E3;
          else
				PE <= E4;
          end if;
          STATE <= "0100"; 
			 
      when E5 => 
          if SW0 = '0' and SW1 = '1' then
              PE <= E6;
          elsif SW0 = '1' and SW1 = '0' then
              PE <= E4;
          else
             PE <= E5;
          end if;
          STATE <= "0101";  
			 
        when E6 => 
          if SW0 = '0' and SW1 = '1' then
              PE <= E7;
          elsif SW0 = '1' and SW1 = '0' then
              PE <= E5;
          else
             PE <= E6;
          end if;
          STATE <= "0110"; 
			 
        when E7 => 
          if SW0 = '0' and SW1 = '1' then
              PE <= E8;
          elsif SW0 = '1' and SW1 = '0' then
              PE <= E6;
          else
             PE <= E7;
          end if;
          STATE <= "0111";
			 
        when E8 => 
          if SW0 = '0' and SW1 = '1' then
              PE <= E9;
          elsif SW0 = '1' and SW1 = '0' then
              PE <= E7;
          else
             PE <= E8;
          end if;
          STATE <= "1000";
			 
        when E9 => 
          if SW0 = '0' and SW1 = '1' then
              PE <= E10;
          elsif SW0 = '1' and SW1 = '0' then
              PE <= E8;
          else
             PE <= E9;
          end if;
          STATE <= "1001";
			 
        when E10 => 
          if SW0 = '0' and SW1 = '1' then
              PE <= E11;
          elsif SW0 = '1' and SW1 = '0' then
              PE <= E9;
          else
             PE <= E10;
          end if;
          STATE <= "1010"; 
			 
        when E11 => 
          if SW0 = '0' and SW1 = '1' then
              PE <= E12;
          elsif SW0 = '1' and SW1 = '0' then
              PE <= E10;
          else
             PE <= E11;
          end if;
          STATE <= "1011";
			 
        when E12 => 
          if SW0 = '0' and SW1 = '1' then
              PE <= E13;
          elsif SW0 = '1' and SW1 = '0' then
              PE <= E11;
          else
             PE <= E12;
          end if;
          STATE <= "1100"; 
			 
        when E13 => 
          if SW0 = '0' and SW1 = '1' then
              PE <= E14;
          elsif SW0 = '1' and SW1 = '0' then
              PE <= E12;
          else
             PE <= E13;
          end if;
          STATE <= "1101"; 
			 
        when E14 => 
          if SW0 = '0' and SW1 = '1' then
              PE <= E15;
          elsif SW0 = '1' and SW1 = '0' then
              PE <= E13;
          else
             PE <= E14;
          end if;
          STATE <= "1110"; 
			 
        when E15 => 
          if SW0 = '1' and SW1 = '0' then
             PE <= E14;
          else
             PE <= E15;
          end if;
          STATE <= "1111";			 
      end case;   
end process;
end FSM_bhv;
	