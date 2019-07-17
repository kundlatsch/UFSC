library ieee;
use ieee.std_logic_1164.all;
entity topo is
	port (SW 	  : in std_logic_vector (9 downto 0);
			KEY	  : in std_logic_vector (3 downto 0);
			CLOCK_50: in std_logic;
			HEX0    : out std_logic_vector(6 downto 0);
			HEX1	  : out std_logic_vector(6 downto 0);
			HEX2	  : out std_logic_vector(6 downto 0);
			HEX3	  : out std_logic_vector(6 downto 0);
			HEX4	  : out std_logic_vector(6 downto 0);
			HEX5	  : out std_logic_vector(6 downto 0);
			LEDR	  : out std_logic_vector (9 downto 0)
	);
end topo;
architecture topo_stru of topo is
--INICIA SIGNALS

signal BTN0, BTN1, BTN2, BTN3, CLK1, CLK2, CLK3, CLK4, CLK5, CLOCK_M, EN_TIME, END_BONUS, END_TIME, RST, SEL_LED, SET_ROL, LIMIT: std_logic;
signal CONTA_B, STATE: std_logic_vector(4 downto 0);
signal CONTA_D, CONTA_U, LED_OUT, POINT: std_logic_vector(9 downto 0);
signal CONCAT_2x1_30_1,CONCAT_2x1_30_2: std_logic_vector(29 downto 0);
signal CONCAT_2X1_10:std_logic_vector(9 downto 0);
signal CONCAT_4x1_30_1,CONCAT_4x1_30_2,CONCAT_4x1_30_3:std_logic_vector(29 downto 0);
signal CONCAT_SOMADOR: std_logic_vector(9 downto 0);
signal H:std_logic_vector(41 downto 0);
signal HX,SCREEN: std_logic_vector(29 downto 0);
signal SEL, SEL_DISPLAY: std_logic_vector (1 downto 0);
signal SPEED: std_logic_vector(2 downto 0);
signal UP_DOWN: std_logic_vector(3 downto 0);

--SIGNALS DE CADA MAPA
signal MAP1_0, MAP1_1, MAP1_2, MAP1_3, MAP1_4, MAP1_5, MAP1_6, MAP1_7, MAP1_8, MAP1_9, MAP1_10, MAP1_11, MAP1_12, MAP1_13, MAP1_14, MAP1_15:std_logic_vector(31 downto 0);
signal MAP2_0, MAP2_1, MAP2_2, MAP2_3, MAP2_4, MAP2_5, MAP2_6, MAP2_7, MAP2_8, MAP2_9, MAP2_10, MAP2_11, MAP2_12, MAP2_13, MAP2_14, MAP2_15: std_logic_vector(31 downto 0);
signal MAP3_0, MAP3_1, MAP3_2, MAP3_3, MAP3_4, MAP3_5, MAP3_6, MAP3_7, MAP3_8, MAP3_9, MAP3_10, MAP3_11, MAP3_12, MAP3_13, MAP3_14, MAP3_15: std_logic_vector(31 downto 0);
signal MAP4_0, MAP4_1, MAP4_2, MAP4_3, MAP4_4, MAP4_5, MAP4_6, MAP4_7, MAP4_8, MAP4_9, MAP4_10, MAP4_11, MAP4_12, MAP4_13, MAP4_14, MAP4_15: std_logic_vector(31 downto 0);

--SIGNALS DE REGISTRADORES
signal REG_IN_0, REG_IN_1, REG_IN_2, REG_IN_3, REG_IN_4, REG_IN_5, REG_IN_6, REG_IN_7, REG_IN_8, REG_IN_9, REG_IN_10, REG_IN_11, REG_IN_12, REG_IN_13, REG_IN_14, REG_IN_15, REG_OUT, REG_OUT_0,REG_OUT_1,REG_OUT_2,REG_OUT_3,REG_OUT_4,REG_OUT_5,REG_OUT_6,REG_OUT_7,
REG_OUT_8,REG_OUT_9,REG_OUT_10,REG_OUT_11,REG_OUT_12,REG_OUT_13,REG_OUT_14,REG_OUT_15: std_logic_vector(31 downto 0);

--FIM DOS SIGNALS
--INICIA COMPONENTS

--DECODIFICADOR
component decod7seg
	port (coded  : in std_logic_vector(4 downto 0);
			decoded: out std_logic_vector(6 downto 0)
	);
end component;



--SINCRONIZADOR
component ButtonSync
	port 	(
			-- Input ports
			key0	: in  std_logic;
			key1	: in  std_logic;
			key2	: in  std_logic;
			key3	: in  std_logic;	
			clk : in std_logic;
			-- Output ports
			btn0	: out std_logic;
			btn1	: out std_logic;
			btn2	: out std_logic;
			btn3	: out std_logic
		);
end component;



--MAPAS
component map1
	port (F0, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13, F14, F15: out std_logic_vector(31 downto 0)
			);
end component;
component map2
	port (F0, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13, F14, F15: out std_logic_vector(31 downto 0)
			);
end component;
component map3
	port (F0, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13, F14, F15: out std_logic_vector(31 downto 0)
			);
end component;
component map4
	port (F0, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13, F14, F15: out std_logic_vector(31 downto 0)
			);
end component;


--MAQUINAS DE ESTADO
component FSM_spd
	port (CLOCK_50, RESET, EN_TIME,
			UP, DOWN: in std_logic;
			SPEED	  : out std_logic_vector(2 downto 0)
			);
end component;
component FSM_pos
	port (CLOCK, RESET, EN_TIME,
			SW0, SW1: in std_logic;
			SPEED	  : in std_logic_vector(2 downto 0);
			STATE	  : out std_logic_vector(3 downto 0)
			);
end component;
component FSM_CLK
	port (CLOCK_50	 : in std_logic;
			CLK1, CLK2, CLK3,
			CLK4, CLK5: buffer std_logic
			);
end component;
component FSM_Ctrl
	port (END_BONUS, END_TIME, ENBL, LIMIT,
			CLOCK_50, RESET: in std_logic;
			STATE				: out std_logic_vector(4 downto 0);
			SEL_DISPLAY		: out std_logic_vector(1 downto 0);
			SEL_LED, SET_ROL,
			EN_TIME, RST	: out std_logic 
			);
end component;


--MULTIPLEXADORES 
component mux_2x1_10
	port (a, b: in std_logic_vector(9 downto 0);
			sel: in std_logic;
			sai: out std_logic_vector(9 downto 0)
			);
end component;
component mux_2x1_30
	port (a, b: in std_logic_vector(29 downto 0);
			sel: in std_logic;
			sai: out std_logic_vector(29 downto 0)
			);
end component;
component mux_4x1_30
	port (a, b, c, d: in std_logic_vector(29 downto 0);
			sel: in std_logic_vector(1 downto 0);
			sai: out std_logic_vector(29 downto 0)
			);
end component;
component mux_4x1_32
	port (a, b, c, d: in std_logic_vector(31 downto 0);
			sel		 : in std_logic_vector(1 downto 0);
			sai		 : out std_logic_vector(31 downto 0)
			);
end component;
component mux_8x1
	port (a, b, c, d, e: in std_logic;
			sel			 : in std_logic_vector(2 downto 0);
			sai			 : out std_logic
			);
end component;
component mux_16x1
	port (a, b, c, d, e, f, g, h, i, j,k, l, 
			m, n, o, p:	in std_logic_vector(31 downto 0);
			sel		 : in std_logic_vector(3 downto 0);
			sai       : out std_logic_vector(31 downto 0)
			);
end component;


--SOMADOR
component somador
	port (CONTA_U: in std_logic_vector(9 downto 0);
			EXT: in std_logic_vector(9	downto 0);
			POINT: out std_logic_vector(9 downto 0)
			);
end component;



--COMPARADORES 
component comparador_T
	port (EXT, CONTA_U: in std_logic_vector(9 downto 0);
			LIMIT			: out std_logic
			);
end component;
component comparador_TIME
	port (EXT,CONTA_D: in std_logic_vector(9 downto 0);
			END_TIME   : out std_logic
			);
end component;
component comparador_B
	port (EXT, CONTA_B: in std_logic_vector(4 downto 0);
			END_BONUS   : out std_logic
			);
end component;



--CONTADORES (ASCENDENTE/DESCENDETE)
component conta_ASC
	port (CLOCK	 	 : in std_logic;
			RESET	    : in std_logic;
			EN_TIME	 :	in std_logic;
			SPEED		 :	in std_logic_vector(2 downto 0);
			CONTA_U	 :	out std_logic_vector(9 downto 0)
			);
end component;
component conta_desA
	port (CLOCK:	in std_logic;
			RESET:	in std_logic;
			ENBL:		in std_logic;
			CONTA_D:	out std_logic_vector(9 downto 0)
			);
end component;
component conta_desB
	port(	CLOCK  :	in std_logic;
			RESET  :	in std_logic;
			EN_TIME:	in std_logic;
			INPUT : in std_logic_vector(31 downto 0);
			CONTA_B:	out std_logic_vector(4 downto 0)
			);
end component;



--SET_ROL
component R_ROL
	port (CLK	  : in std_logic;
			EN_TIME : in std_logic;
			SPEED	  : in std_logic_vector(2 downto 0);
			RESET	  : in std_logic;
			input	  : in std_logic_vector(31 downto 0);
			SET_ROL : in std_logic;
			output  : out std_logic_vector(31 downto 0)
			);
end component;
--TERMINA COMPONENTS

begin 
--HEXS
L1: decod7seg port map(HX(29 downto 25), H(41 downto 35));
L2: decod7seg port map(HX(24 downto 20), H(34 downto 28));
L3: decod7seg port map(HX(19 downto 15), H(27 downto 21));
L4: decod7seg port map(HX(14 downto 10), H(20 downto 14));
L5: decod7seg port map(HX(9 downto 5), H(13 downto 7));
L6: decod7seg port map(HX(4 downto 0), H(6 downto 0));

-- ButtonSync - SINCRONIZADOR
L7: ButtonSync port map (KEY(0), KEY(1), KEY(2), KEY(3), CLOCK_50, BTN0, BTN1, BTN2, BTN3); 
 
-- Mapas para sinal
L8: map1 port map (MAP1_0, MAP1_1, MAP1_2, MAP1_3, MAP1_4, MAP1_5, MAP1_6, MAP1_7, MAP1_8, MAP1_9, MAP1_10, MAP1_11, MAP1_12, MAP1_13, MAP1_14, MAP1_15);
L9: map2 port map (MAP2_0, MAP2_1, MAP2_2, MAP2_3, MAP2_4, MAP2_5, MAP2_6, MAP2_7, MAP2_8, MAP2_9, MAP2_10, MAP2_11, MAP2_12, MAP2_13, MAP2_14, MAP2_15);
L10: map3 port map (MAP3_0, MAP3_1, MAP3_2, MAP3_3, MAP3_4, MAP3_5, MAP3_6, MAP3_7, MAP3_8, MAP3_9, MAP3_10, MAP3_11, MAP3_12, MAP3_13, MAP3_14, MAP3_15);
L11: map4 port map (MAP4_0, MAP4_1, MAP4_2, MAP4_3, MAP4_4, MAP4_5, MAP4_6, MAP4_7, MAP4_8, MAP4_9, MAP4_10, MAP4_11, MAP4_12, MAP4_13, MAP4_14, MAP4_15);
-- MUX dos mapas
L12: mux_4x1_32 port map (MAP1_0, MAP2_0, MAP3_0, MAP4_0, SW(8 downto 7), REG_IN_0);
L13: mux_4x1_32 port map (MAP1_1, MAP2_1, MAP3_1, MAP4_1, SW(8 downto 7), REG_IN_1);
L14: mux_4x1_32 port map (MAP1_2, MAP2_2, MAP3_2, MAP4_2, SW(8 downto 7), REG_IN_2);
L15: mux_4x1_32 port map (MAP1_3, MAP2_3, MAP3_3, MAP4_3, SW(8 downto 7), REG_IN_3);
L16: mux_4x1_32 port map (MAP1_4, MAP2_4, MAP3_4, MAP4_4, SW(8 downto 7), REG_IN_4);
L17: mux_4x1_32 port map (MAP1_5, MAP2_5, MAP3_5, MAP4_5, SW(8 downto 7), REG_IN_5);
L18: mux_4x1_32 port map (MAP1_6, MAP2_6, MAP3_6, MAP4_6, SW(8 downto 7), REG_IN_6);
L19: mux_4x1_32 port map (MAP1_7, MAP2_7, MAP3_7, MAP4_7, SW(8 downto 7), REG_IN_7);
L20: mux_4x1_32 port map (MAP1_8, MAP2_8, MAP3_8, MAP4_8, SW(8 downto 7), REG_IN_8);
L21: mux_4x1_32 port map (MAP1_9, MAP2_9, MAP3_9, MAP4_9, SW(8 downto 7), REG_IN_9);
L22: mux_4x1_32 port map (MAP1_10, MAP2_10, MAP3_10, MAP4_10, SW(8 downto 7), REG_IN_10);
L23: mux_4x1_32 port map (MAP1_11, MAP2_11, MAP3_11, MAP4_11, SW(8 downto 7), REG_IN_11);
L24: mux_4x1_32 port map (MAP1_12, MAP2_12, MAP3_12, MAP4_12, SW(8 downto 7), REG_IN_12);
L25: mux_4x1_32 port map (MAP1_13, MAP2_13, MAP3_13, MAP4_13, SW(8 downto 7), REG_IN_13);
L26: mux_4x1_32 port map (MAP1_14, MAP2_14, MAP3_14, MAP4_14, SW(8 downto 7), REG_IN_14);
L27: mux_4x1_32 port map (MAP1_15, MAP2_15, MAP3_15, MAP4_15, SW(8 downto 7), REG_IN_15);
 
-- MAQUINAS DE ESTADO
L28: FSM_Ctrl port map (END_BONUS, END_TIME, BTN1, LIMIT, CLOCK_50, BTN0, STATE, SEL_DISPLAY, SEL_LED, SET_ROL, EN_TIME, RST);   
L29: FSM_CLK  port map (CLOCK_50, CLK1, CLK2, CLK3, CLK4, CLK5);
L30: FSM_spd  port map (CLOCK_50, BTN0, EN_TIME, BTN2, BTN3, SPEED);
L31: FSM_pos  port map (CLOCK_M, BTN0, EN_TIME, SW(0), SW(1), SPEED, UP_DOWN);

-- MULTIPLEXADORES
CONCAT_2X1_10<=(CLK5 & REG_OUT(30 downto 22));
CONCAT_2x1_30_1<=("01110" & STATE & "01011" & CONTA_B & CONTA_D);
CONCAT_2x1_30_2<=("00101" & "00" & SPEED & "11000" & "0" & UP_DOWN & CONTA_U );
CONCAT_4x1_30_1<=("01110" & STATE & "101100101011000000" & SW(8) & SW(7));
CONCAT_4x1_30_2<=("01110" & STATE & "11111000000111101111");
CONCAT_4x1_30_3<=("01110" & STATE & "1100011101" & POINT);

L32: mux_2x1_30 port map(CONCAT_2x1_30_1, CONCAT_2x1_30_2,SW(9),SCREEN);
L33: mux_2x1_10 port map("0000000000",CONCAT_2X1_10,SEL_LED, LED_OUT);
L34: mux_4x1_30 port map(SCREEN, CONCAT_4x1_30_1,CONCAT_4x1_30_2 , CONCAT_4x1_30_3 , SEL_DISPLAY, HX);
L35: mux_8x1 port map (CLK1, CLK2, CLK3, CLK4, CLK5, SPEED, CLOCK_M);
L36: mux_16x1 port map(REG_OUT_0,REG_OUT_1,REG_OUT_2,REG_OUT_3,REG_OUT_4,REG_OUT_5,REG_OUT_6,REG_OUT_7,
REG_OUT_8,REG_OUT_9,REG_OUT_10,REG_OUT_11,REG_OUT_12,REG_OUT_13,REG_OUT_14,REG_OUT_15, UP_DOWN, REG_OUT);

--Somador e Comparadores
CONCAT_SOMADOR<=("0000" & CONTA_B & '0');
L37: comparador_TIME port map("0000000000", CONTA_D, END_TIME);
L38: comparador_T port map("0010000000", CONTA_U, LIMIT);
L39: comparador_B port map("00000", CONTA_B, END_BONUS);
L40: somador port map(CONTA_U,CONCAT_SOMADOR, POINT);

--CONTADORES
L45: conta_desA port map(CLK1, RST, EN_TIME, CONTA_D);
L46: conta_desB port map(CLOCK_M, RST, EN_TIME, REG_OUT, CONTA_B);
L47: conta_ASC port map(CLOCK_M, RST, EN_TIME, SPEED,CONTA_U);

--SET_ROL
L48: R_ROL port map(CLOCK_M, EN_TIME, SPEED, RST, REG_IN_0, SET_ROL,REG_OUT_0);
L49: R_ROL port map(CLOCK_M, EN_TIME, SPEED, RST, REG_IN_1, SET_ROL,REG_OUT_1);
L50: R_ROL port map(CLOCK_M, EN_TIME, SPEED, RST, REG_IN_2, SET_ROL,REG_OUT_2);
L51: R_ROL port map(CLOCK_M, EN_TIME, SPEED, RST, REG_IN_3, SET_ROL,REG_OUT_3);
L52: R_ROL port map(CLOCK_M, EN_TIME, SPEED, RST, REG_IN_4, SET_ROL,REG_OUT_4);
L53: R_ROL port map(CLOCK_M, EN_TIME, SPEED, RST, REG_IN_5, SET_ROL,REG_OUT_5);
L54: R_ROL port map(CLOCK_M, EN_TIME, SPEED, RST, REG_IN_6, SET_ROL,REG_OUT_6);
L55: R_ROL port map(CLOCK_M, EN_TIME, SPEED, RST, REG_IN_7, SET_ROL,REG_OUT_7);
L56: R_ROL port map(CLOCK_M, EN_TIME, SPEED, RST, REG_IN_8, SET_ROL,REG_OUT_8);
L57: R_ROL port map(CLOCK_M, EN_TIME, SPEED, RST, REG_IN_9, SET_ROL,REG_OUT_9);
L58: R_ROL port map(CLOCK_M, EN_TIME, SPEED, RST, REG_IN_10, SET_ROL,REG_OUT_10);
L59: R_ROL port map(CLOCK_M, EN_TIME, SPEED, RST, REG_IN_11, SET_ROL,REG_OUT_11);
L60: R_ROL port map(CLOCK_M, EN_TIME, SPEED, RST, REG_IN_12, SET_ROL,REG_OUT_12);
L61: R_ROL port map(CLOCK_M, EN_TIME, SPEED, RST, REG_IN_13, SET_ROL,REG_OUT_13);
L62: R_ROL port map(CLOCK_M, EN_TIME, SPEED, RST, REG_IN_14, SET_ROL,REG_OUT_14);
L63: R_ROL port map(CLOCK_M, EN_TIME, SPEED, RST, REG_IN_15, SET_ROL,REG_OUT_15);

--HEXS
HEX5 <= H(41 downto 35);
HEX4 <=H(34 downto 28);
HEX3 <=H(27 downto 21);
HEX2 <=H(20 downto 14);
HEX1 <=H(13 downto 7);
HEX0 <=H(6 downto 0);

LEDR <= LED_OUT;
end topo_stru;
