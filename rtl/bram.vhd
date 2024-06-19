library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use ieee.numeric_std.all;

entity bram is
    generic ( WIDTH: integer := 8;
            BRAM_SIZE : integer := 80000
 );    
    port (    
              clka : in std_logic;
              clkb : in std_logic;
              reseta : in std_logic;
              resetb : in std_logic;
              ena : in std_logic;
              enb : in std_logic;
              wea : in std_logic;
              web : in std_logic;
              addra : in std_logic_vector(4*WIDTH-1 downto 0);
              addrb : in std_logic_vector(4*WIDTH-1 downto 0);
              dia: in std_logic_vector(3*WIDTH-1 downto 0);
              dib: in std_logic_vector(3*WIDTH-1 downto 0);
              doa: out std_logic_vector(3*WIDTH-1 downto 0);
              dob: out std_logic_vector(3*WIDTH-1 downto 0)
          );    
end bram;

architecture Behavioral of bram is
--type ram_type is array ( (2**(2*WIDTH))-1 downto 0) of std_logic_vector(3*WIDTH-1 downto 0);
type ram_type is array (BRAM_SIZE-1 downto 0) of std_logic_vector(3*WIDTH-1 downto 0);
shared variable RAM: ram_type;
begin
    process(clka)
        begin
            if(rising_edge (clka)) then
                if(ena = '1') then
                    doa <= RAM(to_integer(unsigned(addra)));
                end if;   
                
                if(wea = '1') then
                    RAM(to_integer(unsigned(addra))) := dia;    
                end if;
            end if;          
    end process;
    
    process(clkb)
        begin
            if(rising_edge (clkb)) then
                if(enb = '1') then
                    dob <= RAM(to_integer(unsigned(addrb)));
                end if;   
                
                if(web = '1') then
                    RAM(to_integer(unsigned(addrb))) := dib;    
                end if;
            end if;          
    end process;


end Behavioral;
