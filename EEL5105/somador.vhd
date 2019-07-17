library IEEE;
use IEEE.Std_Logic_1164.all;
use IEEE.std_logic_unsigned.all; -- é necessario para realizar a operacao de soma
entity somador is
port (CONTA_U: in std_logic_vector(9 downto 0);
EXT: in std_logic_vector(9 downto 0);
POINT: out std_logic_vector(9 downto 0));
end somador;
architecture bhv of somador is
begin
	POINT <= CONTA_U + EXT;
end bhv;