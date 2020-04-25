-- Copyright 1986-2018 Xilinx, Inc. All Rights Reserved.
-- --------------------------------------------------------------------------------
-- Tool Version: Vivado v.2018.1 (win64) Build 2188600 Wed Apr  4 18:40:38 MDT 2018
-- Date        : Sun Apr 19 14:10:31 2020
-- Host        : zsh-PC running 64-bit Service Pack 1  (build 7601)
-- Command     : write_vhdl -force -mode funcsim
--               e:/vivadoProj/vTestB/project_1/project_1.srcs/sources_1/bd/design_1/ip/design_1_test_0_0/design_1_test_0_0_sim_netlist.vhdl
-- Design      : design_1_test_0_0
-- Purpose     : This VHDL netlist is a functional simulation representation of the design and should not be modified or
--               synthesized. This netlist cannot be used for SDF annotated simulation.
-- Device      : xc7z010clg400-1
-- --------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;
entity design_1_test_0_0_uart_recv_hs is
  port (
    E : out STD_LOGIC_VECTOR ( 0 to 0 );
    D : out STD_LOGIC_VECTOR ( 7 downto 0 );
    \command_reg[0]\ : out STD_LOGIC_VECTOR ( 0 to 0 );
    \data_reg[0]\ : out STD_LOGIC_VECTOR ( 0 to 0 );
    \command_reg[7]\ : out STD_LOGIC_VECTOR ( 7 downto 0 );
    data_cmd_reg : out STD_LOGIC;
    uart_rxd : in STD_LOGIC;
    clk : in STD_LOGIC;
    \keyin[3]\ : in STD_LOGIC;
    keyin : in STD_LOGIC_VECTOR ( 0 to 0 );
    data_cmd_reg_0 : in STD_LOGIC;
    command_done_reg : in STD_LOGIC;
    Q : in STD_LOGIC_VECTOR ( 7 downto 0 )
  );
  attribute ORIG_REF_NAME : string;
  attribute ORIG_REF_NAME of design_1_test_0_0_uart_recv_hs : entity is "uart_recv_hs";
end design_1_test_0_0_uart_recv_hs;

architecture STRUCTURE of design_1_test_0_0_uart_recv_hs is
  signal \^d\ : STD_LOGIC_VECTOR ( 7 downto 0 );
  signal \clk_cnt[7]_i_2__0_n_0\ : STD_LOGIC;
  signal \clk_cnt_reg__0\ : STD_LOGIC_VECTOR ( 7 downto 0 );
  signal \command_temp[7]_i_3_n_0\ : STD_LOGIC;
  signal \data_cmd0__6\ : STD_LOGIC;
  signal \p_0_in__0\ : STD_LOGIC_VECTOR ( 7 downto 0 );
  signal p_7_in : STD_LOGIC_VECTOR ( 0 to 0 );
  signal rx_flag : STD_LOGIC;
  signal rx_flag_i_1_n_0 : STD_LOGIC;
  signal rx_flag_i_2_n_0 : STD_LOGIC;
  signal rx_flag_i_3_n_0 : STD_LOGIC;
  signal rx_flag_i_4_n_0 : STD_LOGIC;
  signal \uart_data_out[0]_i_1_n_0\ : STD_LOGIC;
  signal \uart_data_out[0]_i_2_n_0\ : STD_LOGIC;
  signal \uart_data_out[1]_i_1_n_0\ : STD_LOGIC;
  signal \uart_data_out[1]_i_2_n_0\ : STD_LOGIC;
  signal \uart_data_out[2]_i_1_n_0\ : STD_LOGIC;
  signal \uart_data_out[2]_i_2_n_0\ : STD_LOGIC;
  signal \uart_data_out[2]_i_3_n_0\ : STD_LOGIC;
  signal \uart_data_out[2]_i_4_n_0\ : STD_LOGIC;
  signal \uart_data_out[3]_i_1_n_0\ : STD_LOGIC;
  signal \uart_data_out[3]_i_2_n_0\ : STD_LOGIC;
  signal \uart_data_out[4]_i_1_n_0\ : STD_LOGIC;
  signal \uart_data_out[4]_i_2_n_0\ : STD_LOGIC;
  signal \uart_data_out[5]_i_1_n_0\ : STD_LOGIC;
  signal \uart_data_out[5]_i_2_n_0\ : STD_LOGIC;
  signal \uart_data_out[6]_i_1_n_0\ : STD_LOGIC;
  signal \uart_data_out[6]_i_2_n_0\ : STD_LOGIC;
  signal \uart_data_out[7]_i_1_n_0\ : STD_LOGIC;
  signal uart_rec : STD_LOGIC;
  signal uart_rec0_out : STD_LOGIC;
  signal uart_rec_i_1_n_0 : STD_LOGIC;
  signal uart_rec_i_2_n_0 : STD_LOGIC;
  signal uart_rec_i_3_n_0 : STD_LOGIC;
  signal uart_rec_i_4_n_0 : STD_LOGIC;
  signal uart_rec_i_5_n_0 : STD_LOGIC;
  signal uart_rec_i_6_n_0 : STD_LOGIC;
  signal uart_rxd_last1 : STD_LOGIC;
  attribute SOFT_HLUTNM : string;
  attribute SOFT_HLUTNM of \clk_cnt[0]_i_1__0\ : label is "soft_lutpair10";
  attribute SOFT_HLUTNM of \clk_cnt[1]_i_1__0\ : label is "soft_lutpair3";
  attribute SOFT_HLUTNM of \clk_cnt[2]_i_1__0\ : label is "soft_lutpair3";
  attribute SOFT_HLUTNM of \clk_cnt[3]_i_1\ : label is "soft_lutpair0";
  attribute SOFT_HLUTNM of \clk_cnt[6]_i_1\ : label is "soft_lutpair1";
  attribute SOFT_HLUTNM of \clk_cnt[7]_i_2__0\ : label is "soft_lutpair0";
  attribute SOFT_HLUTNM of \command[0]_i_1\ : label is "soft_lutpair9";
  attribute SOFT_HLUTNM of \command[1]_i_1\ : label is "soft_lutpair7";
  attribute SOFT_HLUTNM of \command[2]_i_1\ : label is "soft_lutpair8";
  attribute SOFT_HLUTNM of \command[3]_i_1\ : label is "soft_lutpair6";
  attribute SOFT_HLUTNM of \command[4]_i_1\ : label is "soft_lutpair9";
  attribute SOFT_HLUTNM of \command[5]_i_1\ : label is "soft_lutpair8";
  attribute SOFT_HLUTNM of \command[6]_i_1\ : label is "soft_lutpair7";
  attribute SOFT_HLUTNM of \command[7]_i_1\ : label is "soft_lutpair5";
  attribute SOFT_HLUTNM of \command[7]_i_2\ : label is "soft_lutpair6";
  attribute SOFT_HLUTNM of \command_temp[7]_i_1\ : label is "soft_lutpair4";
  attribute SOFT_HLUTNM of \data[7]_i_1\ : label is "soft_lutpair5";
  attribute SOFT_HLUTNM of data_cmd_i_1 : label is "soft_lutpair4";
  attribute SOFT_HLUTNM of rx_flag_i_2 : label is "soft_lutpair2";
  attribute SOFT_HLUTNM of rx_flag_i_3 : label is "soft_lutpair11";
  attribute SOFT_HLUTNM of rx_flag_i_4 : label is "soft_lutpair10";
  attribute SOFT_HLUTNM of \uart_data_out[2]_i_2\ : label is "soft_lutpair11";
  attribute SOFT_HLUTNM of \uart_data_out[2]_i_3\ : label is "soft_lutpair1";
  attribute SOFT_HLUTNM of \uart_data_out[2]_i_4\ : label is "soft_lutpair2";
begin
  D(7 downto 0) <= \^d\(7 downto 0);
\clk_cnt[0]_i_1__0\: unisim.vcomponents.LUT2
    generic map(
      INIT => X"2"
    )
        port map (
      I0 => rx_flag,
      I1 => \clk_cnt_reg__0\(0),
      O => \p_0_in__0\(0)
    );
\clk_cnt[1]_i_1__0\: unisim.vcomponents.LUT3
    generic map(
      INIT => X"48"
    )
        port map (
      I0 => \clk_cnt_reg__0\(0),
      I1 => rx_flag,
      I2 => \clk_cnt_reg__0\(1),
      O => \p_0_in__0\(1)
    );
\clk_cnt[2]_i_1__0\: unisim.vcomponents.LUT4
    generic map(
      INIT => X"7080"
    )
        port map (
      I0 => \clk_cnt_reg__0\(1),
      I1 => \clk_cnt_reg__0\(0),
      I2 => rx_flag,
      I3 => \clk_cnt_reg__0\(2),
      O => \p_0_in__0\(2)
    );
\clk_cnt[3]_i_1\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"78F00000"
    )
        port map (
      I0 => \clk_cnt_reg__0\(1),
      I1 => \clk_cnt_reg__0\(0),
      I2 => \clk_cnt_reg__0\(3),
      I3 => \clk_cnt_reg__0\(2),
      I4 => rx_flag,
      O => \p_0_in__0\(3)
    );
\clk_cnt[4]_i_1__0\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"7FFF000080000000"
    )
        port map (
      I0 => \clk_cnt_reg__0\(3),
      I1 => \clk_cnt_reg__0\(2),
      I2 => \clk_cnt_reg__0\(1),
      I3 => \clk_cnt_reg__0\(0),
      I4 => rx_flag,
      I5 => \clk_cnt_reg__0\(4),
      O => \p_0_in__0\(4)
    );
\clk_cnt[5]_i_1\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"F708FF0000000000"
    )
        port map (
      I0 => \clk_cnt_reg__0\(3),
      I1 => \clk_cnt_reg__0\(2),
      I2 => \uart_data_out[2]_i_2_n_0\,
      I3 => \clk_cnt_reg__0\(5),
      I4 => \clk_cnt_reg__0\(4),
      I5 => rx_flag,
      O => \p_0_in__0\(5)
    );
\clk_cnt[6]_i_1\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"7F008000"
    )
        port map (
      I0 => \clk_cnt_reg__0\(4),
      I1 => \clk_cnt_reg__0\(5),
      I2 => \clk_cnt[7]_i_2__0_n_0\,
      I3 => rx_flag,
      I4 => \clk_cnt_reg__0\(6),
      O => \p_0_in__0\(6)
    );
\clk_cnt[7]_i_1\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"7F800000FF000000"
    )
        port map (
      I0 => \clk_cnt[7]_i_2__0_n_0\,
      I1 => \clk_cnt_reg__0\(5),
      I2 => \clk_cnt_reg__0\(4),
      I3 => \clk_cnt_reg__0\(7),
      I4 => rx_flag,
      I5 => \clk_cnt_reg__0\(6),
      O => \p_0_in__0\(7)
    );
\clk_cnt[7]_i_2__0\: unisim.vcomponents.LUT4
    generic map(
      INIT => X"8000"
    )
        port map (
      I0 => \clk_cnt_reg__0\(3),
      I1 => \clk_cnt_reg__0\(2),
      I2 => \clk_cnt_reg__0\(1),
      I3 => \clk_cnt_reg__0\(0),
      O => \clk_cnt[7]_i_2__0_n_0\
    );
\clk_cnt_reg[0]\: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => '1',
      CLR => \keyin[3]\,
      D => \p_0_in__0\(0),
      Q => \clk_cnt_reg__0\(0)
    );
\clk_cnt_reg[1]\: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => '1',
      CLR => \keyin[3]\,
      D => \p_0_in__0\(1),
      Q => \clk_cnt_reg__0\(1)
    );
\clk_cnt_reg[2]\: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => '1',
      CLR => \keyin[3]\,
      D => \p_0_in__0\(2),
      Q => \clk_cnt_reg__0\(2)
    );
\clk_cnt_reg[3]\: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => '1',
      CLR => \keyin[3]\,
      D => \p_0_in__0\(3),
      Q => \clk_cnt_reg__0\(3)
    );
\clk_cnt_reg[4]\: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => '1',
      CLR => \keyin[3]\,
      D => \p_0_in__0\(4),
      Q => \clk_cnt_reg__0\(4)
    );
\clk_cnt_reg[5]\: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => '1',
      CLR => \keyin[3]\,
      D => \p_0_in__0\(5),
      Q => \clk_cnt_reg__0\(5)
    );
\clk_cnt_reg[6]\: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => '1',
      CLR => \keyin[3]\,
      D => \p_0_in__0\(6),
      Q => \clk_cnt_reg__0\(6)
    );
\clk_cnt_reg[7]\: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => '1',
      CLR => \keyin[3]\,
      D => \p_0_in__0\(7),
      Q => \clk_cnt_reg__0\(7)
    );
\command[0]_i_1\: unisim.vcomponents.LUT2
    generic map(
      INIT => X"8"
    )
        port map (
      I0 => uart_rec,
      I1 => Q(0),
      O => \command_reg[7]\(0)
    );
\command[1]_i_1\: unisim.vcomponents.LUT2
    generic map(
      INIT => X"8"
    )
        port map (
      I0 => uart_rec,
      I1 => Q(1),
      O => \command_reg[7]\(1)
    );
\command[2]_i_1\: unisim.vcomponents.LUT2
    generic map(
      INIT => X"8"
    )
        port map (
      I0 => uart_rec,
      I1 => Q(2),
      O => \command_reg[7]\(2)
    );
\command[3]_i_1\: unisim.vcomponents.LUT2
    generic map(
      INIT => X"8"
    )
        port map (
      I0 => uart_rec,
      I1 => Q(3),
      O => \command_reg[7]\(3)
    );
\command[4]_i_1\: unisim.vcomponents.LUT2
    generic map(
      INIT => X"8"
    )
        port map (
      I0 => uart_rec,
      I1 => Q(4),
      O => \command_reg[7]\(4)
    );
\command[5]_i_1\: unisim.vcomponents.LUT2
    generic map(
      INIT => X"8"
    )
        port map (
      I0 => uart_rec,
      I1 => Q(5),
      O => \command_reg[7]\(5)
    );
\command[6]_i_1\: unisim.vcomponents.LUT2
    generic map(
      INIT => X"8"
    )
        port map (
      I0 => uart_rec,
      I1 => Q(6),
      O => \command_reg[7]\(6)
    );
\command[7]_i_1\: unisim.vcomponents.LUT3
    generic map(
      INIT => X"B8"
    )
        port map (
      I0 => data_cmd_reg_0,
      I1 => uart_rec,
      I2 => command_done_reg,
      O => \command_reg[0]\(0)
    );
\command[7]_i_2\: unisim.vcomponents.LUT2
    generic map(
      INIT => X"8"
    )
        port map (
      I0 => uart_rec,
      I1 => Q(7),
      O => \command_reg[7]\(7)
    );
\command_temp[7]_i_1\: unisim.vcomponents.LUT4
    generic map(
      INIT => X"0800"
    )
        port map (
      I0 => uart_rec,
      I1 => keyin(0),
      I2 => data_cmd_reg_0,
      I3 => \data_cmd0__6\,
      O => E(0)
    );
\command_temp[7]_i_2\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"FFFFFFFE"
    )
        port map (
      I0 => \^d\(7),
      I1 => \^d\(6),
      I2 => \^d\(4),
      I3 => \^d\(5),
      I4 => \command_temp[7]_i_3_n_0\,
      O => \data_cmd0__6\
    );
\command_temp[7]_i_3\: unisim.vcomponents.LUT4
    generic map(
      INIT => X"FFFE"
    )
        port map (
      I0 => \^d\(1),
      I1 => \^d\(0),
      I2 => \^d\(3),
      I3 => \^d\(2),
      O => \command_temp[7]_i_3_n_0\
    );
\data[7]_i_1\: unisim.vcomponents.LUT2
    generic map(
      INIT => X"8"
    )
        port map (
      I0 => uart_rec,
      I1 => data_cmd_reg_0,
      O => \data_reg[0]\(0)
    );
data_cmd_i_1: unisim.vcomponents.LUT4
    generic map(
      INIT => X"7870"
    )
        port map (
      I0 => keyin(0),
      I1 => uart_rec,
      I2 => data_cmd_reg_0,
      I3 => \data_cmd0__6\,
      O => data_cmd_reg
    );
rx_flag_i_1: unisim.vcomponents.LUT6
    generic map(
      INIT => X"F7FFF7F700FF0000"
    )
        port map (
      I0 => rx_flag_i_2_n_0,
      I1 => rx_flag_i_3_n_0,
      I2 => rx_flag_i_4_n_0,
      I3 => uart_rxd_last1,
      I4 => p_7_in(0),
      I5 => rx_flag,
      O => rx_flag_i_1_n_0
    );
rx_flag_i_2: unisim.vcomponents.LUT4
    generic map(
      INIT => X"0800"
    )
        port map (
      I0 => \clk_cnt_reg__0\(3),
      I1 => \clk_cnt_reg__0\(2),
      I2 => \clk_cnt_reg__0\(4),
      I3 => \clk_cnt_reg__0\(6),
      O => rx_flag_i_2_n_0
    );
rx_flag_i_3: unisim.vcomponents.LUT2
    generic map(
      INIT => X"2"
    )
        port map (
      I0 => \clk_cnt_reg__0\(7),
      I1 => \clk_cnt_reg__0\(1),
      O => rx_flag_i_3_n_0
    );
rx_flag_i_4: unisim.vcomponents.LUT2
    generic map(
      INIT => X"7"
    )
        port map (
      I0 => \clk_cnt_reg__0\(0),
      I1 => \clk_cnt_reg__0\(5),
      O => rx_flag_i_4_n_0
    );
rx_flag_reg: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => '1',
      CLR => \keyin[3]\,
      D => rx_flag_i_1_n_0,
      Q => rx_flag
    );
\uart_data_out[0]_i_1\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"FFFFFFFB00000008"
    )
        port map (
      I0 => p_7_in(0),
      I1 => \uart_data_out[0]_i_2_n_0\,
      I2 => \clk_cnt_reg__0\(1),
      I3 => \clk_cnt_reg__0\(4),
      I4 => \clk_cnt_reg__0\(7),
      I5 => \^d\(0),
      O => \uart_data_out[0]_i_1_n_0\
    );
\uart_data_out[0]_i_2\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"0000008000000000"
    )
        port map (
      I0 => \clk_cnt_reg__0\(0),
      I1 => \clk_cnt_reg__0\(5),
      I2 => \clk_cnt_reg__0\(2),
      I3 => \clk_cnt_reg__0\(3),
      I4 => \clk_cnt_reg__0\(6),
      I5 => rx_flag,
      O => \uart_data_out[0]_i_2_n_0\
    );
\uart_data_out[1]_i_1\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"FBFFFFFF08000000"
    )
        port map (
      I0 => p_7_in(0),
      I1 => \uart_data_out[1]_i_2_n_0\,
      I2 => \clk_cnt_reg__0\(0),
      I3 => \clk_cnt_reg__0\(5),
      I4 => \clk_cnt_reg__0\(4),
      I5 => \^d\(1),
      O => \uart_data_out[1]_i_1_n_0\
    );
\uart_data_out[1]_i_2\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"0000200000000000"
    )
        port map (
      I0 => \clk_cnt_reg__0\(3),
      I1 => \clk_cnt_reg__0\(7),
      I2 => \clk_cnt_reg__0\(1),
      I3 => \clk_cnt_reg__0\(2),
      I4 => \clk_cnt_reg__0\(6),
      I5 => rx_flag,
      O => \uart_data_out[1]_i_2_n_0\
    );
\uart_data_out[2]_i_1\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"FFEFFFFF00200000"
    )
        port map (
      I0 => p_7_in(0),
      I1 => \uart_data_out[2]_i_2_n_0\,
      I2 => \uart_data_out[2]_i_3_n_0\,
      I3 => \clk_cnt_reg__0\(7),
      I4 => \uart_data_out[2]_i_4_n_0\,
      I5 => \^d\(2),
      O => \uart_data_out[2]_i_1_n_0\
    );
\uart_data_out[2]_i_2\: unisim.vcomponents.LUT2
    generic map(
      INIT => X"7"
    )
        port map (
      I0 => \clk_cnt_reg__0\(1),
      I1 => \clk_cnt_reg__0\(0),
      O => \uart_data_out[2]_i_2_n_0\
    );
\uart_data_out[2]_i_3\: unisim.vcomponents.LUT4
    generic map(
      INIT => X"0800"
    )
        port map (
      I0 => \clk_cnt_reg__0\(6),
      I1 => rx_flag,
      I2 => \clk_cnt_reg__0\(5),
      I3 => \clk_cnt_reg__0\(4),
      O => \uart_data_out[2]_i_3_n_0\
    );
\uart_data_out[2]_i_4\: unisim.vcomponents.LUT2
    generic map(
      INIT => X"2"
    )
        port map (
      I0 => \clk_cnt_reg__0\(2),
      I1 => \clk_cnt_reg__0\(3),
      O => \uart_data_out[2]_i_4_n_0\
    );
\uart_data_out[3]_i_1\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"FBFFFFFF08000000"
    )
        port map (
      I0 => p_7_in(0),
      I1 => \uart_data_out[3]_i_2_n_0\,
      I2 => \clk_cnt_reg__0\(0),
      I3 => \clk_cnt_reg__0\(5),
      I4 => \clk_cnt_reg__0\(4),
      I5 => \^d\(3),
      O => \uart_data_out[3]_i_1_n_0\
    );
\uart_data_out[3]_i_2\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"0001000000000000"
    )
        port map (
      I0 => \clk_cnt_reg__0\(3),
      I1 => \clk_cnt_reg__0\(2),
      I2 => \clk_cnt_reg__0\(1),
      I3 => \clk_cnt_reg__0\(7),
      I4 => \clk_cnt_reg__0\(6),
      I5 => rx_flag,
      O => \uart_data_out[3]_i_2_n_0\
    );
\uart_data_out[4]_i_1\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"FFFFBFFF00008000"
    )
        port map (
      I0 => p_7_in(0),
      I1 => \uart_data_out[4]_i_2_n_0\,
      I2 => \clk_cnt_reg__0\(0),
      I3 => \clk_cnt_reg__0\(3),
      I4 => \clk_cnt_reg__0\(2),
      I5 => \^d\(4),
      O => \uart_data_out[4]_i_1_n_0\
    );
\uart_data_out[4]_i_2\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"0000001000000000"
    )
        port map (
      I0 => \clk_cnt_reg__0\(4),
      I1 => \clk_cnt_reg__0\(5),
      I2 => \clk_cnt_reg__0\(7),
      I3 => \clk_cnt_reg__0\(1),
      I4 => \clk_cnt_reg__0\(6),
      I5 => rx_flag,
      O => \uart_data_out[4]_i_2_n_0\
    );
\uart_data_out[5]_i_1\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"FFFFBFFF00008000"
    )
        port map (
      I0 => p_7_in(0),
      I1 => \uart_data_out[5]_i_2_n_0\,
      I2 => \clk_cnt_reg__0\(1),
      I3 => \clk_cnt_reg__0\(7),
      I4 => \clk_cnt_reg__0\(4),
      I5 => \^d\(5),
      O => \uart_data_out[5]_i_1_n_0\
    );
\uart_data_out[5]_i_2\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"0000010000000000"
    )
        port map (
      I0 => \clk_cnt_reg__0\(3),
      I1 => \clk_cnt_reg__0\(2),
      I2 => \clk_cnt_reg__0\(0),
      I3 => \clk_cnt_reg__0\(5),
      I4 => \clk_cnt_reg__0\(6),
      I5 => rx_flag,
      O => \uart_data_out[5]_i_2_n_0\
    );
\uart_data_out[6]_i_1\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"FBFFFFFF08000000"
    )
        port map (
      I0 => p_7_in(0),
      I1 => \uart_data_out[6]_i_2_n_0\,
      I2 => \clk_cnt_reg__0\(2),
      I3 => \clk_cnt_reg__0\(3),
      I4 => \clk_cnt_reg__0\(7),
      I5 => \^d\(6),
      O => \uart_data_out[6]_i_1_n_0\
    );
\uart_data_out[6]_i_2\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"0000800000000000"
    )
        port map (
      I0 => \clk_cnt_reg__0\(5),
      I1 => \clk_cnt_reg__0\(4),
      I2 => \clk_cnt_reg__0\(1),
      I3 => \clk_cnt_reg__0\(0),
      I4 => \clk_cnt_reg__0\(6),
      I5 => rx_flag,
      O => \uart_data_out[6]_i_2_n_0\
    );
\uart_data_out[7]_i_1\: unisim.vcomponents.LUT3
    generic map(
      INIT => X"B8"
    )
        port map (
      I0 => p_7_in(0),
      I1 => uart_rec0_out,
      I2 => \^d\(7),
      O => \uart_data_out[7]_i_1_n_0\
    );
\uart_data_out[7]_i_2\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"0000000000200000"
    )
        port map (
      I0 => \clk_cnt_reg__0\(7),
      I1 => \clk_cnt_reg__0\(1),
      I2 => \uart_data_out[2]_i_3_n_0\,
      I3 => \clk_cnt_reg__0\(0),
      I4 => \clk_cnt_reg__0\(2),
      I5 => \clk_cnt_reg__0\(3),
      O => uart_rec0_out
    );
\uart_data_out_reg[0]\: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => '1',
      CLR => \keyin[3]\,
      D => \uart_data_out[0]_i_1_n_0\,
      Q => \^d\(0)
    );
\uart_data_out_reg[1]\: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => '1',
      CLR => \keyin[3]\,
      D => \uart_data_out[1]_i_1_n_0\,
      Q => \^d\(1)
    );
\uart_data_out_reg[2]\: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => '1',
      CLR => \keyin[3]\,
      D => \uart_data_out[2]_i_1_n_0\,
      Q => \^d\(2)
    );
\uart_data_out_reg[3]\: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => '1',
      CLR => \keyin[3]\,
      D => \uart_data_out[3]_i_1_n_0\,
      Q => \^d\(3)
    );
\uart_data_out_reg[4]\: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => '1',
      CLR => \keyin[3]\,
      D => \uart_data_out[4]_i_1_n_0\,
      Q => \^d\(4)
    );
\uart_data_out_reg[5]\: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => '1',
      CLR => \keyin[3]\,
      D => \uart_data_out[5]_i_1_n_0\,
      Q => \^d\(5)
    );
\uart_data_out_reg[6]\: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => '1',
      CLR => \keyin[3]\,
      D => \uart_data_out[6]_i_1_n_0\,
      Q => \^d\(6)
    );
\uart_data_out_reg[7]\: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => '1',
      CLR => \keyin[3]\,
      D => \uart_data_out[7]_i_1_n_0\,
      Q => \^d\(7)
    );
uart_rec_i_1: unisim.vcomponents.LUT5
    generic map(
      INIT => X"BBBF8880"
    )
        port map (
      I0 => uart_rec0_out,
      I1 => keyin(0),
      I2 => uart_rec_i_2_n_0,
      I3 => uart_rec_i_3_n_0,
      I4 => uart_rec,
      O => uart_rec_i_1_n_0
    );
uart_rec_i_2: unisim.vcomponents.LUT6
    generic map(
      INIT => X"FFFFFFFFAAAAFEAB"
    )
        port map (
      I0 => uart_rec_i_4_n_0,
      I1 => \clk_cnt_reg__0\(3),
      I2 => \clk_cnt_reg__0\(2),
      I3 => \clk_cnt_reg__0\(4),
      I4 => \clk_cnt_reg__0\(1),
      I5 => uart_rec_i_5_n_0,
      O => uart_rec_i_2_n_0
    );
uart_rec_i_3: unisim.vcomponents.LUT6
    generic map(
      INIT => X"FFFFFFFF0FC40C4C"
    )
        port map (
      I0 => \clk_cnt_reg__0\(0),
      I1 => \clk_cnt_reg__0\(2),
      I2 => \clk_cnt_reg__0\(4),
      I3 => \clk_cnt_reg__0\(3),
      I4 => \clk_cnt_reg__0\(5),
      I5 => uart_rec_i_6_n_0,
      O => uart_rec_i_3_n_0
    );
uart_rec_i_4: unisim.vcomponents.LUT6
    generic map(
      INIT => X"FF00FF00FF4545FF"
    )
        port map (
      I0 => \clk_cnt_reg__0\(3),
      I1 => \clk_cnt_reg__0\(0),
      I2 => \clk_cnt_reg__0\(5),
      I3 => \clk_cnt_reg__0\(7),
      I4 => \clk_cnt_reg__0\(6),
      I5 => \clk_cnt_reg__0\(2),
      O => uart_rec_i_4_n_0
    );
uart_rec_i_5: unisim.vcomponents.LUT6
    generic map(
      INIT => X"FF7F5D5D55555D5D"
    )
        port map (
      I0 => rx_flag,
      I1 => \clk_cnt_reg__0\(4),
      I2 => \clk_cnt_reg__0\(5),
      I3 => \clk_cnt_reg__0\(0),
      I4 => \clk_cnt_reg__0\(2),
      I5 => \clk_cnt_reg__0\(3),
      O => uart_rec_i_5_n_0
    );
uart_rec_i_6: unisim.vcomponents.LUT6
    generic map(
      INIT => X"CFCCDFDD3F3CFC3C"
    )
        port map (
      I0 => \clk_cnt_reg__0\(0),
      I1 => \clk_cnt_reg__0\(6),
      I2 => \clk_cnt_reg__0\(4),
      I3 => \clk_cnt_reg__0\(1),
      I4 => \clk_cnt_reg__0\(2),
      I5 => \clk_cnt_reg__0\(3),
      O => uart_rec_i_6_n_0
    );
uart_rec_reg: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => '1',
      D => uart_rec_i_1_n_0,
      Q => uart_rec,
      R => '0'
    );
uart_rxd_last1_reg: unisim.vcomponents.FDPE
     port map (
      C => clk,
      CE => '1',
      D => uart_rxd,
      PRE => \keyin[3]\,
      Q => uart_rxd_last1
    );
uart_rxd_last2_reg: unisim.vcomponents.FDPE
     port map (
      C => clk,
      CE => '1',
      D => uart_rxd_last1,
      PRE => \keyin[3]\,
      Q => p_7_in(0)
    );
end STRUCTURE;
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;
entity design_1_test_0_0_uart_send_hs is
  port (
    \clk_cnt_reg[1]_0\ : out STD_LOGIC;
    uart_txd : out STD_LOGIC;
    uart_send : in STD_LOGIC;
    clk : in STD_LOGIC;
    keyin : in STD_LOGIC_VECTOR ( 0 to 0 );
    \uart_data_in_reg[7]\ : in STD_LOGIC_VECTOR ( 7 downto 0 )
  );
  attribute ORIG_REF_NAME : string;
  attribute ORIG_REF_NAME of design_1_test_0_0_uart_send_hs : entity is "uart_send_hs";
end design_1_test_0_0_uart_send_hs;

architecture STRUCTURE of design_1_test_0_0_uart_send_hs is
  signal \clk_cnt[7]_i_2_n_0\ : STD_LOGIC;
  signal \^clk_cnt_reg[1]_0\ : STD_LOGIC;
  signal \clk_cnt_reg__0\ : STD_LOGIC_VECTOR ( 7 downto 0 );
  signal data1 : STD_LOGIC;
  signal data2 : STD_LOGIC;
  signal data3 : STD_LOGIC;
  signal data4 : STD_LOGIC;
  signal data5 : STD_LOGIC;
  signal data6 : STD_LOGIC;
  signal data7 : STD_LOGIC;
  signal p_0_in : STD_LOGIC_VECTOR ( 7 downto 0 );
  signal \tx_data[7]_i_1_n_0\ : STD_LOGIC;
  signal \tx_data_reg_n_0_[0]\ : STD_LOGIC;
  signal tx_flag_i_1_n_0 : STD_LOGIC;
  signal tx_flag_i_2_n_0 : STD_LOGIC;
  signal tx_flag_i_3_n_0 : STD_LOGIC;
  signal tx_flag_reg_n_0 : STD_LOGIC;
  signal uart_send_last1 : STD_LOGIC;
  signal uart_send_last2 : STD_LOGIC;
  signal \^uart_txd\ : STD_LOGIC;
  signal uart_txd_i_10_n_0 : STD_LOGIC;
  signal uart_txd_i_11_n_0 : STD_LOGIC;
  signal uart_txd_i_12_n_0 : STD_LOGIC;
  signal uart_txd_i_13_n_0 : STD_LOGIC;
  signal uart_txd_i_14_n_0 : STD_LOGIC;
  signal uart_txd_i_15_n_0 : STD_LOGIC;
  signal uart_txd_i_16_n_0 : STD_LOGIC;
  signal uart_txd_i_1_n_0 : STD_LOGIC;
  signal uart_txd_i_2_n_0 : STD_LOGIC;
  signal uart_txd_i_3_n_0 : STD_LOGIC;
  signal uart_txd_i_4_n_0 : STD_LOGIC;
  signal uart_txd_i_5_n_0 : STD_LOGIC;
  signal uart_txd_i_6_n_0 : STD_LOGIC;
  signal uart_txd_i_7_n_0 : STD_LOGIC;
  signal uart_txd_i_8_n_0 : STD_LOGIC;
  signal uart_txd_i_9_n_0 : STD_LOGIC;
  attribute SOFT_HLUTNM : string;
  attribute SOFT_HLUTNM of \clk_cnt[0]_i_1\ : label is "soft_lutpair19";
  attribute SOFT_HLUTNM of \clk_cnt[1]_i_1\ : label is "soft_lutpair19";
  attribute SOFT_HLUTNM of \clk_cnt[2]_i_1\ : label is "soft_lutpair15";
  attribute SOFT_HLUTNM of \clk_cnt[3]_i_1__0\ : label is "soft_lutpair15";
  attribute SOFT_HLUTNM of \clk_cnt[5]_i_1__0\ : label is "soft_lutpair13";
  attribute SOFT_HLUTNM of \clk_cnt[6]_i_1__0\ : label is "soft_lutpair13";
  attribute SOFT_HLUTNM of \clk_cnt[7]_i_2\ : label is "soft_lutpair17";
  attribute SOFT_HLUTNM of tx_flag_i_2 : label is "soft_lutpair17";
  attribute SOFT_HLUTNM of tx_flag_i_3 : label is "soft_lutpair12";
  attribute SOFT_HLUTNM of uart_txd_i_12 : label is "soft_lutpair14";
  attribute SOFT_HLUTNM of uart_txd_i_13 : label is "soft_lutpair18";
  attribute SOFT_HLUTNM of uart_txd_i_14 : label is "soft_lutpair16";
  attribute SOFT_HLUTNM of uart_txd_i_15 : label is "soft_lutpair18";
  attribute SOFT_HLUTNM of uart_txd_i_16 : label is "soft_lutpair16";
  attribute SOFT_HLUTNM of uart_txd_i_3 : label is "soft_lutpair12";
  attribute SOFT_HLUTNM of uart_txd_i_8 : label is "soft_lutpair14";
begin
  \clk_cnt_reg[1]_0\ <= \^clk_cnt_reg[1]_0\;
  uart_txd <= \^uart_txd\;
\clk_cnt[0]_i_1\: unisim.vcomponents.LUT2
    generic map(
      INIT => X"2"
    )
        port map (
      I0 => tx_flag_reg_n_0,
      I1 => \clk_cnt_reg__0\(0),
      O => p_0_in(0)
    );
\clk_cnt[1]_i_1\: unisim.vcomponents.LUT3
    generic map(
      INIT => X"48"
    )
        port map (
      I0 => \clk_cnt_reg__0\(0),
      I1 => tx_flag_reg_n_0,
      I2 => \clk_cnt_reg__0\(1),
      O => p_0_in(1)
    );
\clk_cnt[2]_i_1\: unisim.vcomponents.LUT4
    generic map(
      INIT => X"7080"
    )
        port map (
      I0 => \clk_cnt_reg__0\(1),
      I1 => \clk_cnt_reg__0\(0),
      I2 => tx_flag_reg_n_0,
      I3 => \clk_cnt_reg__0\(2),
      O => p_0_in(2)
    );
\clk_cnt[3]_i_1__0\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"7F008000"
    )
        port map (
      I0 => \clk_cnt_reg__0\(0),
      I1 => \clk_cnt_reg__0\(1),
      I2 => \clk_cnt_reg__0\(2),
      I3 => tx_flag_reg_n_0,
      I4 => \clk_cnt_reg__0\(3),
      O => p_0_in(3)
    );
\clk_cnt[4]_i_1\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"7FFF000080000000"
    )
        port map (
      I0 => \clk_cnt_reg__0\(2),
      I1 => \clk_cnt_reg__0\(1),
      I2 => \clk_cnt_reg__0\(0),
      I3 => \clk_cnt_reg__0\(3),
      I4 => tx_flag_reg_n_0,
      I5 => \clk_cnt_reg__0\(4),
      O => p_0_in(4)
    );
\clk_cnt[5]_i_1__0\: unisim.vcomponents.LUT4
    generic map(
      INIT => X"D020"
    )
        port map (
      I0 => \clk_cnt_reg__0\(4),
      I1 => \clk_cnt[7]_i_2_n_0\,
      I2 => tx_flag_reg_n_0,
      I3 => \clk_cnt_reg__0\(5),
      O => p_0_in(5)
    );
\clk_cnt[6]_i_1__0\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"DF002000"
    )
        port map (
      I0 => \clk_cnt_reg__0\(5),
      I1 => \clk_cnt[7]_i_2_n_0\,
      I2 => \clk_cnt_reg__0\(4),
      I3 => tx_flag_reg_n_0,
      I4 => \clk_cnt_reg__0\(6),
      O => p_0_in(6)
    );
\clk_cnt[7]_i_1__0\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"DFFF000020000000"
    )
        port map (
      I0 => \clk_cnt_reg__0\(4),
      I1 => \clk_cnt[7]_i_2_n_0\,
      I2 => \clk_cnt_reg__0\(5),
      I3 => \clk_cnt_reg__0\(6),
      I4 => tx_flag_reg_n_0,
      I5 => \clk_cnt_reg__0\(7),
      O => p_0_in(7)
    );
\clk_cnt[7]_i_2\: unisim.vcomponents.LUT4
    generic map(
      INIT => X"7FFF"
    )
        port map (
      I0 => \clk_cnt_reg__0\(2),
      I1 => \clk_cnt_reg__0\(1),
      I2 => \clk_cnt_reg__0\(0),
      I3 => \clk_cnt_reg__0\(3),
      O => \clk_cnt[7]_i_2_n_0\
    );
\clk_cnt_reg[0]\: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => '1',
      CLR => \^clk_cnt_reg[1]_0\,
      D => p_0_in(0),
      Q => \clk_cnt_reg__0\(0)
    );
\clk_cnt_reg[1]\: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => '1',
      CLR => \^clk_cnt_reg[1]_0\,
      D => p_0_in(1),
      Q => \clk_cnt_reg__0\(1)
    );
\clk_cnt_reg[2]\: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => '1',
      CLR => \^clk_cnt_reg[1]_0\,
      D => p_0_in(2),
      Q => \clk_cnt_reg__0\(2)
    );
\clk_cnt_reg[3]\: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => '1',
      CLR => \^clk_cnt_reg[1]_0\,
      D => p_0_in(3),
      Q => \clk_cnt_reg__0\(3)
    );
\clk_cnt_reg[4]\: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => '1',
      CLR => \^clk_cnt_reg[1]_0\,
      D => p_0_in(4),
      Q => \clk_cnt_reg__0\(4)
    );
\clk_cnt_reg[5]\: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => '1',
      CLR => \^clk_cnt_reg[1]_0\,
      D => p_0_in(5),
      Q => \clk_cnt_reg__0\(5)
    );
\clk_cnt_reg[6]\: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => '1',
      CLR => \^clk_cnt_reg[1]_0\,
      D => p_0_in(6),
      Q => \clk_cnt_reg__0\(6)
    );
\clk_cnt_reg[7]\: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => '1',
      CLR => \^clk_cnt_reg[1]_0\,
      D => p_0_in(7),
      Q => \clk_cnt_reg__0\(7)
    );
flg_i_2: unisim.vcomponents.LUT1
    generic map(
      INIT => X"1"
    )
        port map (
      I0 => keyin(0),
      O => \^clk_cnt_reg[1]_0\
    );
\tx_data[7]_i_1\: unisim.vcomponents.LUT2
    generic map(
      INIT => X"2"
    )
        port map (
      I0 => uart_send_last2,
      I1 => uart_send_last1,
      O => \tx_data[7]_i_1_n_0\
    );
\tx_data_reg[0]\: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => \tx_data[7]_i_1_n_0\,
      CLR => \^clk_cnt_reg[1]_0\,
      D => \uart_data_in_reg[7]\(0),
      Q => \tx_data_reg_n_0_[0]\
    );
\tx_data_reg[1]\: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => \tx_data[7]_i_1_n_0\,
      CLR => \^clk_cnt_reg[1]_0\,
      D => \uart_data_in_reg[7]\(1),
      Q => data1
    );
\tx_data_reg[2]\: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => \tx_data[7]_i_1_n_0\,
      CLR => \^clk_cnt_reg[1]_0\,
      D => \uart_data_in_reg[7]\(2),
      Q => data2
    );
\tx_data_reg[3]\: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => \tx_data[7]_i_1_n_0\,
      CLR => \^clk_cnt_reg[1]_0\,
      D => \uart_data_in_reg[7]\(3),
      Q => data3
    );
\tx_data_reg[4]\: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => \tx_data[7]_i_1_n_0\,
      CLR => \^clk_cnt_reg[1]_0\,
      D => \uart_data_in_reg[7]\(4),
      Q => data4
    );
\tx_data_reg[5]\: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => \tx_data[7]_i_1_n_0\,
      CLR => \^clk_cnt_reg[1]_0\,
      D => \uart_data_in_reg[7]\(5),
      Q => data5
    );
\tx_data_reg[6]\: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => \tx_data[7]_i_1_n_0\,
      CLR => \^clk_cnt_reg[1]_0\,
      D => \uart_data_in_reg[7]\(6),
      Q => data6
    );
\tx_data_reg[7]\: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => \tx_data[7]_i_1_n_0\,
      CLR => \^clk_cnt_reg[1]_0\,
      D => \uart_data_in_reg[7]\(7),
      Q => data7
    );
tx_flag_i_1: unisim.vcomponents.LUT6
    generic map(
      INIT => X"FFFF7FFFFFFF0000"
    )
        port map (
      I0 => tx_flag_i_2_n_0,
      I1 => tx_flag_i_3_n_0,
      I2 => \clk_cnt_reg__0\(6),
      I3 => \clk_cnt_reg__0\(5),
      I4 => \tx_data[7]_i_1_n_0\,
      I5 => tx_flag_reg_n_0,
      O => tx_flag_i_1_n_0
    );
tx_flag_i_2: unisim.vcomponents.LUT4
    generic map(
      INIT => X"4000"
    )
        port map (
      I0 => \clk_cnt_reg__0\(1),
      I1 => \clk_cnt_reg__0\(0),
      I2 => \clk_cnt_reg__0\(3),
      I3 => \clk_cnt_reg__0\(2),
      O => tx_flag_i_2_n_0
    );
tx_flag_i_3: unisim.vcomponents.LUT2
    generic map(
      INIT => X"2"
    )
        port map (
      I0 => \clk_cnt_reg__0\(7),
      I1 => \clk_cnt_reg__0\(4),
      O => tx_flag_i_3_n_0
    );
tx_flag_reg: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => '1',
      CLR => \^clk_cnt_reg[1]_0\,
      D => tx_flag_i_1_n_0,
      Q => tx_flag_reg_n_0
    );
uart_send_last1_reg: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => '1',
      CLR => \^clk_cnt_reg[1]_0\,
      D => uart_send,
      Q => uart_send_last1
    );
uart_send_last2_reg: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => '1',
      CLR => \^clk_cnt_reg[1]_0\,
      D => uart_send_last1,
      Q => uart_send_last2
    );
uart_txd_i_1: unisim.vcomponents.LUT6
    generic map(
      INIT => X"EEEEEEEFEEEEEEE0"
    )
        port map (
      I0 => uart_txd_i_2_n_0,
      I1 => uart_txd_i_3_n_0,
      I2 => uart_txd_i_4_n_0,
      I3 => uart_txd_i_5_n_0,
      I4 => uart_txd_i_6_n_0,
      I5 => \^uart_txd\,
      O => uart_txd_i_1_n_0
    );
uart_txd_i_10: unisim.vcomponents.LUT6
    generic map(
      INIT => X"0000B800FFFFFFFF"
    )
        port map (
      I0 => data4,
      I1 => \clk_cnt_reg__0\(4),
      I2 => data3,
      I3 => \clk_cnt_reg__0\(2),
      I4 => \clk_cnt_reg__0\(7),
      I5 => tx_flag_reg_n_0,
      O => uart_txd_i_10_n_0
    );
uart_txd_i_11: unisim.vcomponents.LUT4
    generic map(
      INIT => X"0008"
    )
        port map (
      I0 => \clk_cnt_reg__0\(3),
      I1 => \clk_cnt_reg__0\(6),
      I2 => \clk_cnt_reg__0\(5),
      I3 => \clk_cnt_reg__0\(4),
      O => uart_txd_i_11_n_0
    );
uart_txd_i_12: unisim.vcomponents.LUT5
    generic map(
      INIT => X"00020000"
    )
        port map (
      I0 => \clk_cnt_reg__0\(6),
      I1 => \clk_cnt_reg__0\(1),
      I2 => \clk_cnt_reg__0\(2),
      I3 => \clk_cnt_reg__0\(4),
      I4 => \clk_cnt_reg__0\(7),
      O => uart_txd_i_12_n_0
    );
uart_txd_i_13: unisim.vcomponents.LUT3
    generic map(
      INIT => X"24"
    )
        port map (
      I0 => \clk_cnt_reg__0\(2),
      I1 => \clk_cnt_reg__0\(5),
      I2 => \clk_cnt_reg__0\(7),
      O => uart_txd_i_13_n_0
    );
uart_txd_i_14: unisim.vcomponents.LUT5
    generic map(
      INIT => X"00040000"
    )
        port map (
      I0 => \clk_cnt_reg__0\(6),
      I1 => \clk_cnt_reg__0\(1),
      I2 => \clk_cnt_reg__0\(3),
      I3 => \clk_cnt_reg__0\(0),
      I4 => \clk_cnt_reg__0\(4),
      O => uart_txd_i_14_n_0
    );
uart_txd_i_15: unisim.vcomponents.LUT4
    generic map(
      INIT => X"4001"
    )
        port map (
      I0 => \clk_cnt_reg__0\(7),
      I1 => \clk_cnt_reg__0\(2),
      I2 => \clk_cnt_reg__0\(5),
      I3 => \clk_cnt_reg__0\(6),
      O => uart_txd_i_15_n_0
    );
uart_txd_i_16: unisim.vcomponents.LUT4
    generic map(
      INIT => X"0081"
    )
        port map (
      I0 => \clk_cnt_reg__0\(4),
      I1 => \clk_cnt_reg__0\(3),
      I2 => \clk_cnt_reg__0\(0),
      I3 => \clk_cnt_reg__0\(1),
      O => uart_txd_i_16_n_0
    );
uart_txd_i_2: unisim.vcomponents.LUT6
    generic map(
      INIT => X"FFFFFFFFBAAAAAAA"
    )
        port map (
      I0 => uart_txd_i_7_n_0,
      I1 => \clk_cnt_reg__0\(4),
      I2 => data2,
      I3 => \clk_cnt_reg__0\(1),
      I4 => uart_txd_i_8_n_0,
      I5 => uart_txd_i_9_n_0,
      O => uart_txd_i_2_n_0
    );
uart_txd_i_3: unisim.vcomponents.LUT5
    generic map(
      INIT => X"AEAAAAAA"
    )
        port map (
      I0 => uart_txd_i_10_n_0,
      I1 => \clk_cnt_reg__0\(7),
      I2 => \clk_cnt_reg__0\(4),
      I3 => \clk_cnt_reg__0\(2),
      I4 => data6,
      O => uart_txd_i_3_n_0
    );
uart_txd_i_4: unisim.vcomponents.LUT6
    generic map(
      INIT => X"02000000FFFFFFFF"
    )
        port map (
      I0 => uart_txd_i_11_n_0,
      I1 => \clk_cnt_reg__0\(2),
      I2 => \clk_cnt_reg__0\(7),
      I3 => \clk_cnt_reg__0\(0),
      I4 => \clk_cnt_reg__0\(1),
      I5 => tx_flag_reg_n_0,
      O => uart_txd_i_4_n_0
    );
uart_txd_i_5: unisim.vcomponents.LUT6
    generic map(
      INIT => X"FFFF240024002400"
    )
        port map (
      I0 => \clk_cnt_reg__0\(0),
      I1 => \clk_cnt_reg__0\(3),
      I2 => \clk_cnt_reg__0\(5),
      I3 => uart_txd_i_12_n_0,
      I4 => uart_txd_i_13_n_0,
      I5 => uart_txd_i_14_n_0,
      O => uart_txd_i_5_n_0
    );
uart_txd_i_6: unisim.vcomponents.LUT6
    generic map(
      INIT => X"FFFF100010001000"
    )
        port map (
      I0 => \clk_cnt[7]_i_2_n_0\,
      I1 => \clk_cnt_reg__0\(6),
      I2 => \clk_cnt_reg__0\(5),
      I3 => tx_flag_i_3_n_0,
      I4 => uart_txd_i_15_n_0,
      I5 => uart_txd_i_16_n_0,
      O => uart_txd_i_6_n_0
    );
uart_txd_i_7: unisim.vcomponents.LUT6
    generic map(
      INIT => X"808080808C8C8C80"
    )
        port map (
      I0 => data5,
      I1 => \clk_cnt_reg__0\(7),
      I2 => \clk_cnt_reg__0\(4),
      I3 => \clk_cnt_reg__0\(5),
      I4 => data7,
      I5 => \clk_cnt_reg__0\(2),
      O => uart_txd_i_7_n_0
    );
uart_txd_i_8: unisim.vcomponents.LUT2
    generic map(
      INIT => X"1"
    )
        port map (
      I0 => \clk_cnt_reg__0\(7),
      I1 => \clk_cnt_reg__0\(2),
      O => uart_txd_i_8_n_0
    );
uart_txd_i_9: unisim.vcomponents.LUT6
    generic map(
      INIT => X"0000000000008A80"
    )
        port map (
      I0 => \clk_cnt_reg__0\(4),
      I1 => data1,
      I2 => \clk_cnt_reg__0\(1),
      I3 => \tx_data_reg_n_0_[0]\,
      I4 => \clk_cnt_reg__0\(2),
      I5 => \clk_cnt_reg__0\(7),
      O => uart_txd_i_9_n_0
    );
uart_txd_reg: unisim.vcomponents.FDPE
     port map (
      C => clk,
      CE => '1',
      D => uart_txd_i_1_n_0,
      PRE => \^clk_cnt_reg[1]_0\,
      Q => \^uart_txd\
    );
end STRUCTURE;
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;
entity design_1_test_0_0_uart_hs is
  port (
    \clk_cnt_reg[1]\ : out STD_LOGIC;
    uart_txd : out STD_LOGIC;
    E : out STD_LOGIC_VECTOR ( 0 to 0 );
    D : out STD_LOGIC_VECTOR ( 7 downto 0 );
    \command_reg[0]\ : out STD_LOGIC_VECTOR ( 0 to 0 );
    \data_reg[0]\ : out STD_LOGIC_VECTOR ( 0 to 0 );
    \command_reg[7]\ : out STD_LOGIC_VECTOR ( 7 downto 0 );
    data_cmd_reg : out STD_LOGIC;
    uart_send : in STD_LOGIC;
    clk : in STD_LOGIC;
    uart_rxd : in STD_LOGIC;
    keyin : in STD_LOGIC_VECTOR ( 0 to 0 );
    data_cmd_reg_0 : in STD_LOGIC;
    command_done_reg : in STD_LOGIC;
    Q : in STD_LOGIC_VECTOR ( 7 downto 0 );
    \uart_data_in_reg[7]\ : in STD_LOGIC_VECTOR ( 7 downto 0 )
  );
  attribute ORIG_REF_NAME : string;
  attribute ORIG_REF_NAME of design_1_test_0_0_uart_hs : entity is "uart_hs";
end design_1_test_0_0_uart_hs;

architecture STRUCTURE of design_1_test_0_0_uart_hs is
  signal \^clk_cnt_reg[1]\ : STD_LOGIC;
begin
  \clk_cnt_reg[1]\ <= \^clk_cnt_reg[1]\;
ins_uart_recv_hs: entity work.design_1_test_0_0_uart_recv_hs
     port map (
      D(7 downto 0) => D(7 downto 0),
      E(0) => E(0),
      Q(7 downto 0) => Q(7 downto 0),
      clk => clk,
      command_done_reg => command_done_reg,
      \command_reg[0]\(0) => \command_reg[0]\(0),
      \command_reg[7]\(7 downto 0) => \command_reg[7]\(7 downto 0),
      data_cmd_reg => data_cmd_reg,
      data_cmd_reg_0 => data_cmd_reg_0,
      \data_reg[0]\(0) => \data_reg[0]\(0),
      keyin(0) => keyin(0),
      \keyin[3]\ => \^clk_cnt_reg[1]\,
      uart_rxd => uart_rxd
    );
ins_uart_send_hs: entity work.design_1_test_0_0_uart_send_hs
     port map (
      clk => clk,
      \clk_cnt_reg[1]_0\ => \^clk_cnt_reg[1]\,
      keyin(0) => keyin(0),
      \uart_data_in_reg[7]\(7 downto 0) => \uart_data_in_reg[7]\(7 downto 0),
      uart_send => uart_send,
      uart_txd => uart_txd
    );
end STRUCTURE;
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;
entity design_1_test_0_0_test is
  port (
    led : out STD_LOGIC_VECTOR ( 1 downto 0 );
    M00_AXI_araddr : out STD_LOGIC_VECTOR ( 31 downto 0 );
    M00_AXI_wdata : out STD_LOGIC_VECTOR ( 31 downto 0 );
    M00_AXI_wstrb : out STD_LOGIC_VECTOR ( 3 downto 0 );
    debug_0_sp_1 : out STD_LOGIC;
    \debug[4]\ : out STD_LOGIC;
    \debug[6]\ : out STD_LOGIC;
    uart_txd : out STD_LOGIC;
    M00_AXI_rready : out STD_LOGIC;
    clk : in STD_LOGIC;
    M00_AXI_rdata : in STD_LOGIC_VECTOR ( 31 downto 0 );
    uart_rxd : in STD_LOGIC;
    debug : in STD_LOGIC_VECTOR ( 3 downto 0 );
    keyin : in STD_LOGIC_VECTOR ( 3 downto 0 );
    M00_AXI_bvalid : in STD_LOGIC
  );
  attribute ORIG_REF_NAME : string;
  attribute ORIG_REF_NAME of design_1_test_0_0_test : entity is "test";
end design_1_test_0_0_test;

architecture STRUCTURE of design_1_test_0_0_test is
  signal M00_AXI_arvalid_i_1_n_0 : STD_LOGIC;
  signal M00_AXI_awvalid_i_1_n_0 : STD_LOGIC;
  signal M00_AXI_bready_i_1_n_0 : STD_LOGIC;
  signal M00_AXI_bready_i_2_n_0 : STD_LOGIC;
  signal M00_AXI_bready_i_3_n_0 : STD_LOGIC;
  signal M00_AXI_bready_i_5_n_0 : STD_LOGIC;
  signal \^m00_axi_rready\ : STD_LOGIC;
  signal M00_AXI_rready_i_1_n_0 : STD_LOGIC;
  signal M00_AXI_rready_i_2_n_0 : STD_LOGIC;
  signal \M00_AXI_wdata[15]_i_1_n_0\ : STD_LOGIC;
  signal \M00_AXI_wdata[15]_i_2_n_0\ : STD_LOGIC;
  signal \M00_AXI_wdata[23]_i_1_n_0\ : STD_LOGIC;
  signal \M00_AXI_wdata[23]_i_2_n_0\ : STD_LOGIC;
  signal \M00_AXI_wdata[31]_i_1_n_0\ : STD_LOGIC;
  signal \M00_AXI_wdata[31]_i_2_n_0\ : STD_LOGIC;
  signal \M00_AXI_wdata[31]_i_3_n_0\ : STD_LOGIC;
  signal \M00_AXI_wdata[31]_i_4_n_0\ : STD_LOGIC;
  signal \M00_AXI_wdata[7]_i_1_n_0\ : STD_LOGIC;
  signal \M00_AXI_wdata[7]_i_2_n_0\ : STD_LOGIC;
  signal \M00_AXI_wstrb[3]_i_1_n_0\ : STD_LOGIC;
  signal \M00_AXI_wstrb[3]_i_2_n_0\ : STD_LOGIC;
  signal M00_AXI_wvalid_i_1_n_0 : STD_LOGIC;
  signal \addr[15]_i_1_n_0\ : STD_LOGIC;
  signal \addr[15]_i_2_n_0\ : STD_LOGIC;
  signal \addr[23]_i_1_n_0\ : STD_LOGIC;
  signal \addr[23]_i_2_n_0\ : STD_LOGIC;
  signal \addr[23]_i_3_n_0\ : STD_LOGIC;
  signal \addr[23]_i_4_n_0\ : STD_LOGIC;
  signal \addr[31]_i_1_n_0\ : STD_LOGIC;
  signal \addr[31]_i_2_n_0\ : STD_LOGIC;
  signal \addr[31]_i_3_n_0\ : STD_LOGIC;
  signal \addr[31]_i_4_n_0\ : STD_LOGIC;
  signal \addr[31]_i_5_n_0\ : STD_LOGIC;
  signal \addr[31]_i_6_n_0\ : STD_LOGIC;
  signal \addr[7]_i_1_n_0\ : STD_LOGIC;
  signal \addr[7]_i_2_n_0\ : STD_LOGIC;
  signal command : STD_LOGIC_VECTOR ( 7 downto 0 );
  signal command_done221_out : STD_LOGIC;
  signal \command_done2__1\ : STD_LOGIC;
  signal command_done_i_1_n_0 : STD_LOGIC;
  signal command_done_i_2_n_0 : STD_LOGIC;
  signal command_done_i_3_n_0 : STD_LOGIC;
  signal command_done_i_4_n_0 : STD_LOGIC;
  signal command_done_i_5_n_0 : STD_LOGIC;
  signal command_done_reg_n_0 : STD_LOGIC;
  signal command_temp : STD_LOGIC_VECTOR ( 7 downto 0 );
  signal command_temp_1 : STD_LOGIC;
  signal data : STD_LOGIC;
  signal data0 : STD_LOGIC_VECTOR ( 31 downto 1 );
  signal data_buff : STD_LOGIC;
  signal \data_buff_reg_n_0_[0]\ : STD_LOGIC;
  signal \data_buff_reg_n_0_[10]\ : STD_LOGIC;
  signal \data_buff_reg_n_0_[11]\ : STD_LOGIC;
  signal \data_buff_reg_n_0_[12]\ : STD_LOGIC;
  signal \data_buff_reg_n_0_[13]\ : STD_LOGIC;
  signal \data_buff_reg_n_0_[14]\ : STD_LOGIC;
  signal \data_buff_reg_n_0_[15]\ : STD_LOGIC;
  signal \data_buff_reg_n_0_[16]\ : STD_LOGIC;
  signal \data_buff_reg_n_0_[17]\ : STD_LOGIC;
  signal \data_buff_reg_n_0_[18]\ : STD_LOGIC;
  signal \data_buff_reg_n_0_[19]\ : STD_LOGIC;
  signal \data_buff_reg_n_0_[1]\ : STD_LOGIC;
  signal \data_buff_reg_n_0_[20]\ : STD_LOGIC;
  signal \data_buff_reg_n_0_[21]\ : STD_LOGIC;
  signal \data_buff_reg_n_0_[22]\ : STD_LOGIC;
  signal \data_buff_reg_n_0_[23]\ : STD_LOGIC;
  signal \data_buff_reg_n_0_[24]\ : STD_LOGIC;
  signal \data_buff_reg_n_0_[25]\ : STD_LOGIC;
  signal \data_buff_reg_n_0_[26]\ : STD_LOGIC;
  signal \data_buff_reg_n_0_[27]\ : STD_LOGIC;
  signal \data_buff_reg_n_0_[28]\ : STD_LOGIC;
  signal \data_buff_reg_n_0_[29]\ : STD_LOGIC;
  signal \data_buff_reg_n_0_[2]\ : STD_LOGIC;
  signal \data_buff_reg_n_0_[30]\ : STD_LOGIC;
  signal \data_buff_reg_n_0_[31]\ : STD_LOGIC;
  signal \data_buff_reg_n_0_[3]\ : STD_LOGIC;
  signal \data_buff_reg_n_0_[4]\ : STD_LOGIC;
  signal \data_buff_reg_n_0_[5]\ : STD_LOGIC;
  signal \data_buff_reg_n_0_[6]\ : STD_LOGIC;
  signal \data_buff_reg_n_0_[7]\ : STD_LOGIC;
  signal \data_buff_reg_n_0_[8]\ : STD_LOGIC;
  signal \data_buff_reg_n_0_[9]\ : STD_LOGIC;
  signal data_cmd_reg_n_0 : STD_LOGIC;
  signal \data_reg_n_0_[0]\ : STD_LOGIC;
  signal \data_reg_n_0_[1]\ : STD_LOGIC;
  signal \data_reg_n_0_[2]\ : STD_LOGIC;
  signal \data_reg_n_0_[3]\ : STD_LOGIC;
  signal \data_reg_n_0_[4]\ : STD_LOGIC;
  signal \data_reg_n_0_[5]\ : STD_LOGIC;
  signal \data_reg_n_0_[6]\ : STD_LOGIC;
  signal \data_reg_n_0_[7]\ : STD_LOGIC;
  signal \^debug[4]\ : STD_LOGIC;
  signal \^debug[6]\ : STD_LOGIC;
  signal debug_0_sn_1 : STD_LOGIC;
  signal \debug_readmem_step[0]_i_1_n_0\ : STD_LOGIC;
  signal \debug_readmem_step[0]_i_2_n_0\ : STD_LOGIC;
  signal \debug_readmem_step[0]_i_3_n_0\ : STD_LOGIC;
  signal \debug_readmem_step[1]_i_1_n_0\ : STD_LOGIC;
  signal \debug_readmem_step[1]_i_2_n_0\ : STD_LOGIC;
  signal \debug_readmem_step[1]_i_3_n_0\ : STD_LOGIC;
  signal \debug_readmem_step[1]_i_4_n_0\ : STD_LOGIC;
  signal \debug_readmem_step[1]_i_5_n_0\ : STD_LOGIC;
  signal \debug_readmem_step[1]_i_6_n_0\ : STD_LOGIC;
  signal \debug_readmem_step[1]_i_7_n_0\ : STD_LOGIC;
  signal \debug_readmem_step[1]_i_8_n_0\ : STD_LOGIC;
  signal \debug_readmem_step_reg_n_0_[0]\ : STD_LOGIC;
  signal \debug_readmem_step_reg_n_0_[1]\ : STD_LOGIC;
  signal flg_i_10_n_0 : STD_LOGIC;
  signal flg_i_1_n_0 : STD_LOGIC;
  signal flg_i_3_n_0 : STD_LOGIC;
  signal flg_i_4_n_0 : STD_LOGIC;
  signal flg_i_5_n_0 : STD_LOGIC;
  signal flg_i_6_n_0 : STD_LOGIC;
  signal flg_i_7_n_0 : STD_LOGIC;
  signal flg_i_8_n_0 : STD_LOGIC;
  signal flg_i_9_n_0 : STD_LOGIC;
  signal \^led\ : STD_LOGIC_VECTOR ( 1 downto 0 );
  signal timer33 : STD_LOGIC_VECTOR ( 31 downto 0 );
  signal timer33_2 : STD_LOGIC_VECTOR ( 31 downto 0 );
  signal \timer33_reg[12]_i_2_n_0\ : STD_LOGIC;
  signal \timer33_reg[12]_i_2_n_1\ : STD_LOGIC;
  signal \timer33_reg[12]_i_2_n_2\ : STD_LOGIC;
  signal \timer33_reg[12]_i_2_n_3\ : STD_LOGIC;
  signal \timer33_reg[16]_i_2_n_0\ : STD_LOGIC;
  signal \timer33_reg[16]_i_2_n_1\ : STD_LOGIC;
  signal \timer33_reg[16]_i_2_n_2\ : STD_LOGIC;
  signal \timer33_reg[16]_i_2_n_3\ : STD_LOGIC;
  signal \timer33_reg[20]_i_2_n_0\ : STD_LOGIC;
  signal \timer33_reg[20]_i_2_n_1\ : STD_LOGIC;
  signal \timer33_reg[20]_i_2_n_2\ : STD_LOGIC;
  signal \timer33_reg[20]_i_2_n_3\ : STD_LOGIC;
  signal \timer33_reg[24]_i_2_n_0\ : STD_LOGIC;
  signal \timer33_reg[24]_i_2_n_1\ : STD_LOGIC;
  signal \timer33_reg[24]_i_2_n_2\ : STD_LOGIC;
  signal \timer33_reg[24]_i_2_n_3\ : STD_LOGIC;
  signal \timer33_reg[28]_i_2_n_0\ : STD_LOGIC;
  signal \timer33_reg[28]_i_2_n_1\ : STD_LOGIC;
  signal \timer33_reg[28]_i_2_n_2\ : STD_LOGIC;
  signal \timer33_reg[28]_i_2_n_3\ : STD_LOGIC;
  signal \timer33_reg[31]_i_2_n_2\ : STD_LOGIC;
  signal \timer33_reg[31]_i_2_n_3\ : STD_LOGIC;
  signal \timer33_reg[4]_i_2_n_0\ : STD_LOGIC;
  signal \timer33_reg[4]_i_2_n_1\ : STD_LOGIC;
  signal \timer33_reg[4]_i_2_n_2\ : STD_LOGIC;
  signal \timer33_reg[4]_i_2_n_3\ : STD_LOGIC;
  signal \timer33_reg[8]_i_2_n_0\ : STD_LOGIC;
  signal \timer33_reg[8]_i_2_n_1\ : STD_LOGIC;
  signal \timer33_reg[8]_i_2_n_2\ : STD_LOGIC;
  signal \timer33_reg[8]_i_2_n_3\ : STD_LOGIC;
  signal uart_data_in : STD_LOGIC_VECTOR ( 7 downto 0 );
  signal \uart_data_in[0]_i_1_n_0\ : STD_LOGIC;
  signal \uart_data_in[0]_i_2_n_0\ : STD_LOGIC;
  signal \uart_data_in[1]_i_1_n_0\ : STD_LOGIC;
  signal \uart_data_in[1]_i_2_n_0\ : STD_LOGIC;
  signal \uart_data_in[2]_i_1_n_0\ : STD_LOGIC;
  signal \uart_data_in[2]_i_2_n_0\ : STD_LOGIC;
  signal \uart_data_in[3]_i_1_n_0\ : STD_LOGIC;
  signal \uart_data_in[3]_i_2_n_0\ : STD_LOGIC;
  signal \uart_data_in[4]_i_1_n_0\ : STD_LOGIC;
  signal \uart_data_in[4]_i_2_n_0\ : STD_LOGIC;
  signal \uart_data_in[5]_i_1_n_0\ : STD_LOGIC;
  signal \uart_data_in[5]_i_2_n_0\ : STD_LOGIC;
  signal \uart_data_in[6]_i_1_n_0\ : STD_LOGIC;
  signal \uart_data_in[6]_i_2_n_0\ : STD_LOGIC;
  signal \uart_data_in[7]_i_2_n_0\ : STD_LOGIC;
  signal \uart_data_in[7]_i_3_n_0\ : STD_LOGIC;
  signal \uart_data_in[7]_i_4_n_0\ : STD_LOGIC;
  signal \uart_data_in[7]_i_5_n_0\ : STD_LOGIC;
  signal \uart_data_in[7]_i_6_n_0\ : STD_LOGIC;
  signal \uart_data_in[7]_i_7_n_0\ : STD_LOGIC;
  signal uart_data_in_0 : STD_LOGIC;
  signal uart_data_out : STD_LOGIC_VECTOR ( 7 downto 0 );
  signal uart_hs_inst_n_0 : STD_LOGIC;
  signal uart_hs_inst_n_11 : STD_LOGIC;
  signal uart_hs_inst_n_13 : STD_LOGIC;
  signal uart_hs_inst_n_14 : STD_LOGIC;
  signal uart_hs_inst_n_15 : STD_LOGIC;
  signal uart_hs_inst_n_16 : STD_LOGIC;
  signal uart_hs_inst_n_17 : STD_LOGIC;
  signal uart_hs_inst_n_18 : STD_LOGIC;
  signal uart_hs_inst_n_19 : STD_LOGIC;
  signal uart_hs_inst_n_20 : STD_LOGIC;
  signal uart_hs_inst_n_21 : STD_LOGIC;
  signal uart_send : STD_LOGIC;
  signal uart_send_i_1_n_0 : STD_LOGIC;
  signal \NLW_timer33_reg[31]_i_2_CO_UNCONNECTED\ : STD_LOGIC_VECTOR ( 3 downto 2 );
  signal \NLW_timer33_reg[31]_i_2_O_UNCONNECTED\ : STD_LOGIC_VECTOR ( 3 to 3 );
  attribute X_INTERFACE_INFO : string;
  attribute X_INTERFACE_INFO of M00_AXI_arvalid_reg : label is "xilinx.com:interface:aximm:1.0 M00_AXI ARVALID";
  attribute X_INTERFACE_INFO of M00_AXI_awvalid_reg : label is "xilinx.com:interface:aximm:1.0 M00_AXI AWVALID";
  attribute SOFT_HLUTNM : string;
  attribute SOFT_HLUTNM of M00_AXI_bready_i_3 : label is "soft_lutpair24";
  attribute SOFT_HLUTNM of M00_AXI_bready_i_4 : label is "soft_lutpair24";
  attribute SOFT_HLUTNM of M00_AXI_bready_i_5 : label is "soft_lutpair27";
  attribute X_INTERFACE_INFO of M00_AXI_bready_reg : label is "xilinx.com:interface:aximm:1.0 M00_AXI BREADY";
  attribute SOFT_HLUTNM of M00_AXI_rready_i_2 : label is "soft_lutpair21";
  attribute SOFT_HLUTNM of M00_AXI_rready_i_3 : label is "soft_lutpair29";
  attribute X_INTERFACE_INFO of M00_AXI_rready_reg : label is "xilinx.com:interface:aximm:1.0 M00_AXI RREADY";
  attribute SOFT_HLUTNM of \M00_AXI_wdata[31]_i_3\ : label is "soft_lutpair27";
  attribute X_INTERFACE_INFO of \M00_AXI_wdata_reg[0]\ : label is "xilinx.com:interface:aximm:1.0 M00_AXI WDATA";
  attribute X_INTERFACE_INFO of \M00_AXI_wdata_reg[10]\ : label is "xilinx.com:interface:aximm:1.0 M00_AXI WDATA";
  attribute X_INTERFACE_INFO of \M00_AXI_wdata_reg[11]\ : label is "xilinx.com:interface:aximm:1.0 M00_AXI WDATA";
  attribute X_INTERFACE_INFO of \M00_AXI_wdata_reg[12]\ : label is "xilinx.com:interface:aximm:1.0 M00_AXI WDATA";
  attribute X_INTERFACE_INFO of \M00_AXI_wdata_reg[13]\ : label is "xilinx.com:interface:aximm:1.0 M00_AXI WDATA";
  attribute X_INTERFACE_INFO of \M00_AXI_wdata_reg[14]\ : label is "xilinx.com:interface:aximm:1.0 M00_AXI WDATA";
  attribute X_INTERFACE_INFO of \M00_AXI_wdata_reg[15]\ : label is "xilinx.com:interface:aximm:1.0 M00_AXI WDATA";
  attribute X_INTERFACE_INFO of \M00_AXI_wdata_reg[16]\ : label is "xilinx.com:interface:aximm:1.0 M00_AXI WDATA";
  attribute X_INTERFACE_INFO of \M00_AXI_wdata_reg[17]\ : label is "xilinx.com:interface:aximm:1.0 M00_AXI WDATA";
  attribute X_INTERFACE_INFO of \M00_AXI_wdata_reg[18]\ : label is "xilinx.com:interface:aximm:1.0 M00_AXI WDATA";
  attribute X_INTERFACE_INFO of \M00_AXI_wdata_reg[19]\ : label is "xilinx.com:interface:aximm:1.0 M00_AXI WDATA";
  attribute X_INTERFACE_INFO of \M00_AXI_wdata_reg[1]\ : label is "xilinx.com:interface:aximm:1.0 M00_AXI WDATA";
  attribute X_INTERFACE_INFO of \M00_AXI_wdata_reg[20]\ : label is "xilinx.com:interface:aximm:1.0 M00_AXI WDATA";
  attribute X_INTERFACE_INFO of \M00_AXI_wdata_reg[21]\ : label is "xilinx.com:interface:aximm:1.0 M00_AXI WDATA";
  attribute X_INTERFACE_INFO of \M00_AXI_wdata_reg[22]\ : label is "xilinx.com:interface:aximm:1.0 M00_AXI WDATA";
  attribute X_INTERFACE_INFO of \M00_AXI_wdata_reg[23]\ : label is "xilinx.com:interface:aximm:1.0 M00_AXI WDATA";
  attribute X_INTERFACE_INFO of \M00_AXI_wdata_reg[24]\ : label is "xilinx.com:interface:aximm:1.0 M00_AXI WDATA";
  attribute X_INTERFACE_INFO of \M00_AXI_wdata_reg[25]\ : label is "xilinx.com:interface:aximm:1.0 M00_AXI WDATA";
  attribute X_INTERFACE_INFO of \M00_AXI_wdata_reg[26]\ : label is "xilinx.com:interface:aximm:1.0 M00_AXI WDATA";
  attribute X_INTERFACE_INFO of \M00_AXI_wdata_reg[27]\ : label is "xilinx.com:interface:aximm:1.0 M00_AXI WDATA";
  attribute X_INTERFACE_INFO of \M00_AXI_wdata_reg[28]\ : label is "xilinx.com:interface:aximm:1.0 M00_AXI WDATA";
  attribute X_INTERFACE_INFO of \M00_AXI_wdata_reg[29]\ : label is "xilinx.com:interface:aximm:1.0 M00_AXI WDATA";
  attribute X_INTERFACE_INFO of \M00_AXI_wdata_reg[2]\ : label is "xilinx.com:interface:aximm:1.0 M00_AXI WDATA";
  attribute X_INTERFACE_INFO of \M00_AXI_wdata_reg[30]\ : label is "xilinx.com:interface:aximm:1.0 M00_AXI WDATA";
  attribute X_INTERFACE_INFO of \M00_AXI_wdata_reg[31]\ : label is "xilinx.com:interface:aximm:1.0 M00_AXI WDATA";
  attribute X_INTERFACE_INFO of \M00_AXI_wdata_reg[3]\ : label is "xilinx.com:interface:aximm:1.0 M00_AXI WDATA";
  attribute X_INTERFACE_INFO of \M00_AXI_wdata_reg[4]\ : label is "xilinx.com:interface:aximm:1.0 M00_AXI WDATA";
  attribute X_INTERFACE_INFO of \M00_AXI_wdata_reg[5]\ : label is "xilinx.com:interface:aximm:1.0 M00_AXI WDATA";
  attribute X_INTERFACE_INFO of \M00_AXI_wdata_reg[6]\ : label is "xilinx.com:interface:aximm:1.0 M00_AXI WDATA";
  attribute X_INTERFACE_INFO of \M00_AXI_wdata_reg[7]\ : label is "xilinx.com:interface:aximm:1.0 M00_AXI WDATA";
  attribute X_INTERFACE_INFO of \M00_AXI_wdata_reg[8]\ : label is "xilinx.com:interface:aximm:1.0 M00_AXI WDATA";
  attribute X_INTERFACE_INFO of \M00_AXI_wdata_reg[9]\ : label is "xilinx.com:interface:aximm:1.0 M00_AXI WDATA";
  attribute SOFT_HLUTNM of \M00_AXI_wstrb[3]_i_2\ : label is "soft_lutpair28";
  attribute X_INTERFACE_INFO of \M00_AXI_wstrb_reg[0]\ : label is "xilinx.com:interface:aximm:1.0 M00_AXI WSTRB";
  attribute X_INTERFACE_INFO of \M00_AXI_wstrb_reg[1]\ : label is "xilinx.com:interface:aximm:1.0 M00_AXI WSTRB";
  attribute X_INTERFACE_INFO of \M00_AXI_wstrb_reg[2]\ : label is "xilinx.com:interface:aximm:1.0 M00_AXI WSTRB";
  attribute X_INTERFACE_INFO of \M00_AXI_wstrb_reg[3]\ : label is "xilinx.com:interface:aximm:1.0 M00_AXI WSTRB";
  attribute X_INTERFACE_INFO of M00_AXI_wvalid_reg : label is "xilinx.com:interface:aximm:1.0 M00_AXI WVALID";
  attribute SOFT_HLUTNM of \addr[23]_i_3\ : label is "soft_lutpair20";
  attribute SOFT_HLUTNM of \addr[23]_i_4\ : label is "soft_lutpair30";
  attribute SOFT_HLUTNM of \addr[31]_i_3\ : label is "soft_lutpair26";
  attribute SOFT_HLUTNM of \addr[31]_i_4\ : label is "soft_lutpair22";
  attribute SOFT_HLUTNM of \addr[31]_i_6\ : label is "soft_lutpair25";
  attribute X_INTERFACE_INFO of \addr_reg[0]\ : label is "xilinx.com:interface:aximm:1.0 M00_AXI ARADDR";
  attribute X_INTERFACE_PARAMETER : string;
  attribute X_INTERFACE_PARAMETER of \addr_reg[0]\ : label is "XIL_INTERFACENAME M00_AXI, DATA_WIDTH 32, PROTOCOL AXI3, FREQ_HZ 50000000, ID_WIDTH 0, ADDR_WIDTH 32, AWUSER_WIDTH 0, ARUSER_WIDTH 0, WUSER_WIDTH 0, RUSER_WIDTH 0, BUSER_WIDTH 0, READ_WRITE_MODE READ_WRITE, HAS_BURST 1, HAS_LOCK 0, HAS_PROT 0, HAS_CACHE 0, HAS_QOS 0, HAS_REGION 0, HAS_WSTRB 1, HAS_BRESP 1, HAS_RRESP 1, SUPPORTS_NARROW_BURST 0, NUM_READ_OUTSTANDING 2, NUM_WRITE_OUTSTANDING 2, MAX_BURST_LENGTH 1, PHASE 0.000, CLK_DOMAIN system_processing_system7_0_0_FCLK_CLK0, NUM_READ_THREADS 1, NUM_WRITE_THREADS 1, RUSER_BITS_PER_BYTE 0, WUSER_BITS_PER_BYTE 0, INSERT_VIP 0";
  attribute X_INTERFACE_INFO of \addr_reg[10]\ : label is "xilinx.com:interface:aximm:1.0 M00_AXI ARADDR";
  attribute X_INTERFACE_PARAMETER of \addr_reg[10]\ : label is "XIL_INTERFACENAME M00_AXI, DATA_WIDTH 32, PROTOCOL AXI3, FREQ_HZ 50000000, ID_WIDTH 0, ADDR_WIDTH 32, AWUSER_WIDTH 0, ARUSER_WIDTH 0, WUSER_WIDTH 0, RUSER_WIDTH 0, BUSER_WIDTH 0, READ_WRITE_MODE READ_WRITE, HAS_BURST 1, HAS_LOCK 0, HAS_PROT 0, HAS_CACHE 0, HAS_QOS 0, HAS_REGION 0, HAS_WSTRB 1, HAS_BRESP 1, HAS_RRESP 1, SUPPORTS_NARROW_BURST 0, NUM_READ_OUTSTANDING 2, NUM_WRITE_OUTSTANDING 2, MAX_BURST_LENGTH 1, PHASE 0.000, CLK_DOMAIN system_processing_system7_0_0_FCLK_CLK0, NUM_READ_THREADS 1, NUM_WRITE_THREADS 1, RUSER_BITS_PER_BYTE 0, WUSER_BITS_PER_BYTE 0, INSERT_VIP 0";
  attribute X_INTERFACE_INFO of \addr_reg[11]\ : label is "xilinx.com:interface:aximm:1.0 M00_AXI ARADDR";
  attribute X_INTERFACE_PARAMETER of \addr_reg[11]\ : label is "XIL_INTERFACENAME M00_AXI, DATA_WIDTH 32, PROTOCOL AXI3, FREQ_HZ 50000000, ID_WIDTH 0, ADDR_WIDTH 32, AWUSER_WIDTH 0, ARUSER_WIDTH 0, WUSER_WIDTH 0, RUSER_WIDTH 0, BUSER_WIDTH 0, READ_WRITE_MODE READ_WRITE, HAS_BURST 1, HAS_LOCK 0, HAS_PROT 0, HAS_CACHE 0, HAS_QOS 0, HAS_REGION 0, HAS_WSTRB 1, HAS_BRESP 1, HAS_RRESP 1, SUPPORTS_NARROW_BURST 0, NUM_READ_OUTSTANDING 2, NUM_WRITE_OUTSTANDING 2, MAX_BURST_LENGTH 1, PHASE 0.000, CLK_DOMAIN system_processing_system7_0_0_FCLK_CLK0, NUM_READ_THREADS 1, NUM_WRITE_THREADS 1, RUSER_BITS_PER_BYTE 0, WUSER_BITS_PER_BYTE 0, INSERT_VIP 0";
  attribute X_INTERFACE_INFO of \addr_reg[12]\ : label is "xilinx.com:interface:aximm:1.0 M00_AXI ARADDR";
  attribute X_INTERFACE_PARAMETER of \addr_reg[12]\ : label is "XIL_INTERFACENAME M00_AXI, DATA_WIDTH 32, PROTOCOL AXI3, FREQ_HZ 50000000, ID_WIDTH 0, ADDR_WIDTH 32, AWUSER_WIDTH 0, ARUSER_WIDTH 0, WUSER_WIDTH 0, RUSER_WIDTH 0, BUSER_WIDTH 0, READ_WRITE_MODE READ_WRITE, HAS_BURST 1, HAS_LOCK 0, HAS_PROT 0, HAS_CACHE 0, HAS_QOS 0, HAS_REGION 0, HAS_WSTRB 1, HAS_BRESP 1, HAS_RRESP 1, SUPPORTS_NARROW_BURST 0, NUM_READ_OUTSTANDING 2, NUM_WRITE_OUTSTANDING 2, MAX_BURST_LENGTH 1, PHASE 0.000, CLK_DOMAIN system_processing_system7_0_0_FCLK_CLK0, NUM_READ_THREADS 1, NUM_WRITE_THREADS 1, RUSER_BITS_PER_BYTE 0, WUSER_BITS_PER_BYTE 0, INSERT_VIP 0";
  attribute X_INTERFACE_INFO of \addr_reg[13]\ : label is "xilinx.com:interface:aximm:1.0 M00_AXI ARADDR";
  attribute X_INTERFACE_PARAMETER of \addr_reg[13]\ : label is "XIL_INTERFACENAME M00_AXI, DATA_WIDTH 32, PROTOCOL AXI3, FREQ_HZ 50000000, ID_WIDTH 0, ADDR_WIDTH 32, AWUSER_WIDTH 0, ARUSER_WIDTH 0, WUSER_WIDTH 0, RUSER_WIDTH 0, BUSER_WIDTH 0, READ_WRITE_MODE READ_WRITE, HAS_BURST 1, HAS_LOCK 0, HAS_PROT 0, HAS_CACHE 0, HAS_QOS 0, HAS_REGION 0, HAS_WSTRB 1, HAS_BRESP 1, HAS_RRESP 1, SUPPORTS_NARROW_BURST 0, NUM_READ_OUTSTANDING 2, NUM_WRITE_OUTSTANDING 2, MAX_BURST_LENGTH 1, PHASE 0.000, CLK_DOMAIN system_processing_system7_0_0_FCLK_CLK0, NUM_READ_THREADS 1, NUM_WRITE_THREADS 1, RUSER_BITS_PER_BYTE 0, WUSER_BITS_PER_BYTE 0, INSERT_VIP 0";
  attribute X_INTERFACE_INFO of \addr_reg[14]\ : label is "xilinx.com:interface:aximm:1.0 M00_AXI ARADDR";
  attribute X_INTERFACE_PARAMETER of \addr_reg[14]\ : label is "XIL_INTERFACENAME M00_AXI, DATA_WIDTH 32, PROTOCOL AXI3, FREQ_HZ 50000000, ID_WIDTH 0, ADDR_WIDTH 32, AWUSER_WIDTH 0, ARUSER_WIDTH 0, WUSER_WIDTH 0, RUSER_WIDTH 0, BUSER_WIDTH 0, READ_WRITE_MODE READ_WRITE, HAS_BURST 1, HAS_LOCK 0, HAS_PROT 0, HAS_CACHE 0, HAS_QOS 0, HAS_REGION 0, HAS_WSTRB 1, HAS_BRESP 1, HAS_RRESP 1, SUPPORTS_NARROW_BURST 0, NUM_READ_OUTSTANDING 2, NUM_WRITE_OUTSTANDING 2, MAX_BURST_LENGTH 1, PHASE 0.000, CLK_DOMAIN system_processing_system7_0_0_FCLK_CLK0, NUM_READ_THREADS 1, NUM_WRITE_THREADS 1, RUSER_BITS_PER_BYTE 0, WUSER_BITS_PER_BYTE 0, INSERT_VIP 0";
  attribute X_INTERFACE_INFO of \addr_reg[15]\ : label is "xilinx.com:interface:aximm:1.0 M00_AXI ARADDR";
  attribute X_INTERFACE_PARAMETER of \addr_reg[15]\ : label is "XIL_INTERFACENAME M00_AXI, DATA_WIDTH 32, PROTOCOL AXI3, FREQ_HZ 50000000, ID_WIDTH 0, ADDR_WIDTH 32, AWUSER_WIDTH 0, ARUSER_WIDTH 0, WUSER_WIDTH 0, RUSER_WIDTH 0, BUSER_WIDTH 0, READ_WRITE_MODE READ_WRITE, HAS_BURST 1, HAS_LOCK 0, HAS_PROT 0, HAS_CACHE 0, HAS_QOS 0, HAS_REGION 0, HAS_WSTRB 1, HAS_BRESP 1, HAS_RRESP 1, SUPPORTS_NARROW_BURST 0, NUM_READ_OUTSTANDING 2, NUM_WRITE_OUTSTANDING 2, MAX_BURST_LENGTH 1, PHASE 0.000, CLK_DOMAIN system_processing_system7_0_0_FCLK_CLK0, NUM_READ_THREADS 1, NUM_WRITE_THREADS 1, RUSER_BITS_PER_BYTE 0, WUSER_BITS_PER_BYTE 0, INSERT_VIP 0";
  attribute X_INTERFACE_INFO of \addr_reg[16]\ : label is "xilinx.com:interface:aximm:1.0 M00_AXI ARADDR";
  attribute X_INTERFACE_PARAMETER of \addr_reg[16]\ : label is "XIL_INTERFACENAME M00_AXI, DATA_WIDTH 32, PROTOCOL AXI3, FREQ_HZ 50000000, ID_WIDTH 0, ADDR_WIDTH 32, AWUSER_WIDTH 0, ARUSER_WIDTH 0, WUSER_WIDTH 0, RUSER_WIDTH 0, BUSER_WIDTH 0, READ_WRITE_MODE READ_WRITE, HAS_BURST 1, HAS_LOCK 0, HAS_PROT 0, HAS_CACHE 0, HAS_QOS 0, HAS_REGION 0, HAS_WSTRB 1, HAS_BRESP 1, HAS_RRESP 1, SUPPORTS_NARROW_BURST 0, NUM_READ_OUTSTANDING 2, NUM_WRITE_OUTSTANDING 2, MAX_BURST_LENGTH 1, PHASE 0.000, CLK_DOMAIN system_processing_system7_0_0_FCLK_CLK0, NUM_READ_THREADS 1, NUM_WRITE_THREADS 1, RUSER_BITS_PER_BYTE 0, WUSER_BITS_PER_BYTE 0, INSERT_VIP 0";
  attribute X_INTERFACE_INFO of \addr_reg[17]\ : label is "xilinx.com:interface:aximm:1.0 M00_AXI ARADDR";
  attribute X_INTERFACE_PARAMETER of \addr_reg[17]\ : label is "XIL_INTERFACENAME M00_AXI, DATA_WIDTH 32, PROTOCOL AXI3, FREQ_HZ 50000000, ID_WIDTH 0, ADDR_WIDTH 32, AWUSER_WIDTH 0, ARUSER_WIDTH 0, WUSER_WIDTH 0, RUSER_WIDTH 0, BUSER_WIDTH 0, READ_WRITE_MODE READ_WRITE, HAS_BURST 1, HAS_LOCK 0, HAS_PROT 0, HAS_CACHE 0, HAS_QOS 0, HAS_REGION 0, HAS_WSTRB 1, HAS_BRESP 1, HAS_RRESP 1, SUPPORTS_NARROW_BURST 0, NUM_READ_OUTSTANDING 2, NUM_WRITE_OUTSTANDING 2, MAX_BURST_LENGTH 1, PHASE 0.000, CLK_DOMAIN system_processing_system7_0_0_FCLK_CLK0, NUM_READ_THREADS 1, NUM_WRITE_THREADS 1, RUSER_BITS_PER_BYTE 0, WUSER_BITS_PER_BYTE 0, INSERT_VIP 0";
  attribute X_INTERFACE_INFO of \addr_reg[18]\ : label is "xilinx.com:interface:aximm:1.0 M00_AXI ARADDR";
  attribute X_INTERFACE_PARAMETER of \addr_reg[18]\ : label is "XIL_INTERFACENAME M00_AXI, DATA_WIDTH 32, PROTOCOL AXI3, FREQ_HZ 50000000, ID_WIDTH 0, ADDR_WIDTH 32, AWUSER_WIDTH 0, ARUSER_WIDTH 0, WUSER_WIDTH 0, RUSER_WIDTH 0, BUSER_WIDTH 0, READ_WRITE_MODE READ_WRITE, HAS_BURST 1, HAS_LOCK 0, HAS_PROT 0, HAS_CACHE 0, HAS_QOS 0, HAS_REGION 0, HAS_WSTRB 1, HAS_BRESP 1, HAS_RRESP 1, SUPPORTS_NARROW_BURST 0, NUM_READ_OUTSTANDING 2, NUM_WRITE_OUTSTANDING 2, MAX_BURST_LENGTH 1, PHASE 0.000, CLK_DOMAIN system_processing_system7_0_0_FCLK_CLK0, NUM_READ_THREADS 1, NUM_WRITE_THREADS 1, RUSER_BITS_PER_BYTE 0, WUSER_BITS_PER_BYTE 0, INSERT_VIP 0";
  attribute X_INTERFACE_INFO of \addr_reg[19]\ : label is "xilinx.com:interface:aximm:1.0 M00_AXI ARADDR";
  attribute X_INTERFACE_PARAMETER of \addr_reg[19]\ : label is "XIL_INTERFACENAME M00_AXI, DATA_WIDTH 32, PROTOCOL AXI3, FREQ_HZ 50000000, ID_WIDTH 0, ADDR_WIDTH 32, AWUSER_WIDTH 0, ARUSER_WIDTH 0, WUSER_WIDTH 0, RUSER_WIDTH 0, BUSER_WIDTH 0, READ_WRITE_MODE READ_WRITE, HAS_BURST 1, HAS_LOCK 0, HAS_PROT 0, HAS_CACHE 0, HAS_QOS 0, HAS_REGION 0, HAS_WSTRB 1, HAS_BRESP 1, HAS_RRESP 1, SUPPORTS_NARROW_BURST 0, NUM_READ_OUTSTANDING 2, NUM_WRITE_OUTSTANDING 2, MAX_BURST_LENGTH 1, PHASE 0.000, CLK_DOMAIN system_processing_system7_0_0_FCLK_CLK0, NUM_READ_THREADS 1, NUM_WRITE_THREADS 1, RUSER_BITS_PER_BYTE 0, WUSER_BITS_PER_BYTE 0, INSERT_VIP 0";
  attribute X_INTERFACE_INFO of \addr_reg[1]\ : label is "xilinx.com:interface:aximm:1.0 M00_AXI ARADDR";
  attribute X_INTERFACE_PARAMETER of \addr_reg[1]\ : label is "XIL_INTERFACENAME M00_AXI, DATA_WIDTH 32, PROTOCOL AXI3, FREQ_HZ 50000000, ID_WIDTH 0, ADDR_WIDTH 32, AWUSER_WIDTH 0, ARUSER_WIDTH 0, WUSER_WIDTH 0, RUSER_WIDTH 0, BUSER_WIDTH 0, READ_WRITE_MODE READ_WRITE, HAS_BURST 1, HAS_LOCK 0, HAS_PROT 0, HAS_CACHE 0, HAS_QOS 0, HAS_REGION 0, HAS_WSTRB 1, HAS_BRESP 1, HAS_RRESP 1, SUPPORTS_NARROW_BURST 0, NUM_READ_OUTSTANDING 2, NUM_WRITE_OUTSTANDING 2, MAX_BURST_LENGTH 1, PHASE 0.000, CLK_DOMAIN system_processing_system7_0_0_FCLK_CLK0, NUM_READ_THREADS 1, NUM_WRITE_THREADS 1, RUSER_BITS_PER_BYTE 0, WUSER_BITS_PER_BYTE 0, INSERT_VIP 0";
  attribute X_INTERFACE_INFO of \addr_reg[20]\ : label is "xilinx.com:interface:aximm:1.0 M00_AXI ARADDR";
  attribute X_INTERFACE_PARAMETER of \addr_reg[20]\ : label is "XIL_INTERFACENAME M00_AXI, DATA_WIDTH 32, PROTOCOL AXI3, FREQ_HZ 50000000, ID_WIDTH 0, ADDR_WIDTH 32, AWUSER_WIDTH 0, ARUSER_WIDTH 0, WUSER_WIDTH 0, RUSER_WIDTH 0, BUSER_WIDTH 0, READ_WRITE_MODE READ_WRITE, HAS_BURST 1, HAS_LOCK 0, HAS_PROT 0, HAS_CACHE 0, HAS_QOS 0, HAS_REGION 0, HAS_WSTRB 1, HAS_BRESP 1, HAS_RRESP 1, SUPPORTS_NARROW_BURST 0, NUM_READ_OUTSTANDING 2, NUM_WRITE_OUTSTANDING 2, MAX_BURST_LENGTH 1, PHASE 0.000, CLK_DOMAIN system_processing_system7_0_0_FCLK_CLK0, NUM_READ_THREADS 1, NUM_WRITE_THREADS 1, RUSER_BITS_PER_BYTE 0, WUSER_BITS_PER_BYTE 0, INSERT_VIP 0";
  attribute X_INTERFACE_INFO of \addr_reg[21]\ : label is "xilinx.com:interface:aximm:1.0 M00_AXI ARADDR";
  attribute X_INTERFACE_PARAMETER of \addr_reg[21]\ : label is "XIL_INTERFACENAME M00_AXI, DATA_WIDTH 32, PROTOCOL AXI3, FREQ_HZ 50000000, ID_WIDTH 0, ADDR_WIDTH 32, AWUSER_WIDTH 0, ARUSER_WIDTH 0, WUSER_WIDTH 0, RUSER_WIDTH 0, BUSER_WIDTH 0, READ_WRITE_MODE READ_WRITE, HAS_BURST 1, HAS_LOCK 0, HAS_PROT 0, HAS_CACHE 0, HAS_QOS 0, HAS_REGION 0, HAS_WSTRB 1, HAS_BRESP 1, HAS_RRESP 1, SUPPORTS_NARROW_BURST 0, NUM_READ_OUTSTANDING 2, NUM_WRITE_OUTSTANDING 2, MAX_BURST_LENGTH 1, PHASE 0.000, CLK_DOMAIN system_processing_system7_0_0_FCLK_CLK0, NUM_READ_THREADS 1, NUM_WRITE_THREADS 1, RUSER_BITS_PER_BYTE 0, WUSER_BITS_PER_BYTE 0, INSERT_VIP 0";
  attribute X_INTERFACE_INFO of \addr_reg[22]\ : label is "xilinx.com:interface:aximm:1.0 M00_AXI ARADDR";
  attribute X_INTERFACE_PARAMETER of \addr_reg[22]\ : label is "XIL_INTERFACENAME M00_AXI, DATA_WIDTH 32, PROTOCOL AXI3, FREQ_HZ 50000000, ID_WIDTH 0, ADDR_WIDTH 32, AWUSER_WIDTH 0, ARUSER_WIDTH 0, WUSER_WIDTH 0, RUSER_WIDTH 0, BUSER_WIDTH 0, READ_WRITE_MODE READ_WRITE, HAS_BURST 1, HAS_LOCK 0, HAS_PROT 0, HAS_CACHE 0, HAS_QOS 0, HAS_REGION 0, HAS_WSTRB 1, HAS_BRESP 1, HAS_RRESP 1, SUPPORTS_NARROW_BURST 0, NUM_READ_OUTSTANDING 2, NUM_WRITE_OUTSTANDING 2, MAX_BURST_LENGTH 1, PHASE 0.000, CLK_DOMAIN system_processing_system7_0_0_FCLK_CLK0, NUM_READ_THREADS 1, NUM_WRITE_THREADS 1, RUSER_BITS_PER_BYTE 0, WUSER_BITS_PER_BYTE 0, INSERT_VIP 0";
  attribute X_INTERFACE_INFO of \addr_reg[23]\ : label is "xilinx.com:interface:aximm:1.0 M00_AXI ARADDR";
  attribute X_INTERFACE_PARAMETER of \addr_reg[23]\ : label is "XIL_INTERFACENAME M00_AXI, DATA_WIDTH 32, PROTOCOL AXI3, FREQ_HZ 50000000, ID_WIDTH 0, ADDR_WIDTH 32, AWUSER_WIDTH 0, ARUSER_WIDTH 0, WUSER_WIDTH 0, RUSER_WIDTH 0, BUSER_WIDTH 0, READ_WRITE_MODE READ_WRITE, HAS_BURST 1, HAS_LOCK 0, HAS_PROT 0, HAS_CACHE 0, HAS_QOS 0, HAS_REGION 0, HAS_WSTRB 1, HAS_BRESP 1, HAS_RRESP 1, SUPPORTS_NARROW_BURST 0, NUM_READ_OUTSTANDING 2, NUM_WRITE_OUTSTANDING 2, MAX_BURST_LENGTH 1, PHASE 0.000, CLK_DOMAIN system_processing_system7_0_0_FCLK_CLK0, NUM_READ_THREADS 1, NUM_WRITE_THREADS 1, RUSER_BITS_PER_BYTE 0, WUSER_BITS_PER_BYTE 0, INSERT_VIP 0";
  attribute X_INTERFACE_INFO of \addr_reg[24]\ : label is "xilinx.com:interface:aximm:1.0 M00_AXI ARADDR";
  attribute X_INTERFACE_PARAMETER of \addr_reg[24]\ : label is "XIL_INTERFACENAME M00_AXI, DATA_WIDTH 32, PROTOCOL AXI3, FREQ_HZ 50000000, ID_WIDTH 0, ADDR_WIDTH 32, AWUSER_WIDTH 0, ARUSER_WIDTH 0, WUSER_WIDTH 0, RUSER_WIDTH 0, BUSER_WIDTH 0, READ_WRITE_MODE READ_WRITE, HAS_BURST 1, HAS_LOCK 0, HAS_PROT 0, HAS_CACHE 0, HAS_QOS 0, HAS_REGION 0, HAS_WSTRB 1, HAS_BRESP 1, HAS_RRESP 1, SUPPORTS_NARROW_BURST 0, NUM_READ_OUTSTANDING 2, NUM_WRITE_OUTSTANDING 2, MAX_BURST_LENGTH 1, PHASE 0.000, CLK_DOMAIN system_processing_system7_0_0_FCLK_CLK0, NUM_READ_THREADS 1, NUM_WRITE_THREADS 1, RUSER_BITS_PER_BYTE 0, WUSER_BITS_PER_BYTE 0, INSERT_VIP 0";
  attribute X_INTERFACE_INFO of \addr_reg[25]\ : label is "xilinx.com:interface:aximm:1.0 M00_AXI ARADDR";
  attribute X_INTERFACE_PARAMETER of \addr_reg[25]\ : label is "XIL_INTERFACENAME M00_AXI, DATA_WIDTH 32, PROTOCOL AXI3, FREQ_HZ 50000000, ID_WIDTH 0, ADDR_WIDTH 32, AWUSER_WIDTH 0, ARUSER_WIDTH 0, WUSER_WIDTH 0, RUSER_WIDTH 0, BUSER_WIDTH 0, READ_WRITE_MODE READ_WRITE, HAS_BURST 1, HAS_LOCK 0, HAS_PROT 0, HAS_CACHE 0, HAS_QOS 0, HAS_REGION 0, HAS_WSTRB 1, HAS_BRESP 1, HAS_RRESP 1, SUPPORTS_NARROW_BURST 0, NUM_READ_OUTSTANDING 2, NUM_WRITE_OUTSTANDING 2, MAX_BURST_LENGTH 1, PHASE 0.000, CLK_DOMAIN system_processing_system7_0_0_FCLK_CLK0, NUM_READ_THREADS 1, NUM_WRITE_THREADS 1, RUSER_BITS_PER_BYTE 0, WUSER_BITS_PER_BYTE 0, INSERT_VIP 0";
  attribute X_INTERFACE_INFO of \addr_reg[26]\ : label is "xilinx.com:interface:aximm:1.0 M00_AXI ARADDR";
  attribute X_INTERFACE_PARAMETER of \addr_reg[26]\ : label is "XIL_INTERFACENAME M00_AXI, DATA_WIDTH 32, PROTOCOL AXI3, FREQ_HZ 50000000, ID_WIDTH 0, ADDR_WIDTH 32, AWUSER_WIDTH 0, ARUSER_WIDTH 0, WUSER_WIDTH 0, RUSER_WIDTH 0, BUSER_WIDTH 0, READ_WRITE_MODE READ_WRITE, HAS_BURST 1, HAS_LOCK 0, HAS_PROT 0, HAS_CACHE 0, HAS_QOS 0, HAS_REGION 0, HAS_WSTRB 1, HAS_BRESP 1, HAS_RRESP 1, SUPPORTS_NARROW_BURST 0, NUM_READ_OUTSTANDING 2, NUM_WRITE_OUTSTANDING 2, MAX_BURST_LENGTH 1, PHASE 0.000, CLK_DOMAIN system_processing_system7_0_0_FCLK_CLK0, NUM_READ_THREADS 1, NUM_WRITE_THREADS 1, RUSER_BITS_PER_BYTE 0, WUSER_BITS_PER_BYTE 0, INSERT_VIP 0";
  attribute X_INTERFACE_INFO of \addr_reg[27]\ : label is "xilinx.com:interface:aximm:1.0 M00_AXI ARADDR";
  attribute X_INTERFACE_PARAMETER of \addr_reg[27]\ : label is "XIL_INTERFACENAME M00_AXI, DATA_WIDTH 32, PROTOCOL AXI3, FREQ_HZ 50000000, ID_WIDTH 0, ADDR_WIDTH 32, AWUSER_WIDTH 0, ARUSER_WIDTH 0, WUSER_WIDTH 0, RUSER_WIDTH 0, BUSER_WIDTH 0, READ_WRITE_MODE READ_WRITE, HAS_BURST 1, HAS_LOCK 0, HAS_PROT 0, HAS_CACHE 0, HAS_QOS 0, HAS_REGION 0, HAS_WSTRB 1, HAS_BRESP 1, HAS_RRESP 1, SUPPORTS_NARROW_BURST 0, NUM_READ_OUTSTANDING 2, NUM_WRITE_OUTSTANDING 2, MAX_BURST_LENGTH 1, PHASE 0.000, CLK_DOMAIN system_processing_system7_0_0_FCLK_CLK0, NUM_READ_THREADS 1, NUM_WRITE_THREADS 1, RUSER_BITS_PER_BYTE 0, WUSER_BITS_PER_BYTE 0, INSERT_VIP 0";
  attribute X_INTERFACE_INFO of \addr_reg[28]\ : label is "xilinx.com:interface:aximm:1.0 M00_AXI ARADDR";
  attribute X_INTERFACE_PARAMETER of \addr_reg[28]\ : label is "XIL_INTERFACENAME M00_AXI, DATA_WIDTH 32, PROTOCOL AXI3, FREQ_HZ 50000000, ID_WIDTH 0, ADDR_WIDTH 32, AWUSER_WIDTH 0, ARUSER_WIDTH 0, WUSER_WIDTH 0, RUSER_WIDTH 0, BUSER_WIDTH 0, READ_WRITE_MODE READ_WRITE, HAS_BURST 1, HAS_LOCK 0, HAS_PROT 0, HAS_CACHE 0, HAS_QOS 0, HAS_REGION 0, HAS_WSTRB 1, HAS_BRESP 1, HAS_RRESP 1, SUPPORTS_NARROW_BURST 0, NUM_READ_OUTSTANDING 2, NUM_WRITE_OUTSTANDING 2, MAX_BURST_LENGTH 1, PHASE 0.000, CLK_DOMAIN system_processing_system7_0_0_FCLK_CLK0, NUM_READ_THREADS 1, NUM_WRITE_THREADS 1, RUSER_BITS_PER_BYTE 0, WUSER_BITS_PER_BYTE 0, INSERT_VIP 0";
  attribute X_INTERFACE_INFO of \addr_reg[29]\ : label is "xilinx.com:interface:aximm:1.0 M00_AXI ARADDR";
  attribute X_INTERFACE_PARAMETER of \addr_reg[29]\ : label is "XIL_INTERFACENAME M00_AXI, DATA_WIDTH 32, PROTOCOL AXI3, FREQ_HZ 50000000, ID_WIDTH 0, ADDR_WIDTH 32, AWUSER_WIDTH 0, ARUSER_WIDTH 0, WUSER_WIDTH 0, RUSER_WIDTH 0, BUSER_WIDTH 0, READ_WRITE_MODE READ_WRITE, HAS_BURST 1, HAS_LOCK 0, HAS_PROT 0, HAS_CACHE 0, HAS_QOS 0, HAS_REGION 0, HAS_WSTRB 1, HAS_BRESP 1, HAS_RRESP 1, SUPPORTS_NARROW_BURST 0, NUM_READ_OUTSTANDING 2, NUM_WRITE_OUTSTANDING 2, MAX_BURST_LENGTH 1, PHASE 0.000, CLK_DOMAIN system_processing_system7_0_0_FCLK_CLK0, NUM_READ_THREADS 1, NUM_WRITE_THREADS 1, RUSER_BITS_PER_BYTE 0, WUSER_BITS_PER_BYTE 0, INSERT_VIP 0";
  attribute X_INTERFACE_INFO of \addr_reg[2]\ : label is "xilinx.com:interface:aximm:1.0 M00_AXI ARADDR";
  attribute X_INTERFACE_PARAMETER of \addr_reg[2]\ : label is "XIL_INTERFACENAME M00_AXI, DATA_WIDTH 32, PROTOCOL AXI3, FREQ_HZ 50000000, ID_WIDTH 0, ADDR_WIDTH 32, AWUSER_WIDTH 0, ARUSER_WIDTH 0, WUSER_WIDTH 0, RUSER_WIDTH 0, BUSER_WIDTH 0, READ_WRITE_MODE READ_WRITE, HAS_BURST 1, HAS_LOCK 0, HAS_PROT 0, HAS_CACHE 0, HAS_QOS 0, HAS_REGION 0, HAS_WSTRB 1, HAS_BRESP 1, HAS_RRESP 1, SUPPORTS_NARROW_BURST 0, NUM_READ_OUTSTANDING 2, NUM_WRITE_OUTSTANDING 2, MAX_BURST_LENGTH 1, PHASE 0.000, CLK_DOMAIN system_processing_system7_0_0_FCLK_CLK0, NUM_READ_THREADS 1, NUM_WRITE_THREADS 1, RUSER_BITS_PER_BYTE 0, WUSER_BITS_PER_BYTE 0, INSERT_VIP 0";
  attribute X_INTERFACE_INFO of \addr_reg[30]\ : label is "xilinx.com:interface:aximm:1.0 M00_AXI ARADDR";
  attribute X_INTERFACE_PARAMETER of \addr_reg[30]\ : label is "XIL_INTERFACENAME M00_AXI, DATA_WIDTH 32, PROTOCOL AXI3, FREQ_HZ 50000000, ID_WIDTH 0, ADDR_WIDTH 32, AWUSER_WIDTH 0, ARUSER_WIDTH 0, WUSER_WIDTH 0, RUSER_WIDTH 0, BUSER_WIDTH 0, READ_WRITE_MODE READ_WRITE, HAS_BURST 1, HAS_LOCK 0, HAS_PROT 0, HAS_CACHE 0, HAS_QOS 0, HAS_REGION 0, HAS_WSTRB 1, HAS_BRESP 1, HAS_RRESP 1, SUPPORTS_NARROW_BURST 0, NUM_READ_OUTSTANDING 2, NUM_WRITE_OUTSTANDING 2, MAX_BURST_LENGTH 1, PHASE 0.000, CLK_DOMAIN system_processing_system7_0_0_FCLK_CLK0, NUM_READ_THREADS 1, NUM_WRITE_THREADS 1, RUSER_BITS_PER_BYTE 0, WUSER_BITS_PER_BYTE 0, INSERT_VIP 0";
  attribute X_INTERFACE_INFO of \addr_reg[31]\ : label is "xilinx.com:interface:aximm:1.0 M00_AXI ARADDR";
  attribute X_INTERFACE_PARAMETER of \addr_reg[31]\ : label is "XIL_INTERFACENAME M00_AXI, DATA_WIDTH 32, PROTOCOL AXI3, FREQ_HZ 50000000, ID_WIDTH 0, ADDR_WIDTH 32, AWUSER_WIDTH 0, ARUSER_WIDTH 0, WUSER_WIDTH 0, RUSER_WIDTH 0, BUSER_WIDTH 0, READ_WRITE_MODE READ_WRITE, HAS_BURST 1, HAS_LOCK 0, HAS_PROT 0, HAS_CACHE 0, HAS_QOS 0, HAS_REGION 0, HAS_WSTRB 1, HAS_BRESP 1, HAS_RRESP 1, SUPPORTS_NARROW_BURST 0, NUM_READ_OUTSTANDING 2, NUM_WRITE_OUTSTANDING 2, MAX_BURST_LENGTH 1, PHASE 0.000, CLK_DOMAIN system_processing_system7_0_0_FCLK_CLK0, NUM_READ_THREADS 1, NUM_WRITE_THREADS 1, RUSER_BITS_PER_BYTE 0, WUSER_BITS_PER_BYTE 0, INSERT_VIP 0";
  attribute X_INTERFACE_INFO of \addr_reg[3]\ : label is "xilinx.com:interface:aximm:1.0 M00_AXI ARADDR";
  attribute X_INTERFACE_PARAMETER of \addr_reg[3]\ : label is "XIL_INTERFACENAME M00_AXI, DATA_WIDTH 32, PROTOCOL AXI3, FREQ_HZ 50000000, ID_WIDTH 0, ADDR_WIDTH 32, AWUSER_WIDTH 0, ARUSER_WIDTH 0, WUSER_WIDTH 0, RUSER_WIDTH 0, BUSER_WIDTH 0, READ_WRITE_MODE READ_WRITE, HAS_BURST 1, HAS_LOCK 0, HAS_PROT 0, HAS_CACHE 0, HAS_QOS 0, HAS_REGION 0, HAS_WSTRB 1, HAS_BRESP 1, HAS_RRESP 1, SUPPORTS_NARROW_BURST 0, NUM_READ_OUTSTANDING 2, NUM_WRITE_OUTSTANDING 2, MAX_BURST_LENGTH 1, PHASE 0.000, CLK_DOMAIN system_processing_system7_0_0_FCLK_CLK0, NUM_READ_THREADS 1, NUM_WRITE_THREADS 1, RUSER_BITS_PER_BYTE 0, WUSER_BITS_PER_BYTE 0, INSERT_VIP 0";
  attribute X_INTERFACE_INFO of \addr_reg[4]\ : label is "xilinx.com:interface:aximm:1.0 M00_AXI ARADDR";
  attribute X_INTERFACE_PARAMETER of \addr_reg[4]\ : label is "XIL_INTERFACENAME M00_AXI, DATA_WIDTH 32, PROTOCOL AXI3, FREQ_HZ 50000000, ID_WIDTH 0, ADDR_WIDTH 32, AWUSER_WIDTH 0, ARUSER_WIDTH 0, WUSER_WIDTH 0, RUSER_WIDTH 0, BUSER_WIDTH 0, READ_WRITE_MODE READ_WRITE, HAS_BURST 1, HAS_LOCK 0, HAS_PROT 0, HAS_CACHE 0, HAS_QOS 0, HAS_REGION 0, HAS_WSTRB 1, HAS_BRESP 1, HAS_RRESP 1, SUPPORTS_NARROW_BURST 0, NUM_READ_OUTSTANDING 2, NUM_WRITE_OUTSTANDING 2, MAX_BURST_LENGTH 1, PHASE 0.000, CLK_DOMAIN system_processing_system7_0_0_FCLK_CLK0, NUM_READ_THREADS 1, NUM_WRITE_THREADS 1, RUSER_BITS_PER_BYTE 0, WUSER_BITS_PER_BYTE 0, INSERT_VIP 0";
  attribute X_INTERFACE_INFO of \addr_reg[5]\ : label is "xilinx.com:interface:aximm:1.0 M00_AXI ARADDR";
  attribute X_INTERFACE_PARAMETER of \addr_reg[5]\ : label is "XIL_INTERFACENAME M00_AXI, DATA_WIDTH 32, PROTOCOL AXI3, FREQ_HZ 50000000, ID_WIDTH 0, ADDR_WIDTH 32, AWUSER_WIDTH 0, ARUSER_WIDTH 0, WUSER_WIDTH 0, RUSER_WIDTH 0, BUSER_WIDTH 0, READ_WRITE_MODE READ_WRITE, HAS_BURST 1, HAS_LOCK 0, HAS_PROT 0, HAS_CACHE 0, HAS_QOS 0, HAS_REGION 0, HAS_WSTRB 1, HAS_BRESP 1, HAS_RRESP 1, SUPPORTS_NARROW_BURST 0, NUM_READ_OUTSTANDING 2, NUM_WRITE_OUTSTANDING 2, MAX_BURST_LENGTH 1, PHASE 0.000, CLK_DOMAIN system_processing_system7_0_0_FCLK_CLK0, NUM_READ_THREADS 1, NUM_WRITE_THREADS 1, RUSER_BITS_PER_BYTE 0, WUSER_BITS_PER_BYTE 0, INSERT_VIP 0";
  attribute X_INTERFACE_INFO of \addr_reg[6]\ : label is "xilinx.com:interface:aximm:1.0 M00_AXI ARADDR";
  attribute X_INTERFACE_PARAMETER of \addr_reg[6]\ : label is "XIL_INTERFACENAME M00_AXI, DATA_WIDTH 32, PROTOCOL AXI3, FREQ_HZ 50000000, ID_WIDTH 0, ADDR_WIDTH 32, AWUSER_WIDTH 0, ARUSER_WIDTH 0, WUSER_WIDTH 0, RUSER_WIDTH 0, BUSER_WIDTH 0, READ_WRITE_MODE READ_WRITE, HAS_BURST 1, HAS_LOCK 0, HAS_PROT 0, HAS_CACHE 0, HAS_QOS 0, HAS_REGION 0, HAS_WSTRB 1, HAS_BRESP 1, HAS_RRESP 1, SUPPORTS_NARROW_BURST 0, NUM_READ_OUTSTANDING 2, NUM_WRITE_OUTSTANDING 2, MAX_BURST_LENGTH 1, PHASE 0.000, CLK_DOMAIN system_processing_system7_0_0_FCLK_CLK0, NUM_READ_THREADS 1, NUM_WRITE_THREADS 1, RUSER_BITS_PER_BYTE 0, WUSER_BITS_PER_BYTE 0, INSERT_VIP 0";
  attribute X_INTERFACE_INFO of \addr_reg[7]\ : label is "xilinx.com:interface:aximm:1.0 M00_AXI ARADDR";
  attribute X_INTERFACE_PARAMETER of \addr_reg[7]\ : label is "XIL_INTERFACENAME M00_AXI, DATA_WIDTH 32, PROTOCOL AXI3, FREQ_HZ 50000000, ID_WIDTH 0, ADDR_WIDTH 32, AWUSER_WIDTH 0, ARUSER_WIDTH 0, WUSER_WIDTH 0, RUSER_WIDTH 0, BUSER_WIDTH 0, READ_WRITE_MODE READ_WRITE, HAS_BURST 1, HAS_LOCK 0, HAS_PROT 0, HAS_CACHE 0, HAS_QOS 0, HAS_REGION 0, HAS_WSTRB 1, HAS_BRESP 1, HAS_RRESP 1, SUPPORTS_NARROW_BURST 0, NUM_READ_OUTSTANDING 2, NUM_WRITE_OUTSTANDING 2, MAX_BURST_LENGTH 1, PHASE 0.000, CLK_DOMAIN system_processing_system7_0_0_FCLK_CLK0, NUM_READ_THREADS 1, NUM_WRITE_THREADS 1, RUSER_BITS_PER_BYTE 0, WUSER_BITS_PER_BYTE 0, INSERT_VIP 0";
  attribute X_INTERFACE_INFO of \addr_reg[8]\ : label is "xilinx.com:interface:aximm:1.0 M00_AXI ARADDR";
  attribute X_INTERFACE_PARAMETER of \addr_reg[8]\ : label is "XIL_INTERFACENAME M00_AXI, DATA_WIDTH 32, PROTOCOL AXI3, FREQ_HZ 50000000, ID_WIDTH 0, ADDR_WIDTH 32, AWUSER_WIDTH 0, ARUSER_WIDTH 0, WUSER_WIDTH 0, RUSER_WIDTH 0, BUSER_WIDTH 0, READ_WRITE_MODE READ_WRITE, HAS_BURST 1, HAS_LOCK 0, HAS_PROT 0, HAS_CACHE 0, HAS_QOS 0, HAS_REGION 0, HAS_WSTRB 1, HAS_BRESP 1, HAS_RRESP 1, SUPPORTS_NARROW_BURST 0, NUM_READ_OUTSTANDING 2, NUM_WRITE_OUTSTANDING 2, MAX_BURST_LENGTH 1, PHASE 0.000, CLK_DOMAIN system_processing_system7_0_0_FCLK_CLK0, NUM_READ_THREADS 1, NUM_WRITE_THREADS 1, RUSER_BITS_PER_BYTE 0, WUSER_BITS_PER_BYTE 0, INSERT_VIP 0";
  attribute X_INTERFACE_INFO of \addr_reg[9]\ : label is "xilinx.com:interface:aximm:1.0 M00_AXI ARADDR";
  attribute X_INTERFACE_PARAMETER of \addr_reg[9]\ : label is "XIL_INTERFACENAME M00_AXI, DATA_WIDTH 32, PROTOCOL AXI3, FREQ_HZ 50000000, ID_WIDTH 0, ADDR_WIDTH 32, AWUSER_WIDTH 0, ARUSER_WIDTH 0, WUSER_WIDTH 0, RUSER_WIDTH 0, BUSER_WIDTH 0, READ_WRITE_MODE READ_WRITE, HAS_BURST 1, HAS_LOCK 0, HAS_PROT 0, HAS_CACHE 0, HAS_QOS 0, HAS_REGION 0, HAS_WSTRB 1, HAS_BRESP 1, HAS_RRESP 1, SUPPORTS_NARROW_BURST 0, NUM_READ_OUTSTANDING 2, NUM_WRITE_OUTSTANDING 2, MAX_BURST_LENGTH 1, PHASE 0.000, CLK_DOMAIN system_processing_system7_0_0_FCLK_CLK0, NUM_READ_THREADS 1, NUM_WRITE_THREADS 1, RUSER_BITS_PER_BYTE 0, WUSER_BITS_PER_BYTE 0, INSERT_VIP 0";
  attribute SOFT_HLUTNM of command_done_i_5 : label is "soft_lutpair28";
  attribute SOFT_HLUTNM of \debug_readmem_step[0]_i_2\ : label is "soft_lutpair23";
  attribute SOFT_HLUTNM of \debug_readmem_step[0]_i_3\ : label is "soft_lutpair21";
  attribute SOFT_HLUTNM of \debug_readmem_step[1]_i_2\ : label is "soft_lutpair23";
  attribute SOFT_HLUTNM of \debug_readmem_step[1]_i_5\ : label is "soft_lutpair20";
  attribute SOFT_HLUTNM of \debug_readmem_step[1]_i_6\ : label is "soft_lutpair30";
  attribute SOFT_HLUTNM of \debug_readmem_step[1]_i_7\ : label is "soft_lutpair22";
  attribute SOFT_HLUTNM of \debug_readmem_step[1]_i_8\ : label is "soft_lutpair29";
  attribute SOFT_HLUTNM of \uart_data_in[7]_i_3\ : label is "soft_lutpair26";
  attribute SOFT_HLUTNM of \uart_data_in[7]_i_4\ : label is "soft_lutpair25";
begin
  M00_AXI_rready <= \^m00_axi_rready\;
  \debug[4]\ <= \^debug[4]\;
  \debug[6]\ <= \^debug[6]\;
  debug_0_sp_1 <= debug_0_sn_1;
  led(1 downto 0) <= \^led\(1 downto 0);
M00_AXI_arvalid_i_1: unisim.vcomponents.LUT6
    generic map(
      INIT => X"F0F0F0F000F1F0F0"
    )
        port map (
      I0 => \debug_readmem_step_reg_n_0_[0]\,
      I1 => \debug_readmem_step_reg_n_0_[1]\,
      I2 => debug_0_sn_1,
      I3 => debug(0),
      I4 => M00_AXI_bready_i_2_n_0,
      I5 => command(0),
      O => M00_AXI_arvalid_i_1_n_0
    );
M00_AXI_arvalid_reg: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => '1',
      CLR => uart_hs_inst_n_0,
      D => M00_AXI_arvalid_i_1_n_0,
      Q => debug_0_sn_1
    );
M00_AXI_awvalid_i_1: unisim.vcomponents.LUT6
    generic map(
      INIT => X"2223AAAAAAAAAAAA"
    )
        port map (
      I0 => \^debug[4]\,
      I1 => debug(2),
      I2 => \debug_readmem_step_reg_n_0_[0]\,
      I3 => \debug_readmem_step_reg_n_0_[1]\,
      I4 => M00_AXI_bready_i_2_n_0,
      I5 => command(0),
      O => M00_AXI_awvalid_i_1_n_0
    );
M00_AXI_awvalid_reg: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => '1',
      CLR => uart_hs_inst_n_0,
      D => M00_AXI_awvalid_i_1_n_0,
      Q => \^debug[4]\
    );
M00_AXI_bready_i_1: unisim.vcomponents.LUT5
    generic map(
      INIT => X"F7F78000"
    )
        port map (
      I0 => command(0),
      I1 => M00_AXI_bready_i_2_n_0,
      I2 => M00_AXI_bready_i_3_n_0,
      I3 => \command_done2__1\,
      I4 => \^led\(0),
      O => M00_AXI_bready_i_1_n_0
    );
M00_AXI_bready_i_2: unisim.vcomponents.LUT6
    generic map(
      INIT => X"0000000000000400"
    )
        port map (
      I0 => command(3),
      I1 => command(5),
      I2 => M00_AXI_bready_i_5_n_0,
      I3 => command(4),
      I4 => command_done_reg_n_0,
      I5 => \addr[31]_i_3_n_0\,
      O => M00_AXI_bready_i_2_n_0
    );
M00_AXI_bready_i_3: unisim.vcomponents.LUT5
    generic map(
      INIT => X"FE0FFFFF"
    )
        port map (
      I0 => \^debug[6]\,
      I1 => \^debug[4]\,
      I2 => \debug_readmem_step_reg_n_0_[1]\,
      I3 => \debug_readmem_step_reg_n_0_[0]\,
      I4 => M00_AXI_bvalid,
      O => M00_AXI_bready_i_3_n_0
    );
M00_AXI_bready_i_4: unisim.vcomponents.LUT4
    generic map(
      INIT => X"0100"
    )
        port map (
      I0 => \^debug[6]\,
      I1 => \^debug[4]\,
      I2 => \debug_readmem_step_reg_n_0_[1]\,
      I3 => \debug_readmem_step_reg_n_0_[0]\,
      O => \command_done2__1\
    );
M00_AXI_bready_i_5: unisim.vcomponents.LUT2
    generic map(
      INIT => X"E"
    )
        port map (
      I0 => command(2),
      I1 => command(1),
      O => M00_AXI_bready_i_5_n_0
    );
M00_AXI_bready_reg: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => '1',
      CLR => uart_hs_inst_n_0,
      D => M00_AXI_bready_i_1_n_0,
      Q => \^led\(0)
    );
M00_AXI_rready_i_1: unisim.vcomponents.LUT5
    generic map(
      INIT => X"FBFB4000"
    )
        port map (
      I0 => command(0),
      I1 => M00_AXI_bready_i_2_n_0,
      I2 => M00_AXI_rready_i_2_n_0,
      I3 => command_done221_out,
      I4 => \^m00_axi_rready\,
      O => M00_AXI_rready_i_1_n_0
    );
M00_AXI_rready_i_2: unisim.vcomponents.LUT5
    generic map(
      INIT => X"0ABBFFFF"
    )
        port map (
      I0 => \debug_readmem_step_reg_n_0_[0]\,
      I1 => \debug_readmem_step_reg_n_0_[1]\,
      I2 => debug_0_sn_1,
      I3 => debug(0),
      I4 => debug(1),
      O => M00_AXI_rready_i_2_n_0
    );
M00_AXI_rready_i_3: unisim.vcomponents.LUT4
    generic map(
      INIT => X"F100"
    )
        port map (
      I0 => \debug_readmem_step_reg_n_0_[0]\,
      I1 => \debug_readmem_step_reg_n_0_[1]\,
      I2 => debug_0_sn_1,
      I3 => debug(0),
      O => command_done221_out
    );
M00_AXI_rready_reg: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => '1',
      CLR => uart_hs_inst_n_0,
      D => M00_AXI_rready_i_1_n_0,
      Q => \^m00_axi_rready\
    );
\M00_AXI_wdata[15]_i_1\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"AAA8AAAAAAAAAAAA"
    )
        port map (
      I0 => \M00_AXI_wdata[15]_i_2_n_0\,
      I1 => \addr[31]_i_3_n_0\,
      I2 => command(4),
      I3 => command(3),
      I4 => command(5),
      I5 => command(2),
      O => \M00_AXI_wdata[15]_i_1_n_0\
    );
\M00_AXI_wdata[15]_i_2\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"0000000000200000"
    )
        port map (
      I0 => command(2),
      I1 => \addr[23]_i_3_n_0\,
      I2 => command(0),
      I3 => command(1),
      I4 => \addr[23]_i_4_n_0\,
      I5 => \M00_AXI_wdata[31]_i_4_n_0\,
      O => \M00_AXI_wdata[15]_i_2_n_0\
    );
\M00_AXI_wdata[23]_i_1\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"AAA8AAAAAAAAAAAA"
    )
        port map (
      I0 => \M00_AXI_wdata[23]_i_2_n_0\,
      I1 => \addr[31]_i_3_n_0\,
      I2 => command(4),
      I3 => command(3),
      I4 => command(5),
      I5 => command(2),
      O => \M00_AXI_wdata[23]_i_1_n_0\
    );
\M00_AXI_wdata[23]_i_2\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"0200000000000000"
    )
        port map (
      I0 => command(2),
      I1 => \addr[23]_i_3_n_0\,
      I2 => command(0),
      I3 => \addr[23]_i_4_n_0\,
      I4 => command(1),
      I5 => \M00_AXI_wdata[31]_i_4_n_0\,
      O => \M00_AXI_wdata[23]_i_2_n_0\
    );
\M00_AXI_wdata[31]_i_1\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"0200000000000000"
    )
        port map (
      I0 => \M00_AXI_wdata[31]_i_2_n_0\,
      I1 => \addr[31]_i_3_n_0\,
      I2 => \M00_AXI_wdata[31]_i_3_n_0\,
      I3 => command(1),
      I4 => command(0),
      I5 => \M00_AXI_wdata[31]_i_4_n_0\,
      O => \M00_AXI_wdata[31]_i_1_n_0\
    );
\M00_AXI_wdata[31]_i_2\: unisim.vcomponents.LUT2
    generic map(
      INIT => X"2"
    )
        port map (
      I0 => keyin(3),
      I1 => command_done_reg_n_0,
      O => \M00_AXI_wdata[31]_i_2_n_0\
    );
\M00_AXI_wdata[31]_i_3\: unisim.vcomponents.LUT4
    generic map(
      INIT => X"EFFF"
    )
        port map (
      I0 => command(4),
      I1 => command(3),
      I2 => command(5),
      I3 => command(2),
      O => \M00_AXI_wdata[31]_i_3_n_0\
    );
\M00_AXI_wdata[31]_i_4\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"FFEEFFEEFFDDDDDC"
    )
        port map (
      I0 => command(5),
      I1 => command(3),
      I2 => command(0),
      I3 => command(2),
      I4 => command(1),
      I5 => command(4),
      O => \M00_AXI_wdata[31]_i_4_n_0\
    );
\M00_AXI_wdata[7]_i_1\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"AAA8AAAAAAAAAAAA"
    )
        port map (
      I0 => \M00_AXI_wdata[7]_i_2_n_0\,
      I1 => \addr[31]_i_3_n_0\,
      I2 => command(4),
      I3 => command(3),
      I4 => command(5),
      I5 => command(2),
      O => \M00_AXI_wdata[7]_i_1_n_0\
    );
\M00_AXI_wdata[7]_i_2\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"0000000000020000"
    )
        port map (
      I0 => command(2),
      I1 => \addr[23]_i_3_n_0\,
      I2 => command(0),
      I3 => command(1),
      I4 => \addr[23]_i_4_n_0\,
      I5 => \M00_AXI_wdata[31]_i_4_n_0\,
      O => \M00_AXI_wdata[7]_i_2_n_0\
    );
\M00_AXI_wdata_reg[0]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \M00_AXI_wdata[7]_i_2_n_0\,
      D => \data_reg_n_0_[0]\,
      Q => M00_AXI_wdata(0),
      R => \M00_AXI_wdata[7]_i_1_n_0\
    );
\M00_AXI_wdata_reg[10]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \M00_AXI_wdata[15]_i_2_n_0\,
      D => \data_reg_n_0_[2]\,
      Q => M00_AXI_wdata(10),
      R => \M00_AXI_wdata[15]_i_1_n_0\
    );
\M00_AXI_wdata_reg[11]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \M00_AXI_wdata[15]_i_2_n_0\,
      D => \data_reg_n_0_[3]\,
      Q => M00_AXI_wdata(11),
      R => \M00_AXI_wdata[15]_i_1_n_0\
    );
\M00_AXI_wdata_reg[12]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \M00_AXI_wdata[15]_i_2_n_0\,
      D => \data_reg_n_0_[4]\,
      Q => M00_AXI_wdata(12),
      R => \M00_AXI_wdata[15]_i_1_n_0\
    );
\M00_AXI_wdata_reg[13]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \M00_AXI_wdata[15]_i_2_n_0\,
      D => \data_reg_n_0_[5]\,
      Q => M00_AXI_wdata(13),
      R => \M00_AXI_wdata[15]_i_1_n_0\
    );
\M00_AXI_wdata_reg[14]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \M00_AXI_wdata[15]_i_2_n_0\,
      D => \data_reg_n_0_[6]\,
      Q => M00_AXI_wdata(14),
      R => \M00_AXI_wdata[15]_i_1_n_0\
    );
\M00_AXI_wdata_reg[15]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \M00_AXI_wdata[15]_i_2_n_0\,
      D => \data_reg_n_0_[7]\,
      Q => M00_AXI_wdata(15),
      R => \M00_AXI_wdata[15]_i_1_n_0\
    );
\M00_AXI_wdata_reg[16]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \M00_AXI_wdata[23]_i_2_n_0\,
      D => \data_reg_n_0_[0]\,
      Q => M00_AXI_wdata(16),
      R => \M00_AXI_wdata[23]_i_1_n_0\
    );
\M00_AXI_wdata_reg[17]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \M00_AXI_wdata[23]_i_2_n_0\,
      D => \data_reg_n_0_[1]\,
      Q => M00_AXI_wdata(17),
      R => \M00_AXI_wdata[23]_i_1_n_0\
    );
\M00_AXI_wdata_reg[18]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \M00_AXI_wdata[23]_i_2_n_0\,
      D => \data_reg_n_0_[2]\,
      Q => M00_AXI_wdata(18),
      R => \M00_AXI_wdata[23]_i_1_n_0\
    );
\M00_AXI_wdata_reg[19]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \M00_AXI_wdata[23]_i_2_n_0\,
      D => \data_reg_n_0_[3]\,
      Q => M00_AXI_wdata(19),
      R => \M00_AXI_wdata[23]_i_1_n_0\
    );
\M00_AXI_wdata_reg[1]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \M00_AXI_wdata[7]_i_2_n_0\,
      D => \data_reg_n_0_[1]\,
      Q => M00_AXI_wdata(1),
      R => \M00_AXI_wdata[7]_i_1_n_0\
    );
\M00_AXI_wdata_reg[20]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \M00_AXI_wdata[23]_i_2_n_0\,
      D => \data_reg_n_0_[4]\,
      Q => M00_AXI_wdata(20),
      R => \M00_AXI_wdata[23]_i_1_n_0\
    );
\M00_AXI_wdata_reg[21]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \M00_AXI_wdata[23]_i_2_n_0\,
      D => \data_reg_n_0_[5]\,
      Q => M00_AXI_wdata(21),
      R => \M00_AXI_wdata[23]_i_1_n_0\
    );
\M00_AXI_wdata_reg[22]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \M00_AXI_wdata[23]_i_2_n_0\,
      D => \data_reg_n_0_[6]\,
      Q => M00_AXI_wdata(22),
      R => \M00_AXI_wdata[23]_i_1_n_0\
    );
\M00_AXI_wdata_reg[23]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \M00_AXI_wdata[23]_i_2_n_0\,
      D => \data_reg_n_0_[7]\,
      Q => M00_AXI_wdata(23),
      R => \M00_AXI_wdata[23]_i_1_n_0\
    );
\M00_AXI_wdata_reg[24]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \M00_AXI_wdata[31]_i_1_n_0\,
      D => \data_reg_n_0_[0]\,
      Q => M00_AXI_wdata(24),
      R => '0'
    );
\M00_AXI_wdata_reg[25]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \M00_AXI_wdata[31]_i_1_n_0\,
      D => \data_reg_n_0_[1]\,
      Q => M00_AXI_wdata(25),
      R => '0'
    );
\M00_AXI_wdata_reg[26]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \M00_AXI_wdata[31]_i_1_n_0\,
      D => \data_reg_n_0_[2]\,
      Q => M00_AXI_wdata(26),
      R => '0'
    );
\M00_AXI_wdata_reg[27]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \M00_AXI_wdata[31]_i_1_n_0\,
      D => \data_reg_n_0_[3]\,
      Q => M00_AXI_wdata(27),
      R => '0'
    );
\M00_AXI_wdata_reg[28]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \M00_AXI_wdata[31]_i_1_n_0\,
      D => \data_reg_n_0_[4]\,
      Q => M00_AXI_wdata(28),
      R => '0'
    );
\M00_AXI_wdata_reg[29]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \M00_AXI_wdata[31]_i_1_n_0\,
      D => \data_reg_n_0_[5]\,
      Q => M00_AXI_wdata(29),
      R => '0'
    );
\M00_AXI_wdata_reg[2]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \M00_AXI_wdata[7]_i_2_n_0\,
      D => \data_reg_n_0_[2]\,
      Q => M00_AXI_wdata(2),
      R => \M00_AXI_wdata[7]_i_1_n_0\
    );
\M00_AXI_wdata_reg[30]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \M00_AXI_wdata[31]_i_1_n_0\,
      D => \data_reg_n_0_[6]\,
      Q => M00_AXI_wdata(30),
      R => '0'
    );
\M00_AXI_wdata_reg[31]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \M00_AXI_wdata[31]_i_1_n_0\,
      D => \data_reg_n_0_[7]\,
      Q => M00_AXI_wdata(31),
      R => '0'
    );
\M00_AXI_wdata_reg[3]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \M00_AXI_wdata[7]_i_2_n_0\,
      D => \data_reg_n_0_[3]\,
      Q => M00_AXI_wdata(3),
      R => \M00_AXI_wdata[7]_i_1_n_0\
    );
\M00_AXI_wdata_reg[4]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \M00_AXI_wdata[7]_i_2_n_0\,
      D => \data_reg_n_0_[4]\,
      Q => M00_AXI_wdata(4),
      R => \M00_AXI_wdata[7]_i_1_n_0\
    );
\M00_AXI_wdata_reg[5]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \M00_AXI_wdata[7]_i_2_n_0\,
      D => \data_reg_n_0_[5]\,
      Q => M00_AXI_wdata(5),
      R => \M00_AXI_wdata[7]_i_1_n_0\
    );
\M00_AXI_wdata_reg[6]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \M00_AXI_wdata[7]_i_2_n_0\,
      D => \data_reg_n_0_[6]\,
      Q => M00_AXI_wdata(6),
      R => \M00_AXI_wdata[7]_i_1_n_0\
    );
\M00_AXI_wdata_reg[7]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \M00_AXI_wdata[7]_i_2_n_0\,
      D => \data_reg_n_0_[7]\,
      Q => M00_AXI_wdata(7),
      R => \M00_AXI_wdata[7]_i_1_n_0\
    );
\M00_AXI_wdata_reg[8]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \M00_AXI_wdata[15]_i_2_n_0\,
      D => \data_reg_n_0_[0]\,
      Q => M00_AXI_wdata(8),
      R => \M00_AXI_wdata[15]_i_1_n_0\
    );
\M00_AXI_wdata_reg[9]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \M00_AXI_wdata[15]_i_2_n_0\,
      D => \data_reg_n_0_[1]\,
      Q => M00_AXI_wdata(9),
      R => \M00_AXI_wdata[15]_i_1_n_0\
    );
\M00_AXI_wstrb[3]_i_1\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"0000000000004000"
    )
        port map (
      I0 => \M00_AXI_wstrb[3]_i_2_n_0\,
      I1 => command(3),
      I2 => command(5),
      I3 => keyin(3),
      I4 => \addr[31]_i_3_n_0\,
      I5 => command_done_reg_n_0,
      O => \M00_AXI_wstrb[3]_i_1_n_0\
    );
\M00_AXI_wstrb[3]_i_2\: unisim.vcomponents.LUT4
    generic map(
      INIT => X"FFFE"
    )
        port map (
      I0 => command(0),
      I1 => command(2),
      I2 => command(1),
      I3 => command(4),
      O => \M00_AXI_wstrb[3]_i_2_n_0\
    );
\M00_AXI_wstrb_reg[0]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \M00_AXI_wstrb[3]_i_1_n_0\,
      D => \data_reg_n_0_[0]\,
      Q => M00_AXI_wstrb(0),
      R => '0'
    );
\M00_AXI_wstrb_reg[1]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \M00_AXI_wstrb[3]_i_1_n_0\,
      D => \data_reg_n_0_[1]\,
      Q => M00_AXI_wstrb(1),
      R => '0'
    );
\M00_AXI_wstrb_reg[2]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \M00_AXI_wstrb[3]_i_1_n_0\,
      D => \data_reg_n_0_[2]\,
      Q => M00_AXI_wstrb(2),
      R => '0'
    );
\M00_AXI_wstrb_reg[3]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \M00_AXI_wstrb[3]_i_1_n_0\,
      D => \data_reg_n_0_[3]\,
      Q => M00_AXI_wstrb(3),
      R => '0'
    );
M00_AXI_wvalid_i_1: unisim.vcomponents.LUT6
    generic map(
      INIT => X"2223AAAAAAAAAAAA"
    )
        port map (
      I0 => \^debug[6]\,
      I1 => debug(3),
      I2 => \debug_readmem_step_reg_n_0_[0]\,
      I3 => \debug_readmem_step_reg_n_0_[1]\,
      I4 => M00_AXI_bready_i_2_n_0,
      I5 => command(0),
      O => M00_AXI_wvalid_i_1_n_0
    );
M00_AXI_wvalid_reg: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => '1',
      CLR => uart_hs_inst_n_0,
      D => M00_AXI_wvalid_i_1_n_0,
      Q => \^debug[6]\
    );
\addr[15]_i_1\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"AAAAAAAAAAA8AAAA"
    )
        port map (
      I0 => \addr[15]_i_2_n_0\,
      I1 => \addr[31]_i_3_n_0\,
      I2 => command(4),
      I3 => command(3),
      I4 => command(5),
      I5 => command(2),
      O => \addr[15]_i_1_n_0\
    );
\addr[15]_i_2\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"0000000400000000"
    )
        port map (
      I0 => \addr[31]_i_5_n_0\,
      I1 => command(0),
      I2 => \addr[23]_i_3_n_0\,
      I3 => command(1),
      I4 => command(2),
      I5 => \addr[23]_i_4_n_0\,
      O => \addr[15]_i_2_n_0\
    );
\addr[23]_i_1\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"AAAAAAAAAAA8AAAA"
    )
        port map (
      I0 => \addr[23]_i_2_n_0\,
      I1 => \addr[31]_i_3_n_0\,
      I2 => command(4),
      I3 => command(3),
      I4 => command(5),
      I5 => command(2),
      O => \addr[23]_i_1_n_0\
    );
\addr[23]_i_2\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"0000040000000000"
    )
        port map (
      I0 => command(0),
      I1 => \addr[31]_i_5_n_0\,
      I2 => \addr[23]_i_3_n_0\,
      I3 => command(1),
      I4 => command(2),
      I5 => \addr[23]_i_4_n_0\,
      O => \addr[23]_i_2_n_0\
    );
\addr[23]_i_3\: unisim.vcomponents.LUT3
    generic map(
      INIT => X"FD"
    )
        port map (
      I0 => command(5),
      I1 => command(3),
      I2 => command(4),
      O => \addr[23]_i_3_n_0\
    );
\addr[23]_i_4\: unisim.vcomponents.LUT4
    generic map(
      INIT => X"0100"
    )
        port map (
      I0 => command_done_reg_n_0,
      I1 => command(7),
      I2 => command(6),
      I3 => keyin(3),
      O => \addr[23]_i_4_n_0\
    );
\addr[31]_i_1\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"000000008A888888"
    )
        port map (
      I0 => \addr[31]_i_2_n_0\,
      I1 => \addr[31]_i_3_n_0\,
      I2 => \addr[31]_i_4_n_0\,
      I3 => \addr[31]_i_5_n_0\,
      I4 => command(0),
      I5 => \addr[31]_i_6_n_0\,
      O => \addr[31]_i_1_n_0\
    );
\addr[31]_i_2\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"0000100000000000"
    )
        port map (
      I0 => command(7),
      I1 => command(6),
      I2 => command(0),
      I3 => command(1),
      I4 => command_done_reg_n_0,
      I5 => keyin(3),
      O => \addr[31]_i_2_n_0\
    );
\addr[31]_i_3\: unisim.vcomponents.LUT2
    generic map(
      INIT => X"E"
    )
        port map (
      I0 => command(7),
      I1 => command(6),
      O => \addr[31]_i_3_n_0\
    );
\addr[31]_i_4\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"00000010"
    )
        port map (
      I0 => command(4),
      I1 => command(3),
      I2 => command(5),
      I3 => command(1),
      I4 => command(2),
      O => \addr[31]_i_4_n_0\
    );
\addr[31]_i_5\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"FFFEFFFEEEFFEEFE"
    )
        port map (
      I0 => command(2),
      I1 => command(3),
      I2 => command(1),
      I3 => command(4),
      I4 => command(0),
      I5 => command(5),
      O => \addr[31]_i_5_n_0\
    );
\addr[31]_i_6\: unisim.vcomponents.LUT4
    generic map(
      INIT => X"FFEF"
    )
        port map (
      I0 => command(4),
      I1 => command(3),
      I2 => command(5),
      I3 => command(2),
      O => \addr[31]_i_6_n_0\
    );
\addr[7]_i_1\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"AAAAAAAAAAA8AAAA"
    )
        port map (
      I0 => \addr[7]_i_2_n_0\,
      I1 => \addr[31]_i_3_n_0\,
      I2 => command(4),
      I3 => command(3),
      I4 => command(5),
      I5 => command(2),
      O => \addr[7]_i_1_n_0\
    );
\addr[7]_i_2\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"0000000100000000"
    )
        port map (
      I0 => \addr[31]_i_5_n_0\,
      I1 => command(0),
      I2 => \addr[23]_i_3_n_0\,
      I3 => command(1),
      I4 => command(2),
      I5 => \addr[23]_i_4_n_0\,
      O => \addr[7]_i_2_n_0\
    );
\addr_reg[0]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \addr[7]_i_2_n_0\,
      D => \data_reg_n_0_[0]\,
      Q => M00_AXI_araddr(0),
      R => \addr[7]_i_1_n_0\
    );
\addr_reg[10]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \addr[15]_i_2_n_0\,
      D => \data_reg_n_0_[2]\,
      Q => M00_AXI_araddr(10),
      R => \addr[15]_i_1_n_0\
    );
\addr_reg[11]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \addr[15]_i_2_n_0\,
      D => \data_reg_n_0_[3]\,
      Q => M00_AXI_araddr(11),
      R => \addr[15]_i_1_n_0\
    );
\addr_reg[12]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \addr[15]_i_2_n_0\,
      D => \data_reg_n_0_[4]\,
      Q => M00_AXI_araddr(12),
      R => \addr[15]_i_1_n_0\
    );
\addr_reg[13]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \addr[15]_i_2_n_0\,
      D => \data_reg_n_0_[5]\,
      Q => M00_AXI_araddr(13),
      R => \addr[15]_i_1_n_0\
    );
\addr_reg[14]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \addr[15]_i_2_n_0\,
      D => \data_reg_n_0_[6]\,
      Q => M00_AXI_araddr(14),
      R => \addr[15]_i_1_n_0\
    );
\addr_reg[15]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \addr[15]_i_2_n_0\,
      D => \data_reg_n_0_[7]\,
      Q => M00_AXI_araddr(15),
      R => \addr[15]_i_1_n_0\
    );
\addr_reg[16]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \addr[23]_i_2_n_0\,
      D => \data_reg_n_0_[0]\,
      Q => M00_AXI_araddr(16),
      R => \addr[23]_i_1_n_0\
    );
\addr_reg[17]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \addr[23]_i_2_n_0\,
      D => \data_reg_n_0_[1]\,
      Q => M00_AXI_araddr(17),
      R => \addr[23]_i_1_n_0\
    );
\addr_reg[18]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \addr[23]_i_2_n_0\,
      D => \data_reg_n_0_[2]\,
      Q => M00_AXI_araddr(18),
      R => \addr[23]_i_1_n_0\
    );
\addr_reg[19]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \addr[23]_i_2_n_0\,
      D => \data_reg_n_0_[3]\,
      Q => M00_AXI_araddr(19),
      R => \addr[23]_i_1_n_0\
    );
\addr_reg[1]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \addr[7]_i_2_n_0\,
      D => \data_reg_n_0_[1]\,
      Q => M00_AXI_araddr(1),
      R => \addr[7]_i_1_n_0\
    );
\addr_reg[20]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \addr[23]_i_2_n_0\,
      D => \data_reg_n_0_[4]\,
      Q => M00_AXI_araddr(20),
      R => \addr[23]_i_1_n_0\
    );
\addr_reg[21]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \addr[23]_i_2_n_0\,
      D => \data_reg_n_0_[5]\,
      Q => M00_AXI_araddr(21),
      R => \addr[23]_i_1_n_0\
    );
\addr_reg[22]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \addr[23]_i_2_n_0\,
      D => \data_reg_n_0_[6]\,
      Q => M00_AXI_araddr(22),
      R => \addr[23]_i_1_n_0\
    );
\addr_reg[23]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \addr[23]_i_2_n_0\,
      D => \data_reg_n_0_[7]\,
      Q => M00_AXI_araddr(23),
      R => \addr[23]_i_1_n_0\
    );
\addr_reg[24]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \addr[31]_i_1_n_0\,
      D => \data_reg_n_0_[0]\,
      Q => M00_AXI_araddr(24),
      R => '0'
    );
\addr_reg[25]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \addr[31]_i_1_n_0\,
      D => \data_reg_n_0_[1]\,
      Q => M00_AXI_araddr(25),
      R => '0'
    );
\addr_reg[26]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \addr[31]_i_1_n_0\,
      D => \data_reg_n_0_[2]\,
      Q => M00_AXI_araddr(26),
      R => '0'
    );
\addr_reg[27]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \addr[31]_i_1_n_0\,
      D => \data_reg_n_0_[3]\,
      Q => M00_AXI_araddr(27),
      R => '0'
    );
\addr_reg[28]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \addr[31]_i_1_n_0\,
      D => \data_reg_n_0_[4]\,
      Q => M00_AXI_araddr(28),
      R => '0'
    );
\addr_reg[29]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \addr[31]_i_1_n_0\,
      D => \data_reg_n_0_[5]\,
      Q => M00_AXI_araddr(29),
      R => '0'
    );
\addr_reg[2]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \addr[7]_i_2_n_0\,
      D => \data_reg_n_0_[2]\,
      Q => M00_AXI_araddr(2),
      R => \addr[7]_i_1_n_0\
    );
\addr_reg[30]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \addr[31]_i_1_n_0\,
      D => \data_reg_n_0_[6]\,
      Q => M00_AXI_araddr(30),
      R => '0'
    );
\addr_reg[31]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \addr[31]_i_1_n_0\,
      D => \data_reg_n_0_[7]\,
      Q => M00_AXI_araddr(31),
      R => '0'
    );
\addr_reg[3]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \addr[7]_i_2_n_0\,
      D => \data_reg_n_0_[3]\,
      Q => M00_AXI_araddr(3),
      R => \addr[7]_i_1_n_0\
    );
\addr_reg[4]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \addr[7]_i_2_n_0\,
      D => \data_reg_n_0_[4]\,
      Q => M00_AXI_araddr(4),
      R => \addr[7]_i_1_n_0\
    );
\addr_reg[5]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \addr[7]_i_2_n_0\,
      D => \data_reg_n_0_[5]\,
      Q => M00_AXI_araddr(5),
      R => \addr[7]_i_1_n_0\
    );
\addr_reg[6]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \addr[7]_i_2_n_0\,
      D => \data_reg_n_0_[6]\,
      Q => M00_AXI_araddr(6),
      R => \addr[7]_i_1_n_0\
    );
\addr_reg[7]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \addr[7]_i_2_n_0\,
      D => \data_reg_n_0_[7]\,
      Q => M00_AXI_araddr(7),
      R => \addr[7]_i_1_n_0\
    );
\addr_reg[8]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \addr[15]_i_2_n_0\,
      D => \data_reg_n_0_[0]\,
      Q => M00_AXI_araddr(8),
      R => \addr[15]_i_1_n_0\
    );
\addr_reg[9]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \addr[15]_i_2_n_0\,
      D => \data_reg_n_0_[1]\,
      Q => M00_AXI_araddr(9),
      R => \addr[15]_i_1_n_0\
    );
command_done_i_1: unisim.vcomponents.LUT6
    generic map(
      INIT => X"CCCCCCCC8080FF80"
    )
        port map (
      I0 => command_done_i_2_n_0,
      I1 => command_done_i_3_n_0,
      I2 => keyin(3),
      I3 => command_done_i_4_n_0,
      I4 => command(7),
      I5 => command_done_reg_n_0,
      O => command_done_i_1_n_0
    );
command_done_i_2: unisim.vcomponents.LUT6
    generic map(
      INIT => X"0000001010100010"
    )
        port map (
      I0 => command(7),
      I1 => command(6),
      I2 => \debug_readmem_step[1]_i_7_n_0\,
      I3 => M00_AXI_rready_i_2_n_0,
      I4 => command(0),
      I5 => M00_AXI_bready_i_3_n_0,
      O => command_done_i_2_n_0
    );
command_done_i_3: unisim.vcomponents.LUT6
    generic map(
      INIT => X"FFFFFFFFFFFFFFFD"
    )
        port map (
      I0 => command_done_reg_n_0,
      I1 => command(7),
      I2 => command(6),
      I3 => command(3),
      I4 => command(5),
      I5 => \M00_AXI_wstrb[3]_i_2_n_0\,
      O => command_done_i_3_n_0
    );
command_done_i_4: unisim.vcomponents.LUT6
    generic map(
      INIT => X"00000144111101CC"
    )
        port map (
      I0 => command(3),
      I1 => command(5),
      I2 => command(2),
      I3 => command(4),
      I4 => command(6),
      I5 => command_done_i_5_n_0,
      O => command_done_i_4_n_0
    );
command_done_i_5: unisim.vcomponents.LUT3
    generic map(
      INIT => X"FE"
    )
        port map (
      I0 => command(1),
      I1 => command(2),
      I2 => command(0),
      O => command_done_i_5_n_0
    );
command_done_reg: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => '1',
      CLR => uart_hs_inst_n_0,
      D => command_done_i_1_n_0,
      Q => command_done_reg_n_0
    );
\command_reg[0]\: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => uart_hs_inst_n_11,
      CLR => uart_hs_inst_n_0,
      D => uart_hs_inst_n_20,
      Q => command(0)
    );
\command_reg[1]\: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => uart_hs_inst_n_11,
      CLR => uart_hs_inst_n_0,
      D => uart_hs_inst_n_19,
      Q => command(1)
    );
\command_reg[2]\: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => uart_hs_inst_n_11,
      CLR => uart_hs_inst_n_0,
      D => uart_hs_inst_n_18,
      Q => command(2)
    );
\command_reg[3]\: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => uart_hs_inst_n_11,
      CLR => uart_hs_inst_n_0,
      D => uart_hs_inst_n_17,
      Q => command(3)
    );
\command_reg[4]\: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => uart_hs_inst_n_11,
      CLR => uart_hs_inst_n_0,
      D => uart_hs_inst_n_16,
      Q => command(4)
    );
\command_reg[5]\: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => uart_hs_inst_n_11,
      CLR => uart_hs_inst_n_0,
      D => uart_hs_inst_n_15,
      Q => command(5)
    );
\command_reg[6]\: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => uart_hs_inst_n_11,
      CLR => uart_hs_inst_n_0,
      D => uart_hs_inst_n_14,
      Q => command(6)
    );
\command_reg[7]\: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => uart_hs_inst_n_11,
      CLR => uart_hs_inst_n_0,
      D => uart_hs_inst_n_13,
      Q => command(7)
    );
\command_temp_reg[0]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => command_temp_1,
      D => uart_data_out(0),
      Q => command_temp(0),
      R => '0'
    );
\command_temp_reg[1]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => command_temp_1,
      D => uart_data_out(1),
      Q => command_temp(1),
      R => '0'
    );
\command_temp_reg[2]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => command_temp_1,
      D => uart_data_out(2),
      Q => command_temp(2),
      R => '0'
    );
\command_temp_reg[3]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => command_temp_1,
      D => uart_data_out(3),
      Q => command_temp(3),
      R => '0'
    );
\command_temp_reg[4]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => command_temp_1,
      D => uart_data_out(4),
      Q => command_temp(4),
      R => '0'
    );
\command_temp_reg[5]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => command_temp_1,
      D => uart_data_out(5),
      Q => command_temp(5),
      R => '0'
    );
\command_temp_reg[6]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => command_temp_1,
      D => uart_data_out(6),
      Q => command_temp(6),
      R => '0'
    );
\command_temp_reg[7]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => command_temp_1,
      D => uart_data_out(7),
      Q => command_temp(7),
      R => '0'
    );
\data_buff[31]_i_1\: unisim.vcomponents.LUT4
    generic map(
      INIT => X"0400"
    )
        port map (
      I0 => M00_AXI_rready_i_2_n_0,
      I1 => keyin(3),
      I2 => command(0),
      I3 => M00_AXI_bready_i_2_n_0,
      O => data_buff
    );
\data_buff_reg[0]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => data_buff,
      D => M00_AXI_rdata(0),
      Q => \data_buff_reg_n_0_[0]\,
      R => '0'
    );
\data_buff_reg[10]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => data_buff,
      D => M00_AXI_rdata(10),
      Q => \data_buff_reg_n_0_[10]\,
      R => '0'
    );
\data_buff_reg[11]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => data_buff,
      D => M00_AXI_rdata(11),
      Q => \data_buff_reg_n_0_[11]\,
      R => '0'
    );
\data_buff_reg[12]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => data_buff,
      D => M00_AXI_rdata(12),
      Q => \data_buff_reg_n_0_[12]\,
      R => '0'
    );
\data_buff_reg[13]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => data_buff,
      D => M00_AXI_rdata(13),
      Q => \data_buff_reg_n_0_[13]\,
      R => '0'
    );
\data_buff_reg[14]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => data_buff,
      D => M00_AXI_rdata(14),
      Q => \data_buff_reg_n_0_[14]\,
      R => '0'
    );
\data_buff_reg[15]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => data_buff,
      D => M00_AXI_rdata(15),
      Q => \data_buff_reg_n_0_[15]\,
      R => '0'
    );
\data_buff_reg[16]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => data_buff,
      D => M00_AXI_rdata(16),
      Q => \data_buff_reg_n_0_[16]\,
      R => '0'
    );
\data_buff_reg[17]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => data_buff,
      D => M00_AXI_rdata(17),
      Q => \data_buff_reg_n_0_[17]\,
      R => '0'
    );
\data_buff_reg[18]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => data_buff,
      D => M00_AXI_rdata(18),
      Q => \data_buff_reg_n_0_[18]\,
      R => '0'
    );
\data_buff_reg[19]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => data_buff,
      D => M00_AXI_rdata(19),
      Q => \data_buff_reg_n_0_[19]\,
      R => '0'
    );
\data_buff_reg[1]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => data_buff,
      D => M00_AXI_rdata(1),
      Q => \data_buff_reg_n_0_[1]\,
      R => '0'
    );
\data_buff_reg[20]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => data_buff,
      D => M00_AXI_rdata(20),
      Q => \data_buff_reg_n_0_[20]\,
      R => '0'
    );
\data_buff_reg[21]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => data_buff,
      D => M00_AXI_rdata(21),
      Q => \data_buff_reg_n_0_[21]\,
      R => '0'
    );
\data_buff_reg[22]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => data_buff,
      D => M00_AXI_rdata(22),
      Q => \data_buff_reg_n_0_[22]\,
      R => '0'
    );
\data_buff_reg[23]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => data_buff,
      D => M00_AXI_rdata(23),
      Q => \data_buff_reg_n_0_[23]\,
      R => '0'
    );
\data_buff_reg[24]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => data_buff,
      D => M00_AXI_rdata(24),
      Q => \data_buff_reg_n_0_[24]\,
      R => '0'
    );
\data_buff_reg[25]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => data_buff,
      D => M00_AXI_rdata(25),
      Q => \data_buff_reg_n_0_[25]\,
      R => '0'
    );
\data_buff_reg[26]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => data_buff,
      D => M00_AXI_rdata(26),
      Q => \data_buff_reg_n_0_[26]\,
      R => '0'
    );
\data_buff_reg[27]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => data_buff,
      D => M00_AXI_rdata(27),
      Q => \data_buff_reg_n_0_[27]\,
      R => '0'
    );
\data_buff_reg[28]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => data_buff,
      D => M00_AXI_rdata(28),
      Q => \data_buff_reg_n_0_[28]\,
      R => '0'
    );
\data_buff_reg[29]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => data_buff,
      D => M00_AXI_rdata(29),
      Q => \data_buff_reg_n_0_[29]\,
      R => '0'
    );
\data_buff_reg[2]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => data_buff,
      D => M00_AXI_rdata(2),
      Q => \data_buff_reg_n_0_[2]\,
      R => '0'
    );
\data_buff_reg[30]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => data_buff,
      D => M00_AXI_rdata(30),
      Q => \data_buff_reg_n_0_[30]\,
      R => '0'
    );
\data_buff_reg[31]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => data_buff,
      D => M00_AXI_rdata(31),
      Q => \data_buff_reg_n_0_[31]\,
      R => '0'
    );
\data_buff_reg[3]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => data_buff,
      D => M00_AXI_rdata(3),
      Q => \data_buff_reg_n_0_[3]\,
      R => '0'
    );
\data_buff_reg[4]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => data_buff,
      D => M00_AXI_rdata(4),
      Q => \data_buff_reg_n_0_[4]\,
      R => '0'
    );
\data_buff_reg[5]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => data_buff,
      D => M00_AXI_rdata(5),
      Q => \data_buff_reg_n_0_[5]\,
      R => '0'
    );
\data_buff_reg[6]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => data_buff,
      D => M00_AXI_rdata(6),
      Q => \data_buff_reg_n_0_[6]\,
      R => '0'
    );
\data_buff_reg[7]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => data_buff,
      D => M00_AXI_rdata(7),
      Q => \data_buff_reg_n_0_[7]\,
      R => '0'
    );
\data_buff_reg[8]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => data_buff,
      D => M00_AXI_rdata(8),
      Q => \data_buff_reg_n_0_[8]\,
      R => '0'
    );
\data_buff_reg[9]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => data_buff,
      D => M00_AXI_rdata(9),
      Q => \data_buff_reg_n_0_[9]\,
      R => '0'
    );
data_cmd_reg: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => '1',
      D => uart_hs_inst_n_21,
      Q => data_cmd_reg_n_0,
      R => '0'
    );
\data_reg[0]\: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => data,
      CLR => uart_hs_inst_n_0,
      D => uart_data_out(0),
      Q => \data_reg_n_0_[0]\
    );
\data_reg[1]\: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => data,
      CLR => uart_hs_inst_n_0,
      D => uart_data_out(1),
      Q => \data_reg_n_0_[1]\
    );
\data_reg[2]\: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => data,
      CLR => uart_hs_inst_n_0,
      D => uart_data_out(2),
      Q => \data_reg_n_0_[2]\
    );
\data_reg[3]\: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => data,
      CLR => uart_hs_inst_n_0,
      D => uart_data_out(3),
      Q => \data_reg_n_0_[3]\
    );
\data_reg[4]\: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => data,
      CLR => uart_hs_inst_n_0,
      D => uart_data_out(4),
      Q => \data_reg_n_0_[4]\
    );
\data_reg[5]\: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => data,
      CLR => uart_hs_inst_n_0,
      D => uart_data_out(5),
      Q => \data_reg_n_0_[5]\
    );
\data_reg[6]\: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => data,
      CLR => uart_hs_inst_n_0,
      D => uart_data_out(6),
      Q => \data_reg_n_0_[6]\
    );
\data_reg[7]\: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => data,
      CLR => uart_hs_inst_n_0,
      D => uart_data_out(7),
      Q => \data_reg_n_0_[7]\
    );
\debug_readmem_step[0]_i_1\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"47FF4700"
    )
        port map (
      I0 => \debug_readmem_step[0]_i_2_n_0\,
      I1 => \debug_readmem_step[1]_i_3_n_0\,
      I2 => \debug_readmem_step[0]_i_3_n_0\,
      I3 => \debug_readmem_step[1]_i_4_n_0\,
      I4 => \debug_readmem_step_reg_n_0_[0]\,
      O => \debug_readmem_step[0]_i_1_n_0\
    );
\debug_readmem_step[0]_i_2\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"2020202C"
    )
        port map (
      I0 => M00_AXI_bvalid,
      I1 => \debug_readmem_step_reg_n_0_[0]\,
      I2 => \debug_readmem_step_reg_n_0_[1]\,
      I3 => \^debug[4]\,
      I4 => \^debug[6]\,
      O => \debug_readmem_step[0]_i_2_n_0\
    );
\debug_readmem_step[0]_i_3\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"C0C0EACC"
    )
        port map (
      I0 => debug(1),
      I1 => debug(0),
      I2 => debug_0_sn_1,
      I3 => \debug_readmem_step_reg_n_0_[1]\,
      I4 => \debug_readmem_step_reg_n_0_[0]\,
      O => \debug_readmem_step[0]_i_3_n_0\
    );
\debug_readmem_step[1]_i_1\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"B888FFFFB8880000"
    )
        port map (
      I0 => \debug_readmem_step[1]_i_2_n_0\,
      I1 => \debug_readmem_step[1]_i_3_n_0\,
      I2 => M00_AXI_rready_i_2_n_0,
      I3 => command_done221_out,
      I4 => \debug_readmem_step[1]_i_4_n_0\,
      I5 => \debug_readmem_step_reg_n_0_[1]\,
      O => \debug_readmem_step[1]_i_1_n_0\
    );
\debug_readmem_step[1]_i_2\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"00000004"
    )
        port map (
      I0 => M00_AXI_bvalid,
      I1 => \debug_readmem_step_reg_n_0_[0]\,
      I2 => \debug_readmem_step_reg_n_0_[1]\,
      I3 => \^debug[4]\,
      I4 => \^debug[6]\,
      O => \debug_readmem_step[1]_i_2_n_0\
    );
\debug_readmem_step[1]_i_3\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"FFFFFFFFFFEFFFFF"
    )
        port map (
      I0 => command(1),
      I1 => command(2),
      I2 => command(4),
      I3 => command(0),
      I4 => command(5),
      I5 => \debug_readmem_step[1]_i_5_n_0\,
      O => \debug_readmem_step[1]_i_3_n_0\
    );
\debug_readmem_step[1]_i_4\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"8880008088888888"
    )
        port map (
      I0 => \debug_readmem_step[1]_i_6_n_0\,
      I1 => \debug_readmem_step[1]_i_7_n_0\,
      I2 => \debug_readmem_step[0]_i_3_n_0\,
      I3 => command(0),
      I4 => \debug_readmem_step[0]_i_2_n_0\,
      I5 => \debug_readmem_step[1]_i_8_n_0\,
      O => \debug_readmem_step[1]_i_4_n_0\
    );
\debug_readmem_step[1]_i_5\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"FFEFEFEF"
    )
        port map (
      I0 => command(6),
      I1 => command(7),
      I2 => command(5),
      I3 => command(3),
      I4 => command(4),
      O => \debug_readmem_step[1]_i_5_n_0\
    );
\debug_readmem_step[1]_i_6\: unisim.vcomponents.LUT3
    generic map(
      INIT => X"01"
    )
        port map (
      I0 => command(6),
      I1 => command(7),
      I2 => command_done_reg_n_0,
      O => \debug_readmem_step[1]_i_6_n_0\
    );
\debug_readmem_step[1]_i_7\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"00000200"
    )
        port map (
      I0 => command(4),
      I1 => command(2),
      I2 => command(1),
      I3 => command(5),
      I4 => command(3),
      O => \debug_readmem_step[1]_i_7_n_0\
    );
\debug_readmem_step[1]_i_8\: unisim.vcomponents.LUT2
    generic map(
      INIT => X"E"
    )
        port map (
      I0 => \debug_readmem_step_reg_n_0_[0]\,
      I1 => \debug_readmem_step_reg_n_0_[1]\,
      O => \debug_readmem_step[1]_i_8_n_0\
    );
\debug_readmem_step_reg[0]\: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => '1',
      CLR => uart_hs_inst_n_0,
      D => \debug_readmem_step[0]_i_1_n_0\,
      Q => \debug_readmem_step_reg_n_0_[0]\
    );
\debug_readmem_step_reg[1]\: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => '1',
      CLR => uart_hs_inst_n_0,
      D => \debug_readmem_step[1]_i_1_n_0\,
      Q => \debug_readmem_step_reg_n_0_[1]\
    );
flg_i_1: unisim.vcomponents.LUT5
    generic map(
      INIT => X"FFFE0001"
    )
        port map (
      I0 => flg_i_3_n_0,
      I1 => flg_i_4_n_0,
      I2 => flg_i_5_n_0,
      I3 => flg_i_6_n_0,
      I4 => \^led\(1),
      O => flg_i_1_n_0
    );
flg_i_10: unisim.vcomponents.LUT4
    generic map(
      INIT => X"FFDF"
    )
        port map (
      I0 => timer33(13),
      I1 => timer33(12),
      I2 => timer33(15),
      I3 => timer33(14),
      O => flg_i_10_n_0
    );
flg_i_3: unisim.vcomponents.LUT5
    generic map(
      INIT => X"FFFFFFF7"
    )
        port map (
      I0 => timer33(18),
      I1 => timer33(19),
      I2 => timer33(16),
      I3 => timer33(17),
      I4 => flg_i_7_n_0,
      O => flg_i_3_n_0
    );
flg_i_4: unisim.vcomponents.LUT5
    generic map(
      INIT => X"FFFFFEFF"
    )
        port map (
      I0 => timer33(26),
      I1 => timer33(27),
      I2 => timer33(25),
      I3 => timer33(24),
      I4 => flg_i_8_n_0,
      O => flg_i_4_n_0
    );
flg_i_5: unisim.vcomponents.LUT5
    generic map(
      INIT => X"FFFFFBFF"
    )
        port map (
      I0 => timer33(3),
      I1 => timer33(2),
      I2 => timer33(1),
      I3 => timer33(0),
      I4 => flg_i_9_n_0,
      O => flg_i_5_n_0
    );
flg_i_6: unisim.vcomponents.LUT5
    generic map(
      INIT => X"FFFFFFFE"
    )
        port map (
      I0 => timer33(10),
      I1 => timer33(11),
      I2 => timer33(8),
      I3 => timer33(9),
      I4 => flg_i_10_n_0,
      O => flg_i_6_n_0
    );
flg_i_7: unisim.vcomponents.LUT4
    generic map(
      INIT => X"7FFF"
    )
        port map (
      I0 => timer33(21),
      I1 => timer33(20),
      I2 => timer33(23),
      I3 => timer33(22),
      O => flg_i_7_n_0
    );
flg_i_8: unisim.vcomponents.LUT4
    generic map(
      INIT => X"FFFE"
    )
        port map (
      I0 => timer33(29),
      I1 => timer33(28),
      I2 => timer33(31),
      I3 => timer33(30),
      O => flg_i_8_n_0
    );
flg_i_9: unisim.vcomponents.LUT4
    generic map(
      INIT => X"FFDF"
    )
        port map (
      I0 => timer33(4),
      I1 => timer33(5),
      I2 => timer33(6),
      I3 => timer33(7),
      O => flg_i_9_n_0
    );
flg_reg: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => '1',
      CLR => uart_hs_inst_n_0,
      D => flg_i_1_n_0,
      Q => \^led\(1)
    );
\timer33[0]_i_1\: unisim.vcomponents.LUT1
    generic map(
      INIT => X"1"
    )
        port map (
      I0 => timer33(0),
      O => timer33_2(0)
    );
\timer33[10]_i_1\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"FFFE0000"
    )
        port map (
      I0 => flg_i_3_n_0,
      I1 => flg_i_4_n_0,
      I2 => flg_i_5_n_0,
      I3 => flg_i_6_n_0,
      I4 => data0(10),
      O => timer33_2(10)
    );
\timer33[11]_i_1\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"FFFE0000"
    )
        port map (
      I0 => flg_i_3_n_0,
      I1 => flg_i_4_n_0,
      I2 => flg_i_5_n_0,
      I3 => flg_i_6_n_0,
      I4 => data0(11),
      O => timer33_2(11)
    );
\timer33[12]_i_1\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"FFFE0000"
    )
        port map (
      I0 => flg_i_3_n_0,
      I1 => flg_i_4_n_0,
      I2 => flg_i_5_n_0,
      I3 => flg_i_6_n_0,
      I4 => data0(12),
      O => timer33_2(12)
    );
\timer33[13]_i_1\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"FFFE0000"
    )
        port map (
      I0 => flg_i_3_n_0,
      I1 => flg_i_4_n_0,
      I2 => flg_i_5_n_0,
      I3 => flg_i_6_n_0,
      I4 => data0(13),
      O => timer33_2(13)
    );
\timer33[14]_i_1\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"FFFE0000"
    )
        port map (
      I0 => flg_i_3_n_0,
      I1 => flg_i_4_n_0,
      I2 => flg_i_5_n_0,
      I3 => flg_i_6_n_0,
      I4 => data0(14),
      O => timer33_2(14)
    );
\timer33[15]_i_1\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"FFFE0000"
    )
        port map (
      I0 => flg_i_3_n_0,
      I1 => flg_i_4_n_0,
      I2 => flg_i_5_n_0,
      I3 => flg_i_6_n_0,
      I4 => data0(15),
      O => timer33_2(15)
    );
\timer33[16]_i_1\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"FFFE0000"
    )
        port map (
      I0 => flg_i_3_n_0,
      I1 => flg_i_4_n_0,
      I2 => flg_i_5_n_0,
      I3 => flg_i_6_n_0,
      I4 => data0(16),
      O => timer33_2(16)
    );
\timer33[17]_i_1\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"FFFE0000"
    )
        port map (
      I0 => flg_i_3_n_0,
      I1 => flg_i_4_n_0,
      I2 => flg_i_5_n_0,
      I3 => flg_i_6_n_0,
      I4 => data0(17),
      O => timer33_2(17)
    );
\timer33[18]_i_1\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"FFFE0000"
    )
        port map (
      I0 => flg_i_3_n_0,
      I1 => flg_i_4_n_0,
      I2 => flg_i_5_n_0,
      I3 => flg_i_6_n_0,
      I4 => data0(18),
      O => timer33_2(18)
    );
\timer33[19]_i_1\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"FFFE0000"
    )
        port map (
      I0 => flg_i_3_n_0,
      I1 => flg_i_4_n_0,
      I2 => flg_i_5_n_0,
      I3 => flg_i_6_n_0,
      I4 => data0(19),
      O => timer33_2(19)
    );
\timer33[1]_i_1\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"FFFE0000"
    )
        port map (
      I0 => flg_i_3_n_0,
      I1 => flg_i_4_n_0,
      I2 => flg_i_5_n_0,
      I3 => flg_i_6_n_0,
      I4 => data0(1),
      O => timer33_2(1)
    );
\timer33[20]_i_1\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"FFFE0000"
    )
        port map (
      I0 => flg_i_3_n_0,
      I1 => flg_i_4_n_0,
      I2 => flg_i_5_n_0,
      I3 => flg_i_6_n_0,
      I4 => data0(20),
      O => timer33_2(20)
    );
\timer33[21]_i_1\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"FFFE0000"
    )
        port map (
      I0 => flg_i_3_n_0,
      I1 => flg_i_4_n_0,
      I2 => flg_i_5_n_0,
      I3 => flg_i_6_n_0,
      I4 => data0(21),
      O => timer33_2(21)
    );
\timer33[22]_i_1\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"FFFE0000"
    )
        port map (
      I0 => flg_i_3_n_0,
      I1 => flg_i_4_n_0,
      I2 => flg_i_5_n_0,
      I3 => flg_i_6_n_0,
      I4 => data0(22),
      O => timer33_2(22)
    );
\timer33[23]_i_1\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"FFFE0000"
    )
        port map (
      I0 => flg_i_3_n_0,
      I1 => flg_i_4_n_0,
      I2 => flg_i_5_n_0,
      I3 => flg_i_6_n_0,
      I4 => data0(23),
      O => timer33_2(23)
    );
\timer33[24]_i_1\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"FFFE0000"
    )
        port map (
      I0 => flg_i_3_n_0,
      I1 => flg_i_4_n_0,
      I2 => flg_i_5_n_0,
      I3 => flg_i_6_n_0,
      I4 => data0(24),
      O => timer33_2(24)
    );
\timer33[25]_i_1\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"FFFE0000"
    )
        port map (
      I0 => flg_i_3_n_0,
      I1 => flg_i_4_n_0,
      I2 => flg_i_5_n_0,
      I3 => flg_i_6_n_0,
      I4 => data0(25),
      O => timer33_2(25)
    );
\timer33[26]_i_1\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"FFFE0000"
    )
        port map (
      I0 => flg_i_3_n_0,
      I1 => flg_i_4_n_0,
      I2 => flg_i_5_n_0,
      I3 => flg_i_6_n_0,
      I4 => data0(26),
      O => timer33_2(26)
    );
\timer33[27]_i_1\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"FFFE0000"
    )
        port map (
      I0 => flg_i_3_n_0,
      I1 => flg_i_4_n_0,
      I2 => flg_i_5_n_0,
      I3 => flg_i_6_n_0,
      I4 => data0(27),
      O => timer33_2(27)
    );
\timer33[28]_i_1\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"FFFE0000"
    )
        port map (
      I0 => flg_i_3_n_0,
      I1 => flg_i_4_n_0,
      I2 => flg_i_5_n_0,
      I3 => flg_i_6_n_0,
      I4 => data0(28),
      O => timer33_2(28)
    );
\timer33[29]_i_1\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"FFFE0000"
    )
        port map (
      I0 => flg_i_3_n_0,
      I1 => flg_i_4_n_0,
      I2 => flg_i_5_n_0,
      I3 => flg_i_6_n_0,
      I4 => data0(29),
      O => timer33_2(29)
    );
\timer33[2]_i_1\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"FFFE0000"
    )
        port map (
      I0 => flg_i_3_n_0,
      I1 => flg_i_4_n_0,
      I2 => flg_i_5_n_0,
      I3 => flg_i_6_n_0,
      I4 => data0(2),
      O => timer33_2(2)
    );
\timer33[30]_i_1\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"FFFE0000"
    )
        port map (
      I0 => flg_i_3_n_0,
      I1 => flg_i_4_n_0,
      I2 => flg_i_5_n_0,
      I3 => flg_i_6_n_0,
      I4 => data0(30),
      O => timer33_2(30)
    );
\timer33[31]_i_1\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"FFFE0000"
    )
        port map (
      I0 => flg_i_3_n_0,
      I1 => flg_i_4_n_0,
      I2 => flg_i_5_n_0,
      I3 => flg_i_6_n_0,
      I4 => data0(31),
      O => timer33_2(31)
    );
\timer33[3]_i_1\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"FFFE0000"
    )
        port map (
      I0 => flg_i_3_n_0,
      I1 => flg_i_4_n_0,
      I2 => flg_i_5_n_0,
      I3 => flg_i_6_n_0,
      I4 => data0(3),
      O => timer33_2(3)
    );
\timer33[4]_i_1\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"FFFE0000"
    )
        port map (
      I0 => flg_i_3_n_0,
      I1 => flg_i_4_n_0,
      I2 => flg_i_5_n_0,
      I3 => flg_i_6_n_0,
      I4 => data0(4),
      O => timer33_2(4)
    );
\timer33[5]_i_1\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"FFFE0000"
    )
        port map (
      I0 => flg_i_3_n_0,
      I1 => flg_i_4_n_0,
      I2 => flg_i_5_n_0,
      I3 => flg_i_6_n_0,
      I4 => data0(5),
      O => timer33_2(5)
    );
\timer33[6]_i_1\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"FFFE0000"
    )
        port map (
      I0 => flg_i_3_n_0,
      I1 => flg_i_4_n_0,
      I2 => flg_i_5_n_0,
      I3 => flg_i_6_n_0,
      I4 => data0(6),
      O => timer33_2(6)
    );
\timer33[7]_i_1\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"FFFE0000"
    )
        port map (
      I0 => flg_i_3_n_0,
      I1 => flg_i_4_n_0,
      I2 => flg_i_5_n_0,
      I3 => flg_i_6_n_0,
      I4 => data0(7),
      O => timer33_2(7)
    );
\timer33[8]_i_1\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"FFFE0000"
    )
        port map (
      I0 => flg_i_3_n_0,
      I1 => flg_i_4_n_0,
      I2 => flg_i_5_n_0,
      I3 => flg_i_6_n_0,
      I4 => data0(8),
      O => timer33_2(8)
    );
\timer33[9]_i_1\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"FFFE0000"
    )
        port map (
      I0 => flg_i_3_n_0,
      I1 => flg_i_4_n_0,
      I2 => flg_i_5_n_0,
      I3 => flg_i_6_n_0,
      I4 => data0(9),
      O => timer33_2(9)
    );
\timer33_reg[0]\: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => '1',
      CLR => uart_hs_inst_n_0,
      D => timer33_2(0),
      Q => timer33(0)
    );
\timer33_reg[10]\: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => '1',
      CLR => uart_hs_inst_n_0,
      D => timer33_2(10),
      Q => timer33(10)
    );
\timer33_reg[11]\: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => '1',
      CLR => uart_hs_inst_n_0,
      D => timer33_2(11),
      Q => timer33(11)
    );
\timer33_reg[12]\: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => '1',
      CLR => uart_hs_inst_n_0,
      D => timer33_2(12),
      Q => timer33(12)
    );
\timer33_reg[12]_i_2\: unisim.vcomponents.CARRY4
     port map (
      CI => \timer33_reg[8]_i_2_n_0\,
      CO(3) => \timer33_reg[12]_i_2_n_0\,
      CO(2) => \timer33_reg[12]_i_2_n_1\,
      CO(1) => \timer33_reg[12]_i_2_n_2\,
      CO(0) => \timer33_reg[12]_i_2_n_3\,
      CYINIT => '0',
      DI(3 downto 0) => B"0000",
      O(3 downto 0) => data0(12 downto 9),
      S(3 downto 0) => timer33(12 downto 9)
    );
\timer33_reg[13]\: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => '1',
      CLR => uart_hs_inst_n_0,
      D => timer33_2(13),
      Q => timer33(13)
    );
\timer33_reg[14]\: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => '1',
      CLR => uart_hs_inst_n_0,
      D => timer33_2(14),
      Q => timer33(14)
    );
\timer33_reg[15]\: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => '1',
      CLR => uart_hs_inst_n_0,
      D => timer33_2(15),
      Q => timer33(15)
    );
\timer33_reg[16]\: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => '1',
      CLR => uart_hs_inst_n_0,
      D => timer33_2(16),
      Q => timer33(16)
    );
\timer33_reg[16]_i_2\: unisim.vcomponents.CARRY4
     port map (
      CI => \timer33_reg[12]_i_2_n_0\,
      CO(3) => \timer33_reg[16]_i_2_n_0\,
      CO(2) => \timer33_reg[16]_i_2_n_1\,
      CO(1) => \timer33_reg[16]_i_2_n_2\,
      CO(0) => \timer33_reg[16]_i_2_n_3\,
      CYINIT => '0',
      DI(3 downto 0) => B"0000",
      O(3 downto 0) => data0(16 downto 13),
      S(3 downto 0) => timer33(16 downto 13)
    );
\timer33_reg[17]\: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => '1',
      CLR => uart_hs_inst_n_0,
      D => timer33_2(17),
      Q => timer33(17)
    );
\timer33_reg[18]\: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => '1',
      CLR => uart_hs_inst_n_0,
      D => timer33_2(18),
      Q => timer33(18)
    );
\timer33_reg[19]\: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => '1',
      CLR => uart_hs_inst_n_0,
      D => timer33_2(19),
      Q => timer33(19)
    );
\timer33_reg[1]\: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => '1',
      CLR => uart_hs_inst_n_0,
      D => timer33_2(1),
      Q => timer33(1)
    );
\timer33_reg[20]\: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => '1',
      CLR => uart_hs_inst_n_0,
      D => timer33_2(20),
      Q => timer33(20)
    );
\timer33_reg[20]_i_2\: unisim.vcomponents.CARRY4
     port map (
      CI => \timer33_reg[16]_i_2_n_0\,
      CO(3) => \timer33_reg[20]_i_2_n_0\,
      CO(2) => \timer33_reg[20]_i_2_n_1\,
      CO(1) => \timer33_reg[20]_i_2_n_2\,
      CO(0) => \timer33_reg[20]_i_2_n_3\,
      CYINIT => '0',
      DI(3 downto 0) => B"0000",
      O(3 downto 0) => data0(20 downto 17),
      S(3 downto 0) => timer33(20 downto 17)
    );
\timer33_reg[21]\: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => '1',
      CLR => uart_hs_inst_n_0,
      D => timer33_2(21),
      Q => timer33(21)
    );
\timer33_reg[22]\: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => '1',
      CLR => uart_hs_inst_n_0,
      D => timer33_2(22),
      Q => timer33(22)
    );
\timer33_reg[23]\: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => '1',
      CLR => uart_hs_inst_n_0,
      D => timer33_2(23),
      Q => timer33(23)
    );
\timer33_reg[24]\: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => '1',
      CLR => uart_hs_inst_n_0,
      D => timer33_2(24),
      Q => timer33(24)
    );
\timer33_reg[24]_i_2\: unisim.vcomponents.CARRY4
     port map (
      CI => \timer33_reg[20]_i_2_n_0\,
      CO(3) => \timer33_reg[24]_i_2_n_0\,
      CO(2) => \timer33_reg[24]_i_2_n_1\,
      CO(1) => \timer33_reg[24]_i_2_n_2\,
      CO(0) => \timer33_reg[24]_i_2_n_3\,
      CYINIT => '0',
      DI(3 downto 0) => B"0000",
      O(3 downto 0) => data0(24 downto 21),
      S(3 downto 0) => timer33(24 downto 21)
    );
\timer33_reg[25]\: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => '1',
      CLR => uart_hs_inst_n_0,
      D => timer33_2(25),
      Q => timer33(25)
    );
\timer33_reg[26]\: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => '1',
      CLR => uart_hs_inst_n_0,
      D => timer33_2(26),
      Q => timer33(26)
    );
\timer33_reg[27]\: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => '1',
      CLR => uart_hs_inst_n_0,
      D => timer33_2(27),
      Q => timer33(27)
    );
\timer33_reg[28]\: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => '1',
      CLR => uart_hs_inst_n_0,
      D => timer33_2(28),
      Q => timer33(28)
    );
\timer33_reg[28]_i_2\: unisim.vcomponents.CARRY4
     port map (
      CI => \timer33_reg[24]_i_2_n_0\,
      CO(3) => \timer33_reg[28]_i_2_n_0\,
      CO(2) => \timer33_reg[28]_i_2_n_1\,
      CO(1) => \timer33_reg[28]_i_2_n_2\,
      CO(0) => \timer33_reg[28]_i_2_n_3\,
      CYINIT => '0',
      DI(3 downto 0) => B"0000",
      O(3 downto 0) => data0(28 downto 25),
      S(3 downto 0) => timer33(28 downto 25)
    );
\timer33_reg[29]\: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => '1',
      CLR => uart_hs_inst_n_0,
      D => timer33_2(29),
      Q => timer33(29)
    );
\timer33_reg[2]\: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => '1',
      CLR => uart_hs_inst_n_0,
      D => timer33_2(2),
      Q => timer33(2)
    );
\timer33_reg[30]\: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => '1',
      CLR => uart_hs_inst_n_0,
      D => timer33_2(30),
      Q => timer33(30)
    );
\timer33_reg[31]\: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => '1',
      CLR => uart_hs_inst_n_0,
      D => timer33_2(31),
      Q => timer33(31)
    );
\timer33_reg[31]_i_2\: unisim.vcomponents.CARRY4
     port map (
      CI => \timer33_reg[28]_i_2_n_0\,
      CO(3 downto 2) => \NLW_timer33_reg[31]_i_2_CO_UNCONNECTED\(3 downto 2),
      CO(1) => \timer33_reg[31]_i_2_n_2\,
      CO(0) => \timer33_reg[31]_i_2_n_3\,
      CYINIT => '0',
      DI(3 downto 0) => B"0000",
      O(3) => \NLW_timer33_reg[31]_i_2_O_UNCONNECTED\(3),
      O(2 downto 0) => data0(31 downto 29),
      S(3) => '0',
      S(2 downto 0) => timer33(31 downto 29)
    );
\timer33_reg[3]\: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => '1',
      CLR => uart_hs_inst_n_0,
      D => timer33_2(3),
      Q => timer33(3)
    );
\timer33_reg[4]\: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => '1',
      CLR => uart_hs_inst_n_0,
      D => timer33_2(4),
      Q => timer33(4)
    );
\timer33_reg[4]_i_2\: unisim.vcomponents.CARRY4
     port map (
      CI => '0',
      CO(3) => \timer33_reg[4]_i_2_n_0\,
      CO(2) => \timer33_reg[4]_i_2_n_1\,
      CO(1) => \timer33_reg[4]_i_2_n_2\,
      CO(0) => \timer33_reg[4]_i_2_n_3\,
      CYINIT => timer33(0),
      DI(3 downto 0) => B"0000",
      O(3 downto 0) => data0(4 downto 1),
      S(3 downto 0) => timer33(4 downto 1)
    );
\timer33_reg[5]\: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => '1',
      CLR => uart_hs_inst_n_0,
      D => timer33_2(5),
      Q => timer33(5)
    );
\timer33_reg[6]\: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => '1',
      CLR => uart_hs_inst_n_0,
      D => timer33_2(6),
      Q => timer33(6)
    );
\timer33_reg[7]\: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => '1',
      CLR => uart_hs_inst_n_0,
      D => timer33_2(7),
      Q => timer33(7)
    );
\timer33_reg[8]\: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => '1',
      CLR => uart_hs_inst_n_0,
      D => timer33_2(8),
      Q => timer33(8)
    );
\timer33_reg[8]_i_2\: unisim.vcomponents.CARRY4
     port map (
      CI => \timer33_reg[4]_i_2_n_0\,
      CO(3) => \timer33_reg[8]_i_2_n_0\,
      CO(2) => \timer33_reg[8]_i_2_n_1\,
      CO(1) => \timer33_reg[8]_i_2_n_2\,
      CO(0) => \timer33_reg[8]_i_2_n_3\,
      CYINIT => '0',
      DI(3 downto 0) => B"0000",
      O(3 downto 0) => data0(8 downto 5),
      S(3 downto 0) => timer33(8 downto 5)
    );
\timer33_reg[9]\: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => '1',
      CLR => uart_hs_inst_n_0,
      D => timer33_2(9),
      Q => timer33(9)
    );
\uart_data_in[0]_i_1\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"AEAEAEAAAAAEAAAA"
    )
        port map (
      I0 => \uart_data_in[0]_i_2_n_0\,
      I1 => command(0),
      I2 => \uart_data_in[7]_i_6_n_0\,
      I3 => command(1),
      I4 => \data_buff_reg_n_0_[8]\,
      I5 => \data_buff_reg_n_0_[24]\,
      O => \uart_data_in[0]_i_1_n_0\
    );
\uart_data_in[0]_i_2\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"FFFF541000005410"
    )
        port map (
      I0 => command(0),
      I1 => command(1),
      I2 => \data_buff_reg_n_0_[0]\,
      I3 => \data_buff_reg_n_0_[16]\,
      I4 => \uart_data_in[7]_i_6_n_0\,
      I5 => keyin(0),
      O => \uart_data_in[0]_i_2_n_0\
    );
\uart_data_in[1]_i_1\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"AEAEAEAAAAAEAAAA"
    )
        port map (
      I0 => \uart_data_in[1]_i_2_n_0\,
      I1 => command(0),
      I2 => \uart_data_in[7]_i_6_n_0\,
      I3 => command(1),
      I4 => \data_buff_reg_n_0_[9]\,
      I5 => \data_buff_reg_n_0_[25]\,
      O => \uart_data_in[1]_i_1_n_0\
    );
\uart_data_in[1]_i_2\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"FFFF541000005410"
    )
        port map (
      I0 => command(0),
      I1 => command(1),
      I2 => \data_buff_reg_n_0_[1]\,
      I3 => \data_buff_reg_n_0_[17]\,
      I4 => \uart_data_in[7]_i_6_n_0\,
      I5 => keyin(1),
      O => \uart_data_in[1]_i_2_n_0\
    );
\uart_data_in[2]_i_1\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"AEAEAEAAAAAEAAAA"
    )
        port map (
      I0 => \uart_data_in[2]_i_2_n_0\,
      I1 => command(0),
      I2 => \uart_data_in[7]_i_6_n_0\,
      I3 => command(1),
      I4 => \data_buff_reg_n_0_[10]\,
      I5 => \data_buff_reg_n_0_[26]\,
      O => \uart_data_in[2]_i_1_n_0\
    );
\uart_data_in[2]_i_2\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"FFFF541000005410"
    )
        port map (
      I0 => command(0),
      I1 => command(1),
      I2 => \data_buff_reg_n_0_[2]\,
      I3 => \data_buff_reg_n_0_[18]\,
      I4 => \uart_data_in[7]_i_6_n_0\,
      I5 => keyin(2),
      O => \uart_data_in[2]_i_2_n_0\
    );
\uart_data_in[3]_i_1\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"AEAEAEAAAAAEAAAA"
    )
        port map (
      I0 => \uart_data_in[3]_i_2_n_0\,
      I1 => command(0),
      I2 => \uart_data_in[7]_i_6_n_0\,
      I3 => command(1),
      I4 => \data_buff_reg_n_0_[11]\,
      I5 => \data_buff_reg_n_0_[27]\,
      O => \uart_data_in[3]_i_1_n_0\
    );
\uart_data_in[3]_i_2\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"FFFF000054105410"
    )
        port map (
      I0 => command(0),
      I1 => command(1),
      I2 => \data_buff_reg_n_0_[3]\,
      I3 => \data_buff_reg_n_0_[19]\,
      I4 => keyin(3),
      I5 => \uart_data_in[7]_i_6_n_0\,
      O => \uart_data_in[3]_i_2_n_0\
    );
\uart_data_in[4]_i_1\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"FFFFFFFF22200200"
    )
        port map (
      I0 => command(0),
      I1 => \uart_data_in[7]_i_6_n_0\,
      I2 => command(1),
      I3 => \data_buff_reg_n_0_[12]\,
      I4 => \data_buff_reg_n_0_[28]\,
      I5 => \uart_data_in[4]_i_2_n_0\,
      O => \uart_data_in[4]_i_1_n_0\
    );
\uart_data_in[4]_i_2\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"000000AC"
    )
        port map (
      I0 => \data_buff_reg_n_0_[20]\,
      I1 => \data_buff_reg_n_0_[4]\,
      I2 => command(1),
      I3 => \uart_data_in[7]_i_6_n_0\,
      I4 => command(0),
      O => \uart_data_in[4]_i_2_n_0\
    );
\uart_data_in[5]_i_1\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"FFFFFFFF22200200"
    )
        port map (
      I0 => command(0),
      I1 => \uart_data_in[7]_i_6_n_0\,
      I2 => command(1),
      I3 => \data_buff_reg_n_0_[13]\,
      I4 => \data_buff_reg_n_0_[29]\,
      I5 => \uart_data_in[5]_i_2_n_0\,
      O => \uart_data_in[5]_i_1_n_0\
    );
\uart_data_in[5]_i_2\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"000000AC"
    )
        port map (
      I0 => \data_buff_reg_n_0_[21]\,
      I1 => \data_buff_reg_n_0_[5]\,
      I2 => command(1),
      I3 => \uart_data_in[7]_i_6_n_0\,
      I4 => command(0),
      O => \uart_data_in[5]_i_2_n_0\
    );
\uart_data_in[6]_i_1\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"FFFFFFFF22200200"
    )
        port map (
      I0 => command(0),
      I1 => \uart_data_in[7]_i_6_n_0\,
      I2 => command(1),
      I3 => \data_buff_reg_n_0_[14]\,
      I4 => \data_buff_reg_n_0_[30]\,
      I5 => \uart_data_in[6]_i_2_n_0\,
      O => \uart_data_in[6]_i_1_n_0\
    );
\uart_data_in[6]_i_2\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"000000AC"
    )
        port map (
      I0 => \data_buff_reg_n_0_[22]\,
      I1 => \data_buff_reg_n_0_[6]\,
      I2 => command(1),
      I3 => \uart_data_in[7]_i_6_n_0\,
      I4 => command(0),
      O => \uart_data_in[6]_i_2_n_0\
    );
\uart_data_in[7]_i_1\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"44404040"
    )
        port map (
      I0 => command_done_reg_n_0,
      I1 => keyin(3),
      I2 => \uart_data_in[7]_i_3_n_0\,
      I3 => \uart_data_in[7]_i_4_n_0\,
      I4 => \uart_data_in[7]_i_5_n_0\,
      O => uart_data_in_0
    );
\uart_data_in[7]_i_2\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"FFFFFFFF22200200"
    )
        port map (
      I0 => command(0),
      I1 => \uart_data_in[7]_i_6_n_0\,
      I2 => command(1),
      I3 => \data_buff_reg_n_0_[15]\,
      I4 => \data_buff_reg_n_0_[31]\,
      I5 => \uart_data_in[7]_i_7_n_0\,
      O => \uart_data_in[7]_i_2_n_0\
    );
\uart_data_in[7]_i_3\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"00010000"
    )
        port map (
      I0 => \M00_AXI_wstrb[3]_i_2_n_0\,
      I1 => command(5),
      I2 => command(3),
      I3 => command(7),
      I4 => command(6),
      O => \uart_data_in[7]_i_3_n_0\
    );
\uart_data_in[7]_i_4\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"00000010"
    )
        port map (
      I0 => command(2),
      I1 => \addr[31]_i_3_n_0\,
      I2 => command(4),
      I3 => command(3),
      I4 => command(5),
      O => \uart_data_in[7]_i_4_n_0\
    );
\uart_data_in[7]_i_5\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"FFFFFFFFFFFFFFF4"
    )
        port map (
      I0 => command(5),
      I1 => command(3),
      I2 => command(4),
      I3 => command(1),
      I4 => command(2),
      I5 => command(0),
      O => \uart_data_in[7]_i_5_n_0\
    );
\uart_data_in[7]_i_6\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"FFFFFFFFFFFFFEFF"
    )
        port map (
      I0 => command(3),
      I1 => command(5),
      I2 => command(2),
      I3 => command(4),
      I4 => command(6),
      I5 => command(7),
      O => \uart_data_in[7]_i_6_n_0\
    );
\uart_data_in[7]_i_7\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"000000AC"
    )
        port map (
      I0 => \data_buff_reg_n_0_[23]\,
      I1 => \data_buff_reg_n_0_[7]\,
      I2 => command(1),
      I3 => \uart_data_in[7]_i_6_n_0\,
      I4 => command(0),
      O => \uart_data_in[7]_i_7_n_0\
    );
\uart_data_in_reg[0]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => uart_data_in_0,
      D => \uart_data_in[0]_i_1_n_0\,
      Q => uart_data_in(0),
      R => '0'
    );
\uart_data_in_reg[1]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => uart_data_in_0,
      D => \uart_data_in[1]_i_1_n_0\,
      Q => uart_data_in(1),
      R => '0'
    );
\uart_data_in_reg[2]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => uart_data_in_0,
      D => \uart_data_in[2]_i_1_n_0\,
      Q => uart_data_in(2),
      R => '0'
    );
\uart_data_in_reg[3]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => uart_data_in_0,
      D => \uart_data_in[3]_i_1_n_0\,
      Q => uart_data_in(3),
      R => '0'
    );
\uart_data_in_reg[4]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => uart_data_in_0,
      D => \uart_data_in[4]_i_1_n_0\,
      Q => uart_data_in(4),
      R => '0'
    );
\uart_data_in_reg[5]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => uart_data_in_0,
      D => \uart_data_in[5]_i_1_n_0\,
      Q => uart_data_in(5),
      R => '0'
    );
\uart_data_in_reg[6]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => uart_data_in_0,
      D => \uart_data_in[6]_i_1_n_0\,
      Q => uart_data_in(6),
      R => '0'
    );
\uart_data_in_reg[7]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => uart_data_in_0,
      D => \uart_data_in[7]_i_2_n_0\,
      Q => uart_data_in(7),
      R => '0'
    );
uart_hs_inst: entity work.design_1_test_0_0_uart_hs
     port map (
      D(7 downto 0) => uart_data_out(7 downto 0),
      E(0) => command_temp_1,
      Q(7 downto 0) => command_temp(7 downto 0),
      clk => clk,
      \clk_cnt_reg[1]\ => uart_hs_inst_n_0,
      command_done_reg => command_done_reg_n_0,
      \command_reg[0]\(0) => uart_hs_inst_n_11,
      \command_reg[7]\(7) => uart_hs_inst_n_13,
      \command_reg[7]\(6) => uart_hs_inst_n_14,
      \command_reg[7]\(5) => uart_hs_inst_n_15,
      \command_reg[7]\(4) => uart_hs_inst_n_16,
      \command_reg[7]\(3) => uart_hs_inst_n_17,
      \command_reg[7]\(2) => uart_hs_inst_n_18,
      \command_reg[7]\(1) => uart_hs_inst_n_19,
      \command_reg[7]\(0) => uart_hs_inst_n_20,
      data_cmd_reg => uart_hs_inst_n_21,
      data_cmd_reg_0 => data_cmd_reg_n_0,
      \data_reg[0]\(0) => data,
      keyin(0) => keyin(3),
      \uart_data_in_reg[7]\(7 downto 0) => uart_data_in(7 downto 0),
      uart_rxd => uart_rxd,
      uart_send => uart_send,
      uart_txd => uart_txd
    );
uart_send_i_1: unisim.vcomponents.LUT6
    generic map(
      INIT => X"00EAFFFF00EA0000"
    )
        port map (
      I0 => \uart_data_in[7]_i_3_n_0\,
      I1 => \uart_data_in[7]_i_4_n_0\,
      I2 => \uart_data_in[7]_i_5_n_0\,
      I3 => command_done_reg_n_0,
      I4 => keyin(3),
      I5 => uart_send,
      O => uart_send_i_1_n_0
    );
uart_send_reg: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => '1',
      D => uart_send_i_1_n_0,
      Q => uart_send,
      R => '0'
    );
end STRUCTURE;
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;
entity design_1_test_0_0 is
  port (
    clk : in STD_LOGIC;
    clk33 : in STD_LOGIC;
    led : out STD_LOGIC_VECTOR ( 3 downto 0 );
    keyin : in STD_LOGIC_VECTOR ( 3 downto 0 );
    uart_txd : out STD_LOGIC;
    uart_rxd : in STD_LOGIC;
    debug : out STD_LOGIC_VECTOR ( 7 downto 0 );
    M00_AXI_awaddr : out STD_LOGIC_VECTOR ( 31 downto 0 );
    M00_AXI_awlen : out STD_LOGIC_VECTOR ( 3 downto 0 );
    M00_AXI_awsize : out STD_LOGIC_VECTOR ( 2 downto 0 );
    M00_AXI_awburst : out STD_LOGIC_VECTOR ( 1 downto 0 );
    M00_AXI_awvalid : out STD_LOGIC;
    M00_AXI_awready : in STD_LOGIC;
    M00_AXI_araddr : out STD_LOGIC_VECTOR ( 31 downto 0 );
    M00_AXI_arlen : out STD_LOGIC_VECTOR ( 3 downto 0 );
    M00_AXI_arsize : out STD_LOGIC_VECTOR ( 2 downto 0 );
    M00_AXI_arburst : out STD_LOGIC_VECTOR ( 1 downto 0 );
    M00_AXI_arvalid : out STD_LOGIC;
    M00_AXI_arready : in STD_LOGIC;
    M00_AXI_rdata : in STD_LOGIC_VECTOR ( 31 downto 0 );
    M00_AXI_rresp : in STD_LOGIC_VECTOR ( 1 downto 0 );
    M00_AXI_rlast : in STD_LOGIC;
    M00_AXI_rvalid : in STD_LOGIC;
    M00_AXI_rready : out STD_LOGIC;
    M00_AXI_wdata : out STD_LOGIC_VECTOR ( 31 downto 0 );
    M00_AXI_wstrb : out STD_LOGIC_VECTOR ( 3 downto 0 );
    M00_AXI_wlast : out STD_LOGIC;
    M00_AXI_wvalid : out STD_LOGIC;
    M00_AXI_wready : in STD_LOGIC;
    M00_AXI_bresp : in STD_LOGIC_VECTOR ( 1 downto 0 );
    M00_AXI_bvalid : in STD_LOGIC;
    M00_AXI_bready : out STD_LOGIC
  );
  attribute NotValidForBitStream : boolean;
  attribute NotValidForBitStream of design_1_test_0_0 : entity is true;
  attribute CHECK_LICENSE_TYPE : string;
  attribute CHECK_LICENSE_TYPE of design_1_test_0_0 : entity is "design_1_test_0_0,test,{}";
  attribute DowngradeIPIdentifiedWarnings : string;
  attribute DowngradeIPIdentifiedWarnings of design_1_test_0_0 : entity is "yes";
  attribute X_CORE_INFO : string;
  attribute X_CORE_INFO of design_1_test_0_0 : entity is "test,Vivado 2018.1";
end design_1_test_0_0;

architecture STRUCTURE of design_1_test_0_0 is
  signal \<const0>\ : STD_LOGIC;
  signal \<const1>\ : STD_LOGIC;
  signal \^m00_axi_araddr\ : STD_LOGIC_VECTOR ( 31 downto 0 );
  signal \^m00_axi_arready\ : STD_LOGIC;
  signal \^m00_axi_awready\ : STD_LOGIC;
  signal \^m00_axi_bready\ : STD_LOGIC;
  signal \^m00_axi_bvalid\ : STD_LOGIC;
  signal \^m00_axi_rready\ : STD_LOGIC;
  signal \^m00_axi_rvalid\ : STD_LOGIC;
  signal \^m00_axi_wready\ : STD_LOGIC;
  signal \^debug\ : STD_LOGIC_VECTOR ( 6 downto 0 );
  signal \^keyin\ : STD_LOGIC_VECTOR ( 3 downto 0 );
  signal \^led\ : STD_LOGIC_VECTOR ( 2 to 2 );
  attribute X_INTERFACE_INFO : string;
  attribute X_INTERFACE_INFO of M00_AXI_arready : signal is "xilinx.com:interface:aximm:1.0 M00_AXI ARREADY";
  attribute X_INTERFACE_INFO of M00_AXI_arvalid : signal is "xilinx.com:interface:aximm:1.0 M00_AXI ARVALID";
  attribute X_INTERFACE_INFO of M00_AXI_awready : signal is "xilinx.com:interface:aximm:1.0 M00_AXI AWREADY";
  attribute X_INTERFACE_INFO of M00_AXI_awvalid : signal is "xilinx.com:interface:aximm:1.0 M00_AXI AWVALID";
  attribute X_INTERFACE_INFO of M00_AXI_bready : signal is "xilinx.com:interface:aximm:1.0 M00_AXI BREADY";
  attribute X_INTERFACE_PARAMETER : string;
  attribute X_INTERFACE_PARAMETER of M00_AXI_bready : signal is "XIL_INTERFACENAME M00_AXI, DATA_WIDTH 32, PROTOCOL AXI3, FREQ_HZ 50000000, ID_WIDTH 0, ADDR_WIDTH 32, AWUSER_WIDTH 0, ARUSER_WIDTH 0, WUSER_WIDTH 0, RUSER_WIDTH 0, BUSER_WIDTH 0, READ_WRITE_MODE READ_WRITE, HAS_BURST 1, HAS_LOCK 0, HAS_PROT 0, HAS_CACHE 0, HAS_QOS 0, HAS_REGION 0, HAS_WSTRB 1, HAS_BRESP 1, HAS_RRESP 1, SUPPORTS_NARROW_BURST 1, NUM_READ_OUTSTANDING 2, NUM_WRITE_OUTSTANDING 2, MAX_BURST_LENGTH 16, PHASE 0.000, CLK_DOMAIN design_1_processing_system7_0_0_FCLK_CLK0, NUM_READ_THREADS 1, NUM_WRITE_THREADS 1, RUSER_BITS_PER_BYTE 0, WUSER_BITS_PER_BYTE 0";
  attribute X_INTERFACE_INFO of M00_AXI_bvalid : signal is "xilinx.com:interface:aximm:1.0 M00_AXI BVALID";
  attribute X_INTERFACE_INFO of M00_AXI_rlast : signal is "xilinx.com:interface:aximm:1.0 M00_AXI RLAST";
  attribute X_INTERFACE_INFO of M00_AXI_rready : signal is "xilinx.com:interface:aximm:1.0 M00_AXI RREADY";
  attribute X_INTERFACE_INFO of M00_AXI_rvalid : signal is "xilinx.com:interface:aximm:1.0 M00_AXI RVALID";
  attribute X_INTERFACE_INFO of M00_AXI_wlast : signal is "xilinx.com:interface:aximm:1.0 M00_AXI WLAST";
  attribute X_INTERFACE_INFO of M00_AXI_wready : signal is "xilinx.com:interface:aximm:1.0 M00_AXI WREADY";
  attribute X_INTERFACE_INFO of M00_AXI_wvalid : signal is "xilinx.com:interface:aximm:1.0 M00_AXI WVALID";
  attribute X_INTERFACE_INFO of clk : signal is "xilinx.com:signal:clock:1.0 clk CLK";
  attribute X_INTERFACE_PARAMETER of clk : signal is "XIL_INTERFACENAME clk, ASSOCIATED_BUSIF M00_AXI, FREQ_HZ 50000000, PHASE 0.000, CLK_DOMAIN design_1_processing_system7_0_0_FCLK_CLK0";
  attribute X_INTERFACE_INFO of M00_AXI_araddr : signal is "xilinx.com:interface:aximm:1.0 M00_AXI ARADDR";
  attribute X_INTERFACE_INFO of M00_AXI_arburst : signal is "xilinx.com:interface:aximm:1.0 M00_AXI ARBURST";
  attribute X_INTERFACE_INFO of M00_AXI_arlen : signal is "xilinx.com:interface:aximm:1.0 M00_AXI ARLEN";
  attribute X_INTERFACE_INFO of M00_AXI_arsize : signal is "xilinx.com:interface:aximm:1.0 M00_AXI ARSIZE";
  attribute X_INTERFACE_INFO of M00_AXI_awaddr : signal is "xilinx.com:interface:aximm:1.0 M00_AXI AWADDR";
  attribute X_INTERFACE_INFO of M00_AXI_awburst : signal is "xilinx.com:interface:aximm:1.0 M00_AXI AWBURST";
  attribute X_INTERFACE_INFO of M00_AXI_awlen : signal is "xilinx.com:interface:aximm:1.0 M00_AXI AWLEN";
  attribute X_INTERFACE_INFO of M00_AXI_awsize : signal is "xilinx.com:interface:aximm:1.0 M00_AXI AWSIZE";
  attribute X_INTERFACE_INFO of M00_AXI_bresp : signal is "xilinx.com:interface:aximm:1.0 M00_AXI BRESP";
  attribute X_INTERFACE_INFO of M00_AXI_rdata : signal is "xilinx.com:interface:aximm:1.0 M00_AXI RDATA";
  attribute X_INTERFACE_INFO of M00_AXI_rresp : signal is "xilinx.com:interface:aximm:1.0 M00_AXI RRESP";
  attribute X_INTERFACE_INFO of M00_AXI_wdata : signal is "xilinx.com:interface:aximm:1.0 M00_AXI WDATA";
  attribute X_INTERFACE_INFO of M00_AXI_wstrb : signal is "xilinx.com:interface:aximm:1.0 M00_AXI WSTRB";
begin
  M00_AXI_araddr(31 downto 0) <= \^m00_axi_araddr\(31 downto 0);
  M00_AXI_arburst(1) <= \<const0>\;
  M00_AXI_arburst(0) <= \<const0>\;
  M00_AXI_arlen(3) <= \<const0>\;
  M00_AXI_arlen(2) <= \<const0>\;
  M00_AXI_arlen(1) <= \<const0>\;
  M00_AXI_arlen(0) <= \<const0>\;
  M00_AXI_arsize(2) <= \<const0>\;
  M00_AXI_arsize(1) <= \<const1>\;
  M00_AXI_arsize(0) <= \<const0>\;
  M00_AXI_arvalid <= \^debug\(0);
  M00_AXI_awaddr(31 downto 0) <= \^m00_axi_araddr\(31 downto 0);
  M00_AXI_awburst(1) <= \<const0>\;
  M00_AXI_awburst(0) <= \<const0>\;
  M00_AXI_awlen(3) <= \<const0>\;
  M00_AXI_awlen(2) <= \<const0>\;
  M00_AXI_awlen(1) <= \<const0>\;
  M00_AXI_awlen(0) <= \<const0>\;
  M00_AXI_awsize(2) <= \<const0>\;
  M00_AXI_awsize(1) <= \<const1>\;
  M00_AXI_awsize(0) <= \<const0>\;
  M00_AXI_awvalid <= \^debug\(4);
  M00_AXI_bready <= \^m00_axi_bready\;
  M00_AXI_rready <= \^m00_axi_rready\;
  M00_AXI_wvalid <= \^debug\(6);
  \^keyin\(3 downto 0) <= keyin(3 downto 0);
  \^m00_axi_arready\ <= M00_AXI_arready;
  \^m00_axi_awready\ <= M00_AXI_awready;
  \^m00_axi_bvalid\ <= M00_AXI_bvalid;
  \^m00_axi_rvalid\ <= M00_AXI_rvalid;
  \^m00_axi_wready\ <= M00_AXI_wready;
  debug(7) <= \^m00_axi_wready\;
  debug(6) <= \^debug\(6);
  debug(5) <= \^m00_axi_awready\;
  debug(4) <= \^debug\(4);
  debug(3) <= \^m00_axi_rready\;
  debug(2) <= \^m00_axi_rvalid\;
  debug(1) <= \^m00_axi_arready\;
  debug(0) <= \^debug\(0);
  led(3) <= \^keyin\(3);
  led(2) <= \^led\(2);
  led(1) <= \^m00_axi_bready\;
  led(0) <= \^m00_axi_bvalid\;
  M00_AXI_wlast <= 'Z';
GND: unisim.vcomponents.GND
     port map (
      G => \<const0>\
    );
VCC: unisim.vcomponents.VCC
     port map (
      P => \<const1>\
    );
inst: entity work.design_1_test_0_0_test
     port map (
      M00_AXI_araddr(31 downto 0) => \^m00_axi_araddr\(31 downto 0),
      M00_AXI_bvalid => \^m00_axi_bvalid\,
      M00_AXI_rdata(31 downto 0) => M00_AXI_rdata(31 downto 0),
      M00_AXI_rready => \^m00_axi_rready\,
      M00_AXI_wdata(31 downto 0) => M00_AXI_wdata(31 downto 0),
      M00_AXI_wstrb(3 downto 0) => M00_AXI_wstrb(3 downto 0),
      clk => clk,
      debug(3) => \^m00_axi_wready\,
      debug(2) => \^m00_axi_awready\,
      debug(1) => \^m00_axi_rvalid\,
      debug(0) => \^m00_axi_arready\,
      \debug[4]\ => \^debug\(4),
      \debug[6]\ => \^debug\(6),
      debug_0_sp_1 => \^debug\(0),
      keyin(3 downto 0) => \^keyin\(3 downto 0),
      led(1) => \^led\(2),
      led(0) => \^m00_axi_bready\,
      uart_rxd => uart_rxd,
      uart_txd => uart_txd
    );
end STRUCTURE;
