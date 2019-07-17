library ieee;
use ieee.std_logic_1164.all;
entity R_ROL is
port( CLK: in std_logic;
		EN_TIME : in std_logic;
		SPEED: in std_logic_vector(2 downto 0);
		RESET: in std_logic;
		input: in std_logic_vector(31 downto 0);
		SET_ROL: in std_logic;
		output : out std_logic_vector(31 downto 0)
		);
end entity;
architecture bhv of R_ROL is
signal reg: std_logic_vector (31 downto 0);
signal reg_ext, ENBL: std_logic;
	begin
	ENBL <= (SPEED(2) or SPEED(1) or SPEED(0)) and EN_TIME; 
		process (CLK, RESET)
		begin
		if (RESET = '1') then -- Reset assincrono
			reg <= (others => '0');
		elsif (SET_ROL = '1') then -- Sinal de clock
				reg <= input;
			elsif (rising_edge(clk)) then		
			if (ENBL ='1') then	-- Sinal de enable do registrador				
			-- Desloca 1 bit para a esquerda
				reg_ext <= reg(31);
				reg(31 downto 1) <= reg(30 downto 0);
				reg(0) <= reg_ext;
			end if;
		end if;	
	end process;
	output <= reg;
end bhv;