-- Copyright 1986-2018 Xilinx, Inc. All Rights Reserved.
-- --------------------------------------------------------------------------------
-- Tool Version: Vivado v.2018.1 (win64) Build 2188600 Wed Apr  4 18:40:38 MDT 2018
-- Date        : Sat Apr 25 16:34:09 2020
-- Host        : zsh-PC running 64-bit Service Pack 1  (build 7601)
-- Command     : write_vhdl -force -mode funcsim
--               E:/MyCode.github/fpga/v/project_1/project_1.srcs/sources_1/bd/design_1/ip/design_1_test_0_0/design_1_test_0_0_sim_netlist.vhdl
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
  signal \clk_cnt[7]_i_2_n_0\ : STD_LOGIC;
  signal \clk_cnt[7]_i_3_n_0\ : STD_LOGIC;
  signal \clk_cnt_reg__0\ : STD_LOGIC_VECTOR ( 7 downto 0 );
  signal \command_temp[7]_i_2_n_0\ : STD_LOGIC;
  signal \command_temp[7]_i_3_n_0\ : STD_LOGIC;
  signal \p_0_in__0\ : STD_LOGIC_VECTOR ( 7 downto 0 );
  signal p_7_in : STD_LOGIC_VECTOR ( 0 to 0 );
  signal rx_flag : STD_LOGIC;
  signal rx_flag_i_1_n_0 : STD_LOGIC;
  signal rx_flag_i_2_n_0 : STD_LOGIC;
  signal \uart_data_out[0]_i_1_n_0\ : STD_LOGIC;
  signal \uart_data_out[0]_i_2_n_0\ : STD_LOGIC;
  signal \uart_data_out[1]_i_1_n_0\ : STD_LOGIC;
  signal \uart_data_out[1]_i_2_n_0\ : STD_LOGIC;
  signal \uart_data_out[2]_i_1_n_0\ : STD_LOGIC;
  signal \uart_data_out[2]_i_2_n_0\ : STD_LOGIC;
  signal \uart_data_out[3]_i_1_n_0\ : STD_LOGIC;
  signal \uart_data_out[3]_i_2_n_0\ : STD_LOGIC;
  signal \uart_data_out[4]_i_1_n_0\ : STD_LOGIC;
  signal \uart_data_out[4]_i_2_n_0\ : STD_LOGIC;
  signal \uart_data_out[4]_i_3_n_0\ : STD_LOGIC;
  signal \uart_data_out[5]_i_1_n_0\ : STD_LOGIC;
  signal \uart_data_out[5]_i_2_n_0\ : STD_LOGIC;
  signal \uart_data_out[5]_i_3_n_0\ : STD_LOGIC;
  signal \uart_data_out[6]_i_1_n_0\ : STD_LOGIC;
  signal \uart_data_out[6]_i_2_n_0\ : STD_LOGIC;
  signal \uart_data_out[7]_i_1_n_0\ : STD_LOGIC;
  signal \uart_data_out[7]_i_3_n_0\ : STD_LOGIC;
  signal uart_rec : STD_LOGIC;
  signal uart_rec0_out : STD_LOGIC;
  signal uart_rec_i_1_n_0 : STD_LOGIC;
  signal uart_rec_i_2_n_0 : STD_LOGIC;
  signal uart_rec_i_3_n_0 : STD_LOGIC;
  signal uart_rec_i_4_n_0 : STD_LOGIC;
  signal uart_rec_i_5_n_0 : STD_LOGIC;
  signal uart_rxd_last1 : STD_LOGIC;
  attribute SOFT_HLUTNM : string;
  attribute SOFT_HLUTNM of \clk_cnt[0]_i_1\ : label is "soft_lutpair1";
  attribute SOFT_HLUTNM of \clk_cnt[1]_i_1__0\ : label is "soft_lutpair5";
  attribute SOFT_HLUTNM of \clk_cnt[2]_i_1__0\ : label is "soft_lutpair0";
  attribute SOFT_HLUTNM of \clk_cnt[3]_i_1__0\ : label is "soft_lutpair0";
  attribute SOFT_HLUTNM of \clk_cnt[5]_i_1\ : label is "soft_lutpair2";
  attribute SOFT_HLUTNM of \clk_cnt[7]_i_2\ : label is "soft_lutpair2";
  attribute SOFT_HLUTNM of \clk_cnt[7]_i_3\ : label is "soft_lutpair5";
  attribute SOFT_HLUTNM of \command[0]_i_1\ : label is "soft_lutpair9";
  attribute SOFT_HLUTNM of \command[1]_i_1\ : label is "soft_lutpair8";
  attribute SOFT_HLUTNM of \command[2]_i_1\ : label is "soft_lutpair11";
  attribute SOFT_HLUTNM of \command[3]_i_1\ : label is "soft_lutpair10";
  attribute SOFT_HLUTNM of \command[4]_i_1\ : label is "soft_lutpair11";
  attribute SOFT_HLUTNM of \command[5]_i_1\ : label is "soft_lutpair10";
  attribute SOFT_HLUTNM of \command[6]_i_1\ : label is "soft_lutpair9";
  attribute SOFT_HLUTNM of \command[7]_i_1\ : label is "soft_lutpair6";
  attribute SOFT_HLUTNM of \command[7]_i_2\ : label is "soft_lutpair8";
  attribute SOFT_HLUTNM of \command_temp[7]_i_1\ : label is "soft_lutpair4";
  attribute SOFT_HLUTNM of \data[7]_i_1\ : label is "soft_lutpair6";
  attribute SOFT_HLUTNM of data_cmd_i_1 : label is "soft_lutpair4";
  attribute SOFT_HLUTNM of \uart_data_out[1]_i_1\ : label is "soft_lutpair1";
  attribute SOFT_HLUTNM of \uart_data_out[4]_i_1\ : label is "soft_lutpair3";
  attribute SOFT_HLUTNM of \uart_data_out[4]_i_3\ : label is "soft_lutpair7";
  attribute SOFT_HLUTNM of \uart_data_out[5]_i_3\ : label is "soft_lutpair3";
  attribute SOFT_HLUTNM of \uart_data_out[7]_i_3\ : label is "soft_lutpair7";
begin
  D(7 downto 0) <= \^d\(7 downto 0);
\clk_cnt[0]_i_1\: unisim.vcomponents.LUT2
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
      INIT => X"60"
    )
        port map (
      I0 => \clk_cnt_reg__0\(1),
      I1 => \clk_cnt_reg__0\(0),
      I2 => rx_flag,
      O => \p_0_in__0\(1)
    );
\clk_cnt[2]_i_1__0\: unisim.vcomponents.LUT4
    generic map(
      INIT => X"6A00"
    )
        port map (
      I0 => \clk_cnt_reg__0\(2),
      I1 => \clk_cnt_reg__0\(0),
      I2 => \clk_cnt_reg__0\(1),
      I3 => rx_flag,
      O => \p_0_in__0\(2)
    );
\clk_cnt[3]_i_1__0\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"6AAA0000"
    )
        port map (
      I0 => \clk_cnt_reg__0\(3),
      I1 => \clk_cnt_reg__0\(1),
      I2 => \clk_cnt_reg__0\(0),
      I3 => \clk_cnt_reg__0\(2),
      I4 => rx_flag,
      O => \p_0_in__0\(3)
    );
\clk_cnt[4]_i_1__0\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"6AAAAAAA00000000"
    )
        port map (
      I0 => \clk_cnt_reg__0\(4),
      I1 => \clk_cnt_reg__0\(3),
      I2 => \clk_cnt_reg__0\(0),
      I3 => \clk_cnt_reg__0\(1),
      I4 => \clk_cnt_reg__0\(2),
      I5 => rx_flag,
      O => \p_0_in__0\(4)
    );
\clk_cnt[5]_i_1\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"6AAA0000"
    )
        port map (
      I0 => \clk_cnt_reg__0\(5),
      I1 => \clk_cnt[7]_i_3_n_0\,
      I2 => \clk_cnt_reg__0\(4),
      I3 => \clk_cnt_reg__0\(2),
      I4 => rx_flag,
      O => \p_0_in__0\(5)
    );
\clk_cnt[6]_i_1\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"6AAAAAAA00000000"
    )
        port map (
      I0 => \clk_cnt_reg__0\(6),
      I1 => \clk_cnt_reg__0\(5),
      I2 => \clk_cnt_reg__0\(2),
      I3 => \clk_cnt_reg__0\(4),
      I4 => \clk_cnt[7]_i_3_n_0\,
      I5 => rx_flag,
      O => \p_0_in__0\(6)
    );
\clk_cnt[7]_i_1\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"6AAAAAAA00000000"
    )
        port map (
      I0 => \clk_cnt_reg__0\(7),
      I1 => \clk_cnt_reg__0\(5),
      I2 => \clk_cnt[7]_i_2_n_0\,
      I3 => \clk_cnt_reg__0\(6),
      I4 => \clk_cnt[7]_i_3_n_0\,
      I5 => rx_flag,
      O => \p_0_in__0\(7)
    );
\clk_cnt[7]_i_2\: unisim.vcomponents.LUT2
    generic map(
      INIT => X"8"
    )
        port map (
      I0 => \clk_cnt_reg__0\(2),
      I1 => \clk_cnt_reg__0\(4),
      O => \clk_cnt[7]_i_2_n_0\
    );
\clk_cnt[7]_i_3\: unisim.vcomponents.LUT3
    generic map(
      INIT => X"80"
    )
        port map (
      I0 => \clk_cnt_reg__0\(1),
      I1 => \clk_cnt_reg__0\(0),
      I2 => \clk_cnt_reg__0\(3),
      O => \clk_cnt[7]_i_3_n_0\
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
      I0 => Q(0),
      I1 => uart_rec,
      O => \command_reg[7]\(0)
    );
\command[1]_i_1\: unisim.vcomponents.LUT2
    generic map(
      INIT => X"8"
    )
        port map (
      I0 => Q(1),
      I1 => uart_rec,
      O => \command_reg[7]\(1)
    );
\command[2]_i_1\: unisim.vcomponents.LUT2
    generic map(
      INIT => X"8"
    )
        port map (
      I0 => Q(2),
      I1 => uart_rec,
      O => \command_reg[7]\(2)
    );
\command[3]_i_1\: unisim.vcomponents.LUT2
    generic map(
      INIT => X"8"
    )
        port map (
      I0 => Q(3),
      I1 => uart_rec,
      O => \command_reg[7]\(3)
    );
\command[4]_i_1\: unisim.vcomponents.LUT2
    generic map(
      INIT => X"8"
    )
        port map (
      I0 => Q(4),
      I1 => uart_rec,
      O => \command_reg[7]\(4)
    );
\command[5]_i_1\: unisim.vcomponents.LUT2
    generic map(
      INIT => X"8"
    )
        port map (
      I0 => Q(5),
      I1 => uart_rec,
      O => \command_reg[7]\(5)
    );
\command[6]_i_1\: unisim.vcomponents.LUT2
    generic map(
      INIT => X"8"
    )
        port map (
      I0 => Q(6),
      I1 => uart_rec,
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
      I0 => Q(7),
      I1 => uart_rec,
      O => \command_reg[7]\(7)
    );
\command_temp[7]_i_1\: unisim.vcomponents.LUT4
    generic map(
      INIT => X"2000"
    )
        port map (
      I0 => \command_temp[7]_i_2_n_0\,
      I1 => data_cmd_reg_0,
      I2 => uart_rec,
      I3 => keyin(0),
      O => E(0)
    );
\command_temp[7]_i_2\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"FFFFFFFE"
    )
        port map (
      I0 => \^d\(2),
      I1 => \^d\(3),
      I2 => \^d\(0),
      I3 => \^d\(1),
      I4 => \command_temp[7]_i_3_n_0\,
      O => \command_temp[7]_i_2_n_0\
    );
\command_temp[7]_i_3\: unisim.vcomponents.LUT4
    generic map(
      INIT => X"FFFE"
    )
        port map (
      I0 => \^d\(5),
      I1 => \^d\(4),
      I2 => \^d\(7),
      I3 => \^d\(6),
      O => \command_temp[7]_i_3_n_0\
    );
\data[7]_i_1\: unisim.vcomponents.LUT2
    generic map(
      INIT => X"8"
    )
        port map (
      I0 => data_cmd_reg_0,
      I1 => uart_rec,
      O => \data_reg[0]\(0)
    );
data_cmd_i_1: unisim.vcomponents.LUT4
    generic map(
      INIT => X"2CCC"
    )
        port map (
      I0 => \command_temp[7]_i_2_n_0\,
      I1 => data_cmd_reg_0,
      I2 => uart_rec,
      I3 => keyin(0),
      O => data_cmd_reg
    );
rx_flag_i_1: unisim.vcomponents.LUT6
    generic map(
      INIT => X"BFFFBFBF00FF0000"
    )
        port map (
      I0 => \clk_cnt_reg__0\(4),
      I1 => \clk_cnt_reg__0\(3),
      I2 => rx_flag_i_2_n_0,
      I3 => uart_rxd_last1,
      I4 => p_7_in(0),
      I5 => rx_flag,
      O => rx_flag_i_1_n_0
    );
rx_flag_i_2: unisim.vcomponents.LUT6
    generic map(
      INIT => X"0080000000000000"
    )
        port map (
      I0 => \clk_cnt_reg__0\(2),
      I1 => \clk_cnt_reg__0\(5),
      I2 => \clk_cnt_reg__0\(0),
      I3 => \clk_cnt_reg__0\(1),
      I4 => \clk_cnt_reg__0\(7),
      I5 => \clk_cnt_reg__0\(6),
      O => rx_flag_i_2_n_0
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
      INIT => X"FFFBFFFF00080000"
    )
        port map (
      I0 => p_7_in(0),
      I1 => \clk_cnt_reg__0\(0),
      I2 => \clk_cnt_reg__0\(1),
      I3 => \clk_cnt_reg__0\(3),
      I4 => \uart_data_out[0]_i_2_n_0\,
      I5 => \^d\(0),
      O => \uart_data_out[0]_i_1_n_0\
    );
\uart_data_out[0]_i_2\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"0000002000000000"
    )
        port map (
      I0 => \clk_cnt_reg__0\(5),
      I1 => \clk_cnt_reg__0\(6),
      I2 => \clk_cnt_reg__0\(2),
      I3 => \clk_cnt_reg__0\(4),
      I4 => \clk_cnt_reg__0\(7),
      I5 => rx_flag,
      O => \uart_data_out[0]_i_2_n_0\
    );
\uart_data_out[1]_i_1\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"FBFF0800"
    )
        port map (
      I0 => p_7_in(0),
      I1 => \uart_data_out[1]_i_2_n_0\,
      I2 => \clk_cnt_reg__0\(0),
      I3 => rx_flag,
      I4 => \^d\(1),
      O => \uart_data_out[1]_i_1_n_0\
    );
\uart_data_out[1]_i_2\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"1000000000000000"
    )
        port map (
      I0 => \clk_cnt_reg__0\(6),
      I1 => \clk_cnt_reg__0\(7),
      I2 => \clk_cnt[7]_i_2_n_0\,
      I3 => \clk_cnt_reg__0\(1),
      I4 => \clk_cnt_reg__0\(3),
      I5 => \clk_cnt_reg__0\(5),
      O => \uart_data_out[1]_i_2_n_0\
    );
\uart_data_out[2]_i_1\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"BFFFFFFF80000000"
    )
        port map (
      I0 => p_7_in(0),
      I1 => \uart_data_out[2]_i_2_n_0\,
      I2 => \clk_cnt_reg__0\(6),
      I3 => \clk_cnt_reg__0\(2),
      I4 => \clk_cnt_reg__0\(4),
      I5 => \^d\(2),
      O => \uart_data_out[2]_i_1_n_0\
    );
\uart_data_out[2]_i_2\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"0002000000000000"
    )
        port map (
      I0 => rx_flag,
      I1 => \clk_cnt_reg__0\(7),
      I2 => \clk_cnt_reg__0\(3),
      I3 => \clk_cnt_reg__0\(5),
      I4 => \clk_cnt_reg__0\(1),
      I5 => \clk_cnt_reg__0\(0),
      O => \uart_data_out[2]_i_2_n_0\
    );
\uart_data_out[3]_i_1\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"FFFB0008"
    )
        port map (
      I0 => p_7_in(0),
      I1 => \uart_data_out[3]_i_2_n_0\,
      I2 => \clk_cnt_reg__0\(3),
      I3 => \clk_cnt_reg__0\(1),
      I4 => \^d\(3),
      O => \uart_data_out[3]_i_1_n_0\
    );
\uart_data_out[3]_i_2\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"0020000000000000"
    )
        port map (
      I0 => \clk_cnt_reg__0\(6),
      I1 => \clk_cnt_reg__0\(7),
      I2 => \p_0_in__0\(0),
      I3 => \clk_cnt_reg__0\(2),
      I4 => \clk_cnt_reg__0\(4),
      I5 => \clk_cnt_reg__0\(5),
      O => \uart_data_out[3]_i_2_n_0\
    );
\uart_data_out[4]_i_1\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"FFFB0008"
    )
        port map (
      I0 => p_7_in(0),
      I1 => \uart_data_out[4]_i_2_n_0\,
      I2 => \clk_cnt_reg__0\(6),
      I3 => \clk_cnt_reg__0\(2),
      I4 => \^d\(4),
      O => \uart_data_out[4]_i_1_n_0\
    );
\uart_data_out[4]_i_2\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"0000000000004000"
    )
        port map (
      I0 => \uart_data_out[4]_i_3_n_0\,
      I1 => \clk_cnt_reg__0\(7),
      I2 => rx_flag,
      I3 => \clk_cnt_reg__0\(0),
      I4 => \clk_cnt_reg__0\(1),
      I5 => \clk_cnt_reg__0\(5),
      O => \uart_data_out[4]_i_2_n_0\
    );
\uart_data_out[4]_i_3\: unisim.vcomponents.LUT2
    generic map(
      INIT => X"B"
    )
        port map (
      I0 => \clk_cnt_reg__0\(4),
      I1 => \clk_cnt_reg__0\(3),
      O => \uart_data_out[4]_i_3_n_0\
    );
\uart_data_out[5]_i_1\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"FBFF0800"
    )
        port map (
      I0 => p_7_in(0),
      I1 => \uart_data_out[5]_i_2_n_0\,
      I2 => \clk_cnt_reg__0\(0),
      I3 => rx_flag,
      I4 => \^d\(5),
      O => \uart_data_out[5]_i_1_n_0\
    );
\uart_data_out[5]_i_2\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"0000000000004000"
    )
        port map (
      I0 => \uart_data_out[5]_i_3_n_0\,
      I1 => \clk_cnt_reg__0\(5),
      I2 => \clk_cnt_reg__0\(7),
      I3 => \clk_cnt_reg__0\(1),
      I4 => \clk_cnt_reg__0\(3),
      I5 => \clk_cnt_reg__0\(4),
      O => \uart_data_out[5]_i_2_n_0\
    );
\uart_data_out[5]_i_3\: unisim.vcomponents.LUT2
    generic map(
      INIT => X"E"
    )
        port map (
      I0 => \clk_cnt_reg__0\(2),
      I1 => \clk_cnt_reg__0\(6),
      O => \uart_data_out[5]_i_3_n_0\
    );
\uart_data_out[6]_i_1\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"BFFFFFFF80000000"
    )
        port map (
      I0 => p_7_in(0),
      I1 => \uart_data_out[6]_i_2_n_0\,
      I2 => \clk_cnt_reg__0\(3),
      I3 => \clk_cnt_reg__0\(0),
      I4 => \clk_cnt_reg__0\(1),
      I5 => \^d\(6),
      O => \uart_data_out[6]_i_1_n_0\
    );
\uart_data_out[6]_i_2\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"0000000000008000"
    )
        port map (
      I0 => \clk_cnt_reg__0\(7),
      I1 => rx_flag,
      I2 => \clk_cnt_reg__0\(4),
      I3 => \clk_cnt_reg__0\(5),
      I4 => \clk_cnt_reg__0\(6),
      I5 => \clk_cnt_reg__0\(2),
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
      INIT => X"0100000000000000"
    )
        port map (
      I0 => \clk_cnt_reg__0\(1),
      I1 => \clk_cnt_reg__0\(3),
      I2 => \clk_cnt_reg__0\(5),
      I3 => \clk_cnt_reg__0\(7),
      I4 => \p_0_in__0\(0),
      I5 => \uart_data_out[7]_i_3_n_0\,
      O => uart_rec0_out
    );
\uart_data_out[7]_i_3\: unisim.vcomponents.LUT3
    generic map(
      INIT => X"80"
    )
        port map (
      I0 => \clk_cnt_reg__0\(4),
      I1 => \clk_cnt_reg__0\(2),
      I2 => \clk_cnt_reg__0\(6),
      O => \uart_data_out[7]_i_3_n_0\
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
uart_rec_i_1: unisim.vcomponents.LUT6
    generic map(
      INIT => X"AAAAAAABAAAAAAA8"
    )
        port map (
      I0 => uart_rec0_out,
      I1 => uart_rec_i_2_n_0,
      I2 => uart_rec_i_3_n_0,
      I3 => uart_rec_i_4_n_0,
      I4 => uart_rec_i_5_n_0,
      I5 => uart_rec,
      O => uart_rec_i_1_n_0
    );
uart_rec_i_2: unisim.vcomponents.LUT6
    generic map(
      INIT => X"FFF40000040F0000"
    )
        port map (
      I0 => \clk_cnt_reg__0\(5),
      I1 => \uart_data_out[4]_i_3_n_0\,
      I2 => \clk_cnt_reg__0\(2),
      I3 => \clk_cnt_reg__0\(6),
      I4 => keyin(0),
      I5 => \clk_cnt_reg__0\(7),
      O => uart_rec_i_2_n_0
    );
uart_rec_i_3: unisim.vcomponents.LUT6
    generic map(
      INIT => X"AA005500FE00AA00"
    )
        port map (
      I0 => \clk_cnt_reg__0\(6),
      I1 => \clk_cnt_reg__0\(5),
      I2 => \clk_cnt_reg__0\(2),
      I3 => keyin(0),
      I4 => \clk_cnt_reg__0\(3),
      I5 => \clk_cnt_reg__0\(4),
      O => uart_rec_i_3_n_0
    );
uart_rec_i_4: unisim.vcomponents.LUT6
    generic map(
      INIT => X"5D55D5DD00000000"
    )
        port map (
      I0 => rx_flag,
      I1 => \clk_cnt_reg__0\(2),
      I2 => \clk_cnt_reg__0\(3),
      I3 => \clk_cnt_reg__0\(4),
      I4 => \clk_cnt_reg__0\(5),
      I5 => keyin(0),
      O => uart_rec_i_4_n_0
    );
uart_rec_i_5: unisim.vcomponents.LUT6
    generic map(
      INIT => X"F05060F060F0F090"
    )
        port map (
      I0 => \clk_cnt_reg__0\(1),
      I1 => \clk_cnt_reg__0\(4),
      I2 => keyin(0),
      I3 => \clk_cnt_reg__0\(0),
      I4 => \clk_cnt_reg__0\(3),
      I5 => \clk_cnt_reg__0\(2),
      O => uart_rec_i_5_n_0
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
  signal \clk_cnt[0]_i_1__0_n_0\ : STD_LOGIC;
  signal \clk_cnt[7]_i_2__0_n_0\ : STD_LOGIC;
  signal \^clk_cnt_reg[1]_0\ : STD_LOGIC;
  signal \clk_cnt_reg__0\ : STD_LOGIC_VECTOR ( 7 downto 0 );
  signal data1 : STD_LOGIC;
  signal data2 : STD_LOGIC;
  signal data3 : STD_LOGIC;
  signal data4 : STD_LOGIC;
  signal data5 : STD_LOGIC;
  signal data6 : STD_LOGIC;
  signal data7 : STD_LOGIC;
  signal p_0_in : STD_LOGIC_VECTOR ( 7 downto 1 );
  signal \tx_data[7]_i_1_n_0\ : STD_LOGIC;
  signal \tx_data_reg_n_0_[0]\ : STD_LOGIC;
  signal tx_flag_i_1_n_0 : STD_LOGIC;
  signal tx_flag_i_2_n_0 : STD_LOGIC;
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
  attribute SOFT_HLUTNM of \clk_cnt[0]_i_1__0\ : label is "soft_lutpair16";
  attribute SOFT_HLUTNM of \clk_cnt[1]_i_1\ : label is "soft_lutpair15";
  attribute SOFT_HLUTNM of \clk_cnt[2]_i_1\ : label is "soft_lutpair15";
  attribute SOFT_HLUTNM of \clk_cnt[3]_i_1\ : label is "soft_lutpair12";
  attribute SOFT_HLUTNM of \clk_cnt[5]_i_1__0\ : label is "soft_lutpair14";
  attribute SOFT_HLUTNM of \clk_cnt[6]_i_1__0\ : label is "soft_lutpair14";
  attribute SOFT_HLUTNM of \clk_cnt[7]_i_2__0\ : label is "soft_lutpair12";
  attribute SOFT_HLUTNM of uart_txd_i_14 : label is "soft_lutpair13";
  attribute SOFT_HLUTNM of uart_txd_i_3 : label is "soft_lutpair16";
  attribute SOFT_HLUTNM of uart_txd_i_8 : label is "soft_lutpair13";
begin
  \clk_cnt_reg[1]_0\ <= \^clk_cnt_reg[1]_0\;
  uart_txd <= \^uart_txd\;
\clk_cnt[0]_i_1__0\: unisim.vcomponents.LUT2
    generic map(
      INIT => X"2"
    )
        port map (
      I0 => tx_flag_reg_n_0,
      I1 => \clk_cnt_reg__0\(0),
      O => \clk_cnt[0]_i_1__0_n_0\
    );
\clk_cnt[1]_i_1\: unisim.vcomponents.LUT3
    generic map(
      INIT => X"60"
    )
        port map (
      I0 => \clk_cnt_reg__0\(1),
      I1 => \clk_cnt_reg__0\(0),
      I2 => tx_flag_reg_n_0,
      O => p_0_in(1)
    );
\clk_cnt[2]_i_1\: unisim.vcomponents.LUT4
    generic map(
      INIT => X"6A00"
    )
        port map (
      I0 => \clk_cnt_reg__0\(2),
      I1 => \clk_cnt_reg__0\(0),
      I2 => \clk_cnt_reg__0\(1),
      I3 => tx_flag_reg_n_0,
      O => p_0_in(2)
    );
\clk_cnt[3]_i_1\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"6AAA0000"
    )
        port map (
      I0 => \clk_cnt_reg__0\(3),
      I1 => \clk_cnt_reg__0\(1),
      I2 => \clk_cnt_reg__0\(0),
      I3 => \clk_cnt_reg__0\(2),
      I4 => tx_flag_reg_n_0,
      O => p_0_in(3)
    );
\clk_cnt[4]_i_1\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"6AAAAAAA00000000"
    )
        port map (
      I0 => \clk_cnt_reg__0\(4),
      I1 => \clk_cnt_reg__0\(3),
      I2 => \clk_cnt_reg__0\(2),
      I3 => \clk_cnt_reg__0\(1),
      I4 => \clk_cnt_reg__0\(0),
      I5 => tx_flag_reg_n_0,
      O => p_0_in(4)
    );
\clk_cnt[5]_i_1__0\: unisim.vcomponents.LUT4
    generic map(
      INIT => X"6A00"
    )
        port map (
      I0 => \clk_cnt_reg__0\(5),
      I1 => \clk_cnt[7]_i_2__0_n_0\,
      I2 => \clk_cnt_reg__0\(4),
      I3 => tx_flag_reg_n_0,
      O => p_0_in(5)
    );
\clk_cnt[6]_i_1__0\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"6AAA0000"
    )
        port map (
      I0 => \clk_cnt_reg__0\(6),
      I1 => \clk_cnt_reg__0\(5),
      I2 => \clk_cnt_reg__0\(4),
      I3 => \clk_cnt[7]_i_2__0_n_0\,
      I4 => tx_flag_reg_n_0,
      O => p_0_in(6)
    );
\clk_cnt[7]_i_1__0\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"6AAAAAAA00000000"
    )
        port map (
      I0 => \clk_cnt_reg__0\(7),
      I1 => \clk_cnt_reg__0\(4),
      I2 => \clk_cnt_reg__0\(5),
      I3 => \clk_cnt_reg__0\(6),
      I4 => \clk_cnt[7]_i_2__0_n_0\,
      I5 => tx_flag_reg_n_0,
      O => p_0_in(7)
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
      CLR => \^clk_cnt_reg[1]_0\,
      D => \clk_cnt[0]_i_1__0_n_0\,
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
      INIT => X"7FFF7F7F00FF0000"
    )
        port map (
      I0 => \clk_cnt_reg__0\(2),
      I1 => \clk_cnt_reg__0\(3),
      I2 => tx_flag_i_2_n_0,
      I3 => uart_send_last1,
      I4 => uart_send_last2,
      I5 => tx_flag_reg_n_0,
      O => tx_flag_i_1_n_0
    );
tx_flag_i_2: unisim.vcomponents.LUT6
    generic map(
      INIT => X"0020000000000000"
    )
        port map (
      I0 => \clk_cnt_reg__0\(5),
      I1 => \clk_cnt_reg__0\(4),
      I2 => \clk_cnt_reg__0\(0),
      I3 => \clk_cnt_reg__0\(1),
      I4 => \clk_cnt_reg__0\(7),
      I5 => \clk_cnt_reg__0\(6),
      O => tx_flag_i_2_n_0
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
uart_txd_i_10: unisim.vcomponents.LUT5
    generic map(
      INIT => X"B8BBB888"
    )
        port map (
      I0 => data4,
      I1 => \clk_cnt_reg__0\(2),
      I2 => data1,
      I3 => \clk_cnt_reg__0\(1),
      I4 => \tx_data_reg_n_0_[0]\,
      O => uart_txd_i_10_n_0
    );
uart_txd_i_11: unisim.vcomponents.LUT6
    generic map(
      INIT => X"004000800C300000"
    )
        port map (
      I0 => \clk_cnt_reg__0\(6),
      I1 => \clk_cnt_reg__0\(0),
      I2 => \clk_cnt_reg__0\(3),
      I3 => \clk_cnt_reg__0\(5),
      I4 => \clk_cnt_reg__0\(7),
      I5 => \clk_cnt_reg__0\(1),
      O => uart_txd_i_11_n_0
    );
uart_txd_i_12: unisim.vcomponents.LUT5
    generic map(
      INIT => X"10000001"
    )
        port map (
      I0 => \clk_cnt_reg__0\(7),
      I1 => \clk_cnt_reg__0\(1),
      I2 => \clk_cnt_reg__0\(6),
      I3 => \clk_cnt_reg__0\(5),
      I4 => \clk_cnt_reg__0\(2),
      O => uart_txd_i_12_n_0
    );
uart_txd_i_13: unisim.vcomponents.LUT4
    generic map(
      INIT => X"0400"
    )
        port map (
      I0 => \clk_cnt_reg__0\(4),
      I1 => \clk_cnt_reg__0\(7),
      I2 => \clk_cnt_reg__0\(6),
      I3 => \clk_cnt_reg__0\(5),
      O => uart_txd_i_13_n_0
    );
uart_txd_i_14: unisim.vcomponents.LUT3
    generic map(
      INIT => X"24"
    )
        port map (
      I0 => \clk_cnt_reg__0\(2),
      I1 => \clk_cnt_reg__0\(5),
      I2 => \clk_cnt_reg__0\(7),
      O => uart_txd_i_14_n_0
    );
uart_txd_i_15: unisim.vcomponents.LUT5
    generic map(
      INIT => X"00040000"
    )
        port map (
      I0 => \clk_cnt_reg__0\(0),
      I1 => \clk_cnt_reg__0\(1),
      I2 => \clk_cnt_reg__0\(3),
      I3 => \clk_cnt_reg__0\(6),
      I4 => \clk_cnt_reg__0\(4),
      O => uart_txd_i_15_n_0
    );
uart_txd_i_2: unisim.vcomponents.LUT6
    generic map(
      INIT => X"FEFFFEFEFEFEFEFE"
    )
        port map (
      I0 => uart_txd_i_7_n_0,
      I1 => uart_txd_i_8_n_0,
      I2 => uart_txd_i_9_n_0,
      I3 => \clk_cnt_reg__0\(7),
      I4 => \clk_cnt_reg__0\(4),
      I5 => uart_txd_i_10_n_0,
      O => uart_txd_i_2_n_0
    );
uart_txd_i_3: unisim.vcomponents.LUT4
    generic map(
      INIT => X"80FF"
    )
        port map (
      I0 => data5,
      I1 => \clk_cnt_reg__0\(4),
      I2 => \clk_cnt_reg__0\(7),
      I3 => tx_flag_reg_n_0,
      O => uart_txd_i_3_n_0
    );
uart_txd_i_4: unisim.vcomponents.LUT4
    generic map(
      INIT => X"0200"
    )
        port map (
      I0 => \clk_cnt_reg__0\(6),
      I1 => \clk_cnt_reg__0\(2),
      I2 => \clk_cnt_reg__0\(4),
      I3 => uart_txd_i_11_n_0,
      O => uart_txd_i_4_n_0
    );
uart_txd_i_5: unisim.vcomponents.LUT4
    generic map(
      INIT => X"8002"
    )
        port map (
      I0 => uart_txd_i_12_n_0,
      I1 => \clk_cnt_reg__0\(4),
      I2 => \clk_cnt_reg__0\(3),
      I3 => \clk_cnt_reg__0\(0),
      O => uart_txd_i_5_n_0
    );
uart_txd_i_6: unisim.vcomponents.LUT5
    generic map(
      INIT => X"FFD5D5D5"
    )
        port map (
      I0 => tx_flag_reg_n_0,
      I1 => uart_txd_i_13_n_0,
      I2 => \clk_cnt[7]_i_2__0_n_0\,
      I3 => uart_txd_i_14_n_0,
      I4 => uart_txd_i_15_n_0,
      O => uart_txd_i_6_n_0
    );
uart_txd_i_7: unisim.vcomponents.LUT5
    generic map(
      INIT => X"44400040"
    )
        port map (
      I0 => \clk_cnt_reg__0\(4),
      I1 => \clk_cnt_reg__0\(2),
      I2 => data3,
      I3 => \clk_cnt_reg__0\(7),
      I4 => data6,
      O => uart_txd_i_7_n_0
    );
uart_txd_i_8: unisim.vcomponents.LUT5
    generic map(
      INIT => X"02020200"
    )
        port map (
      I0 => \clk_cnt_reg__0\(7),
      I1 => \clk_cnt_reg__0\(2),
      I2 => \clk_cnt_reg__0\(4),
      I3 => \clk_cnt_reg__0\(5),
      I4 => data7,
      O => uart_txd_i_8_n_0
    );
uart_txd_i_9: unisim.vcomponents.LUT5
    generic map(
      INIT => X"00000040"
    )
        port map (
      I0 => \clk_cnt_reg__0\(7),
      I1 => data2,
      I2 => \clk_cnt_reg__0\(1),
      I3 => \clk_cnt_reg__0\(4),
      I4 => \clk_cnt_reg__0\(2),
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
    led : out STD_LOGIC_VECTOR ( 3 downto 0 );
    M00_AXI_araddr : out STD_LOGIC_VECTOR ( 31 downto 0 );
    M00_AXI_wdata : out STD_LOGIC_VECTOR ( 31 downto 0 );
    M00_AXI_wstrb : out STD_LOGIC_VECTOR ( 3 downto 0 );
    uart_txd : out STD_LOGIC;
    debug : out STD_LOGIC_VECTOR ( 2 downto 0 );
    M00_AXI_wlast : out STD_LOGIC;
    M00_AXI_rvalid : in STD_LOGIC;
    clk : in STD_LOGIC;
    M00_AXI_arready : in STD_LOGIC;
    M00_AXI_wready : in STD_LOGIC;
    M00_AXI_bvalid : in STD_LOGIC;
    M00_AXI_awready : in STD_LOGIC;
    uart_rxd : in STD_LOGIC;
    M00_AXI_rdata : in STD_LOGIC_VECTOR ( 31 downto 0 );
    keyin : in STD_LOGIC_VECTOR ( 0 to 0 )
  );
  attribute ORIG_REF_NAME : string;
  attribute ORIG_REF_NAME of design_1_test_0_0_test : entity is "test";
end design_1_test_0_0_test;

architecture STRUCTURE of design_1_test_0_0_test is
  signal \FSM_onehot_debug_readmem_ar_status[0]_i_1_n_0\ : STD_LOGIC;
  signal \FSM_onehot_debug_readmem_ar_status[1]_i_1_n_0\ : STD_LOGIC;
  signal \FSM_onehot_debug_readmem_ar_status[2]_i_1_n_0\ : STD_LOGIC;
  signal \FSM_onehot_debug_readmem_ar_status_reg_n_0_[0]\ : STD_LOGIC;
  attribute RTL_KEEP : string;
  attribute RTL_KEEP of \FSM_onehot_debug_readmem_ar_status_reg_n_0_[0]\ : signal is "yes";
  signal \FSM_onehot_debug_readmem_ar_status_reg_n_0_[1]\ : STD_LOGIC;
  attribute RTL_KEEP of \FSM_onehot_debug_readmem_ar_status_reg_n_0_[1]\ : signal is "yes";
  signal \FSM_onehot_debug_readmem_ar_status_reg_n_0_[2]\ : STD_LOGIC;
  attribute RTL_KEEP of \FSM_onehot_debug_readmem_ar_status_reg_n_0_[2]\ : signal is "yes";
  signal \FSM_onehot_debug_readmem_aw_status[0]_i_1_n_0\ : STD_LOGIC;
  signal \FSM_onehot_debug_readmem_aw_status[1]_i_1_n_0\ : STD_LOGIC;
  signal \FSM_onehot_debug_readmem_aw_status[2]_i_1_n_0\ : STD_LOGIC;
  signal \FSM_onehot_debug_readmem_aw_status_reg_n_0_[0]\ : STD_LOGIC;
  attribute RTL_KEEP of \FSM_onehot_debug_readmem_aw_status_reg_n_0_[0]\ : signal is "yes";
  signal \FSM_onehot_debug_readmem_aw_status_reg_n_0_[1]\ : STD_LOGIC;
  attribute RTL_KEEP of \FSM_onehot_debug_readmem_aw_status_reg_n_0_[1]\ : signal is "yes";
  signal \FSM_onehot_debug_readmem_aw_status_reg_n_0_[2]\ : STD_LOGIC;
  attribute RTL_KEEP of \FSM_onehot_debug_readmem_aw_status_reg_n_0_[2]\ : signal is "yes";
  signal \FSM_onehot_debug_readmem_b_status[0]_i_1_n_0\ : STD_LOGIC;
  signal \FSM_onehot_debug_readmem_b_status[1]_i_1_n_0\ : STD_LOGIC;
  signal \FSM_onehot_debug_readmem_b_status[2]_i_1_n_0\ : STD_LOGIC;
  signal \FSM_onehot_debug_readmem_b_status_reg_n_0_[0]\ : STD_LOGIC;
  attribute RTL_KEEP of \FSM_onehot_debug_readmem_b_status_reg_n_0_[0]\ : signal is "yes";
  signal \FSM_onehot_debug_readmem_b_status_reg_n_0_[1]\ : STD_LOGIC;
  attribute RTL_KEEP of \FSM_onehot_debug_readmem_b_status_reg_n_0_[1]\ : signal is "yes";
  signal \FSM_onehot_debug_readmem_b_status_reg_n_0_[2]\ : STD_LOGIC;
  attribute RTL_KEEP of \FSM_onehot_debug_readmem_b_status_reg_n_0_[2]\ : signal is "yes";
  signal \FSM_onehot_debug_readmem_dr_status[0]_i_1_n_0\ : STD_LOGIC;
  signal \FSM_onehot_debug_readmem_dr_status[1]_i_1_n_0\ : STD_LOGIC;
  signal \FSM_onehot_debug_readmem_dr_status[2]_i_1_n_0\ : STD_LOGIC;
  signal \FSM_onehot_debug_readmem_dr_status[2]_i_2_n_0\ : STD_LOGIC;
  signal \FSM_onehot_debug_readmem_dr_status_reg_n_0_[0]\ : STD_LOGIC;
  attribute RTL_KEEP of \FSM_onehot_debug_readmem_dr_status_reg_n_0_[0]\ : signal is "yes";
  signal \FSM_onehot_debug_readmem_dr_status_reg_n_0_[1]\ : STD_LOGIC;
  attribute RTL_KEEP of \FSM_onehot_debug_readmem_dr_status_reg_n_0_[1]\ : signal is "yes";
  signal \FSM_onehot_debug_readmem_dr_status_reg_n_0_[2]\ : STD_LOGIC;
  attribute RTL_KEEP of \FSM_onehot_debug_readmem_dr_status_reg_n_0_[2]\ : signal is "yes";
  signal \FSM_onehot_debug_readmem_dw_status[0]_i_1_n_0\ : STD_LOGIC;
  signal \FSM_onehot_debug_readmem_dw_status[1]_i_1_n_0\ : STD_LOGIC;
  signal \FSM_onehot_debug_readmem_dw_status[2]_i_1_n_0\ : STD_LOGIC;
  signal \FSM_onehot_debug_readmem_dw_status[2]_i_2_n_0\ : STD_LOGIC;
  signal \FSM_onehot_debug_readmem_dw_status_reg_n_0_[0]\ : STD_LOGIC;
  attribute RTL_KEEP of \FSM_onehot_debug_readmem_dw_status_reg_n_0_[0]\ : signal is "yes";
  signal \FSM_onehot_debug_readmem_dw_status_reg_n_0_[1]\ : STD_LOGIC;
  attribute RTL_KEEP of \FSM_onehot_debug_readmem_dw_status_reg_n_0_[1]\ : signal is "yes";
  signal \FSM_onehot_debug_readmem_dw_status_reg_n_0_[2]\ : STD_LOGIC;
  attribute RTL_KEEP of \FSM_onehot_debug_readmem_dw_status_reg_n_0_[2]\ : signal is "yes";
  signal M00_AXI_arvalid_i_1_n_0 : STD_LOGIC;
  signal M00_AXI_awvalid_i_1_n_0 : STD_LOGIC;
  signal M00_AXI_bready_i_1_n_0 : STD_LOGIC;
  signal M00_AXI_bready_i_2_n_0 : STD_LOGIC;
  signal M00_AXI_rready_i_1_n_0 : STD_LOGIC;
  signal M00_AXI_rready_i_2_n_0 : STD_LOGIC;
  signal \M00_AXI_wdata[15]_i_1_n_0\ : STD_LOGIC;
  signal \M00_AXI_wdata[23]_i_1_n_0\ : STD_LOGIC;
  signal \M00_AXI_wdata[31]_i_1_n_0\ : STD_LOGIC;
  signal \M00_AXI_wdata[31]_i_2_n_0\ : STD_LOGIC;
  signal \M00_AXI_wdata[7]_i_1_n_0\ : STD_LOGIC;
  signal \^m00_axi_wlast\ : STD_LOGIC;
  signal M00_AXI_wlast_i_1_n_0 : STD_LOGIC;
  signal \M00_AXI_wstrb[3]_i_1_n_0\ : STD_LOGIC;
  signal \M00_AXI_wstrb[3]_i_2_n_0\ : STD_LOGIC;
  signal M00_AXI_wvalid_i_1_n_0 : STD_LOGIC;
  signal \addr[15]_i_1_n_0\ : STD_LOGIC;
  signal \addr[23]_i_1_n_0\ : STD_LOGIC;
  signal \addr[31]_i_1_n_0\ : STD_LOGIC;
  signal \addr[31]_i_2_n_0\ : STD_LOGIC;
  signal \addr[7]_i_1_n_0\ : STD_LOGIC;
  signal command : STD_LOGIC_VECTOR ( 7 downto 0 );
  signal command_done_i_1_n_0 : STD_LOGIC;
  signal command_done_i_2_n_0 : STD_LOGIC;
  signal command_done_i_3_n_0 : STD_LOGIC;
  signal command_done_i_4_n_0 : STD_LOGIC;
  signal command_done_i_5_n_0 : STD_LOGIC;
  signal command_done_i_6_n_0 : STD_LOGIC;
  signal command_done_i_7_n_0 : STD_LOGIC;
  signal command_done_i_8_n_0 : STD_LOGIC;
  signal command_done_reg_n_0 : STD_LOGIC;
  signal command_temp : STD_LOGIC_VECTOR ( 7 downto 0 );
  signal command_temp_1 : STD_LOGIC;
  signal data : STD_LOGIC;
  signal data0 : STD_LOGIC_VECTOR ( 31 downto 1 );
  signal \data_buff[31]_i_1_n_0\ : STD_LOGIC;
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
  signal \data_buff_reg_n_0_[2]\ : STD_LOGIC;
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
  signal \^debug\ : STD_LOGIC_VECTOR ( 2 downto 0 );
  signal flg_i_10_n_0 : STD_LOGIC;
  signal flg_i_1_n_0 : STD_LOGIC;
  signal flg_i_3_n_0 : STD_LOGIC;
  signal flg_i_4_n_0 : STD_LOGIC;
  signal flg_i_5_n_0 : STD_LOGIC;
  signal flg_i_6_n_0 : STD_LOGIC;
  signal flg_i_7_n_0 : STD_LOGIC;
  signal flg_i_8_n_0 : STD_LOGIC;
  signal flg_i_9_n_0 : STD_LOGIC;
  signal \^led\ : STD_LOGIC_VECTOR ( 3 downto 0 );
  signal \led[3]_INST_0_i_1_n_0\ : STD_LOGIC;
  signal p_0_in1_in : STD_LOGIC_VECTOR ( 7 downto 0 );
  signal timer : STD_LOGIC_VECTOR ( 31 downto 0 );
  signal \timer0_carry__0_n_0\ : STD_LOGIC;
  signal \timer0_carry__0_n_1\ : STD_LOGIC;
  signal \timer0_carry__0_n_2\ : STD_LOGIC;
  signal \timer0_carry__0_n_3\ : STD_LOGIC;
  signal \timer0_carry__1_n_0\ : STD_LOGIC;
  signal \timer0_carry__1_n_1\ : STD_LOGIC;
  signal \timer0_carry__1_n_2\ : STD_LOGIC;
  signal \timer0_carry__1_n_3\ : STD_LOGIC;
  signal \timer0_carry__2_n_0\ : STD_LOGIC;
  signal \timer0_carry__2_n_1\ : STD_LOGIC;
  signal \timer0_carry__2_n_2\ : STD_LOGIC;
  signal \timer0_carry__2_n_3\ : STD_LOGIC;
  signal \timer0_carry__3_n_0\ : STD_LOGIC;
  signal \timer0_carry__3_n_1\ : STD_LOGIC;
  signal \timer0_carry__3_n_2\ : STD_LOGIC;
  signal \timer0_carry__3_n_3\ : STD_LOGIC;
  signal \timer0_carry__4_n_0\ : STD_LOGIC;
  signal \timer0_carry__4_n_1\ : STD_LOGIC;
  signal \timer0_carry__4_n_2\ : STD_LOGIC;
  signal \timer0_carry__4_n_3\ : STD_LOGIC;
  signal \timer0_carry__5_n_0\ : STD_LOGIC;
  signal \timer0_carry__5_n_1\ : STD_LOGIC;
  signal \timer0_carry__5_n_2\ : STD_LOGIC;
  signal \timer0_carry__5_n_3\ : STD_LOGIC;
  signal \timer0_carry__6_n_2\ : STD_LOGIC;
  signal \timer0_carry__6_n_3\ : STD_LOGIC;
  signal timer0_carry_n_0 : STD_LOGIC;
  signal timer0_carry_n_1 : STD_LOGIC;
  signal timer0_carry_n_2 : STD_LOGIC;
  signal timer0_carry_n_3 : STD_LOGIC;
  signal timer_2 : STD_LOGIC_VECTOR ( 31 downto 0 );
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
  signal \uart_data_in[6]_i_3_n_0\ : STD_LOGIC;
  signal \uart_data_in[6]_i_4_n_0\ : STD_LOGIC;
  signal \uart_data_in[6]_i_5_n_0\ : STD_LOGIC;
  signal \uart_data_in[7]_i_2_n_0\ : STD_LOGIC;
  signal \uart_data_in[7]_i_3_n_0\ : STD_LOGIC;
  signal \uart_data_in[7]_i_4_n_0\ : STD_LOGIC;
  signal \uart_data_in[7]_i_5_n_0\ : STD_LOGIC;
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
  signal uart_send_i_2_n_0 : STD_LOGIC;
  signal \NLW_timer0_carry__6_CO_UNCONNECTED\ : STD_LOGIC_VECTOR ( 3 downto 2 );
  signal \NLW_timer0_carry__6_O_UNCONNECTED\ : STD_LOGIC_VECTOR ( 3 to 3 );
  attribute FSM_ENCODED_STATES : string;
  attribute FSM_ENCODED_STATES of \FSM_onehot_debug_readmem_ar_status_reg[0]\ : label is "iSTATE:001,iSTATE0:010,iSTATE1:100";
  attribute KEEP : string;
  attribute KEEP of \FSM_onehot_debug_readmem_ar_status_reg[0]\ : label is "yes";
  attribute FSM_ENCODED_STATES of \FSM_onehot_debug_readmem_ar_status_reg[1]\ : label is "iSTATE:001,iSTATE0:010,iSTATE1:100";
  attribute KEEP of \FSM_onehot_debug_readmem_ar_status_reg[1]\ : label is "yes";
  attribute FSM_ENCODED_STATES of \FSM_onehot_debug_readmem_ar_status_reg[2]\ : label is "iSTATE:001,iSTATE0:010,iSTATE1:100";
  attribute KEEP of \FSM_onehot_debug_readmem_ar_status_reg[2]\ : label is "yes";
  attribute FSM_ENCODED_STATES of \FSM_onehot_debug_readmem_aw_status_reg[0]\ : label is "iSTATE:001,iSTATE0:010,iSTATE1:100";
  attribute KEEP of \FSM_onehot_debug_readmem_aw_status_reg[0]\ : label is "yes";
  attribute FSM_ENCODED_STATES of \FSM_onehot_debug_readmem_aw_status_reg[1]\ : label is "iSTATE:001,iSTATE0:010,iSTATE1:100";
  attribute KEEP of \FSM_onehot_debug_readmem_aw_status_reg[1]\ : label is "yes";
  attribute FSM_ENCODED_STATES of \FSM_onehot_debug_readmem_aw_status_reg[2]\ : label is "iSTATE:001,iSTATE0:010,iSTATE1:100";
  attribute KEEP of \FSM_onehot_debug_readmem_aw_status_reg[2]\ : label is "yes";
  attribute FSM_ENCODED_STATES of \FSM_onehot_debug_readmem_b_status_reg[0]\ : label is "iSTATE:001,iSTATE0:010,iSTATE1:100";
  attribute KEEP of \FSM_onehot_debug_readmem_b_status_reg[0]\ : label is "yes";
  attribute FSM_ENCODED_STATES of \FSM_onehot_debug_readmem_b_status_reg[1]\ : label is "iSTATE:001,iSTATE0:010,iSTATE1:100";
  attribute KEEP of \FSM_onehot_debug_readmem_b_status_reg[1]\ : label is "yes";
  attribute FSM_ENCODED_STATES of \FSM_onehot_debug_readmem_b_status_reg[2]\ : label is "iSTATE:001,iSTATE0:010,iSTATE1:100";
  attribute KEEP of \FSM_onehot_debug_readmem_b_status_reg[2]\ : label is "yes";
  attribute FSM_ENCODED_STATES of \FSM_onehot_debug_readmem_dr_status_reg[0]\ : label is "iSTATE:001,iSTATE0:010,iSTATE1:100";
  attribute KEEP of \FSM_onehot_debug_readmem_dr_status_reg[0]\ : label is "yes";
  attribute FSM_ENCODED_STATES of \FSM_onehot_debug_readmem_dr_status_reg[1]\ : label is "iSTATE:001,iSTATE0:010,iSTATE1:100";
  attribute KEEP of \FSM_onehot_debug_readmem_dr_status_reg[1]\ : label is "yes";
  attribute FSM_ENCODED_STATES of \FSM_onehot_debug_readmem_dr_status_reg[2]\ : label is "iSTATE:001,iSTATE0:010,iSTATE1:100";
  attribute KEEP of \FSM_onehot_debug_readmem_dr_status_reg[2]\ : label is "yes";
  attribute FSM_ENCODED_STATES of \FSM_onehot_debug_readmem_dw_status_reg[0]\ : label is "iSTATE:001,iSTATE0:010,iSTATE1:100";
  attribute KEEP of \FSM_onehot_debug_readmem_dw_status_reg[0]\ : label is "yes";
  attribute FSM_ENCODED_STATES of \FSM_onehot_debug_readmem_dw_status_reg[1]\ : label is "iSTATE:001,iSTATE0:010,iSTATE1:100";
  attribute KEEP of \FSM_onehot_debug_readmem_dw_status_reg[1]\ : label is "yes";
  attribute FSM_ENCODED_STATES of \FSM_onehot_debug_readmem_dw_status_reg[2]\ : label is "iSTATE:001,iSTATE0:010,iSTATE1:100";
  attribute KEEP of \FSM_onehot_debug_readmem_dw_status_reg[2]\ : label is "yes";
  attribute X_INTERFACE_INFO : string;
  attribute X_INTERFACE_INFO of M00_AXI_arvalid_reg : label is "xilinx.com:interface:aximm:1.0 M00_AXI ARVALID";
  attribute X_INTERFACE_INFO of M00_AXI_awvalid_reg : label is "xilinx.com:interface:aximm:1.0 M00_AXI AWVALID";
  attribute X_INTERFACE_INFO of M00_AXI_bready_reg : label is "xilinx.com:interface:aximm:1.0 M00_AXI BREADY";
  attribute X_INTERFACE_INFO of M00_AXI_rready_reg : label is "xilinx.com:interface:aximm:1.0 M00_AXI RREADY";
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
  attribute X_INTERFACE_INFO of M00_AXI_wlast_reg : label is "xilinx.com:interface:aximm:1.0 M00_AXI WLAST";
  attribute X_INTERFACE_INFO of \M00_AXI_wstrb_reg[0]\ : label is "xilinx.com:interface:aximm:1.0 M00_AXI WSTRB";
  attribute X_INTERFACE_INFO of \M00_AXI_wstrb_reg[1]\ : label is "xilinx.com:interface:aximm:1.0 M00_AXI WSTRB";
  attribute X_INTERFACE_INFO of \M00_AXI_wstrb_reg[2]\ : label is "xilinx.com:interface:aximm:1.0 M00_AXI WSTRB";
  attribute X_INTERFACE_INFO of \M00_AXI_wstrb_reg[3]\ : label is "xilinx.com:interface:aximm:1.0 M00_AXI WSTRB";
  attribute X_INTERFACE_INFO of M00_AXI_wvalid_reg : label is "xilinx.com:interface:aximm:1.0 M00_AXI WVALID";
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
  attribute SOFT_HLUTNM : string;
  attribute SOFT_HLUTNM of command_done_i_7 : label is "soft_lutpair21";
  attribute SOFT_HLUTNM of flg_i_1 : label is "soft_lutpair18";
  attribute SOFT_HLUTNM of \led[3]_INST_0\ : label is "soft_lutpair17";
  attribute SOFT_HLUTNM of \led[3]_INST_0_i_1\ : label is "soft_lutpair21";
  attribute SOFT_HLUTNM of \timer[0]_i_1\ : label is "soft_lutpair18";
  attribute SOFT_HLUTNM of \uart_data_in[0]_i_2\ : label is "soft_lutpair20";
  attribute SOFT_HLUTNM of \uart_data_in[2]_i_1\ : label is "soft_lutpair22";
  attribute SOFT_HLUTNM of \uart_data_in[6]_i_2\ : label is "soft_lutpair17";
  attribute SOFT_HLUTNM of \uart_data_in[6]_i_3\ : label is "soft_lutpair20";
  attribute SOFT_HLUTNM of \uart_data_in[7]_i_2\ : label is "soft_lutpair22";
  attribute SOFT_HLUTNM of \uart_data_in[7]_i_3\ : label is "soft_lutpair19";
  attribute SOFT_HLUTNM of \uart_data_in[7]_i_5\ : label is "soft_lutpair19";
begin
  M00_AXI_wlast <= \^m00_axi_wlast\;
  debug(2 downto 0) <= \^debug\(2 downto 0);
  led(3 downto 0) <= \^led\(3 downto 0);
\FSM_onehot_debug_readmem_ar_status[0]_i_1\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"557FFFFF55550000"
    )
        port map (
      I0 => \FSM_onehot_debug_readmem_dr_status[2]_i_2_n_0\,
      I1 => \FSM_onehot_debug_readmem_ar_status_reg_n_0_[1]\,
      I2 => M00_AXI_arready,
      I3 => \FSM_onehot_debug_readmem_ar_status_reg_n_0_[0]\,
      I4 => M00_AXI_rready_i_2_n_0,
      I5 => \FSM_onehot_debug_readmem_ar_status_reg_n_0_[0]\,
      O => \FSM_onehot_debug_readmem_ar_status[0]_i_1_n_0\
    );
\FSM_onehot_debug_readmem_ar_status[1]_i_1\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"0A2AFFFF0A000000"
    )
        port map (
      I0 => \FSM_onehot_debug_readmem_dr_status[2]_i_2_n_0\,
      I1 => \FSM_onehot_debug_readmem_ar_status_reg_n_0_[1]\,
      I2 => M00_AXI_arready,
      I3 => \FSM_onehot_debug_readmem_ar_status_reg_n_0_[0]\,
      I4 => M00_AXI_rready_i_2_n_0,
      I5 => \FSM_onehot_debug_readmem_ar_status_reg_n_0_[1]\,
      O => \FSM_onehot_debug_readmem_ar_status[1]_i_1_n_0\
    );
\FSM_onehot_debug_readmem_ar_status[2]_i_1\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"A0AAFFFFA0800000"
    )
        port map (
      I0 => \FSM_onehot_debug_readmem_dr_status[2]_i_2_n_0\,
      I1 => \FSM_onehot_debug_readmem_ar_status_reg_n_0_[1]\,
      I2 => M00_AXI_arready,
      I3 => \FSM_onehot_debug_readmem_ar_status_reg_n_0_[0]\,
      I4 => M00_AXI_rready_i_2_n_0,
      I5 => \FSM_onehot_debug_readmem_ar_status_reg_n_0_[2]\,
      O => \FSM_onehot_debug_readmem_ar_status[2]_i_1_n_0\
    );
\FSM_onehot_debug_readmem_ar_status_reg[0]\: unisim.vcomponents.FDPE
    generic map(
      INIT => '1'
    )
        port map (
      C => clk,
      CE => '1',
      D => \FSM_onehot_debug_readmem_ar_status[0]_i_1_n_0\,
      PRE => uart_hs_inst_n_0,
      Q => \FSM_onehot_debug_readmem_ar_status_reg_n_0_[0]\
    );
\FSM_onehot_debug_readmem_ar_status_reg[1]\: unisim.vcomponents.FDCE
    generic map(
      INIT => '0'
    )
        port map (
      C => clk,
      CE => '1',
      CLR => uart_hs_inst_n_0,
      D => \FSM_onehot_debug_readmem_ar_status[1]_i_1_n_0\,
      Q => \FSM_onehot_debug_readmem_ar_status_reg_n_0_[1]\
    );
\FSM_onehot_debug_readmem_ar_status_reg[2]\: unisim.vcomponents.FDCE
    generic map(
      INIT => '0'
    )
        port map (
      C => clk,
      CE => '1',
      CLR => uart_hs_inst_n_0,
      D => \FSM_onehot_debug_readmem_ar_status[2]_i_1_n_0\,
      Q => \FSM_onehot_debug_readmem_ar_status_reg_n_0_[2]\
    );
\FSM_onehot_debug_readmem_aw_status[0]_i_1\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"0F7FFFFF0F0F0000"
    )
        port map (
      I0 => M00_AXI_awready,
      I1 => \FSM_onehot_debug_readmem_aw_status_reg_n_0_[1]\,
      I2 => \FSM_onehot_debug_readmem_dw_status[2]_i_2_n_0\,
      I3 => \FSM_onehot_debug_readmem_aw_status_reg_n_0_[0]\,
      I4 => M00_AXI_bready_i_2_n_0,
      I5 => \FSM_onehot_debug_readmem_aw_status_reg_n_0_[0]\,
      O => \FSM_onehot_debug_readmem_aw_status[0]_i_1_n_0\
    );
\FSM_onehot_debug_readmem_aw_status[1]_i_1\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"5070FFFF50000000"
    )
        port map (
      I0 => M00_AXI_awready,
      I1 => \FSM_onehot_debug_readmem_aw_status_reg_n_0_[1]\,
      I2 => \FSM_onehot_debug_readmem_dw_status[2]_i_2_n_0\,
      I3 => \FSM_onehot_debug_readmem_aw_status_reg_n_0_[0]\,
      I4 => M00_AXI_bready_i_2_n_0,
      I5 => \FSM_onehot_debug_readmem_aw_status_reg_n_0_[1]\,
      O => \FSM_onehot_debug_readmem_aw_status[1]_i_1_n_0\
    );
\FSM_onehot_debug_readmem_aw_status[2]_i_1\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"A0F0FFFFA0800000"
    )
        port map (
      I0 => M00_AXI_awready,
      I1 => \FSM_onehot_debug_readmem_aw_status_reg_n_0_[1]\,
      I2 => \FSM_onehot_debug_readmem_dw_status[2]_i_2_n_0\,
      I3 => \FSM_onehot_debug_readmem_aw_status_reg_n_0_[0]\,
      I4 => M00_AXI_bready_i_2_n_0,
      I5 => \FSM_onehot_debug_readmem_aw_status_reg_n_0_[2]\,
      O => \FSM_onehot_debug_readmem_aw_status[2]_i_1_n_0\
    );
\FSM_onehot_debug_readmem_aw_status_reg[0]\: unisim.vcomponents.FDPE
    generic map(
      INIT => '1'
    )
        port map (
      C => clk,
      CE => '1',
      D => \FSM_onehot_debug_readmem_aw_status[0]_i_1_n_0\,
      PRE => uart_hs_inst_n_0,
      Q => \FSM_onehot_debug_readmem_aw_status_reg_n_0_[0]\
    );
\FSM_onehot_debug_readmem_aw_status_reg[1]\: unisim.vcomponents.FDCE
    generic map(
      INIT => '0'
    )
        port map (
      C => clk,
      CE => '1',
      CLR => uart_hs_inst_n_0,
      D => \FSM_onehot_debug_readmem_aw_status[1]_i_1_n_0\,
      Q => \FSM_onehot_debug_readmem_aw_status_reg_n_0_[1]\
    );
\FSM_onehot_debug_readmem_aw_status_reg[2]\: unisim.vcomponents.FDCE
    generic map(
      INIT => '0'
    )
        port map (
      C => clk,
      CE => '1',
      CLR => uart_hs_inst_n_0,
      D => \FSM_onehot_debug_readmem_aw_status[2]_i_1_n_0\,
      Q => \FSM_onehot_debug_readmem_aw_status_reg_n_0_[2]\
    );
\FSM_onehot_debug_readmem_b_status[0]_i_1\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"0F7FFFFF0F0F0000"
    )
        port map (
      I0 => M00_AXI_bvalid,
      I1 => \FSM_onehot_debug_readmem_b_status_reg_n_0_[1]\,
      I2 => \FSM_onehot_debug_readmem_dw_status[2]_i_2_n_0\,
      I3 => \FSM_onehot_debug_readmem_b_status_reg_n_0_[0]\,
      I4 => M00_AXI_bready_i_2_n_0,
      I5 => \FSM_onehot_debug_readmem_b_status_reg_n_0_[0]\,
      O => \FSM_onehot_debug_readmem_b_status[0]_i_1_n_0\
    );
\FSM_onehot_debug_readmem_b_status[1]_i_1\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"5070FFFF50000000"
    )
        port map (
      I0 => M00_AXI_bvalid,
      I1 => \FSM_onehot_debug_readmem_b_status_reg_n_0_[1]\,
      I2 => \FSM_onehot_debug_readmem_dw_status[2]_i_2_n_0\,
      I3 => \FSM_onehot_debug_readmem_b_status_reg_n_0_[0]\,
      I4 => M00_AXI_bready_i_2_n_0,
      I5 => \FSM_onehot_debug_readmem_b_status_reg_n_0_[1]\,
      O => \FSM_onehot_debug_readmem_b_status[1]_i_1_n_0\
    );
\FSM_onehot_debug_readmem_b_status[2]_i_1\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"A0F0FFFFA0800000"
    )
        port map (
      I0 => M00_AXI_bvalid,
      I1 => \FSM_onehot_debug_readmem_b_status_reg_n_0_[1]\,
      I2 => \FSM_onehot_debug_readmem_dw_status[2]_i_2_n_0\,
      I3 => \FSM_onehot_debug_readmem_b_status_reg_n_0_[0]\,
      I4 => M00_AXI_bready_i_2_n_0,
      I5 => \FSM_onehot_debug_readmem_b_status_reg_n_0_[2]\,
      O => \FSM_onehot_debug_readmem_b_status[2]_i_1_n_0\
    );
\FSM_onehot_debug_readmem_b_status_reg[0]\: unisim.vcomponents.FDPE
    generic map(
      INIT => '1'
    )
        port map (
      C => clk,
      CE => '1',
      D => \FSM_onehot_debug_readmem_b_status[0]_i_1_n_0\,
      PRE => uart_hs_inst_n_0,
      Q => \FSM_onehot_debug_readmem_b_status_reg_n_0_[0]\
    );
\FSM_onehot_debug_readmem_b_status_reg[1]\: unisim.vcomponents.FDCE
    generic map(
      INIT => '0'
    )
        port map (
      C => clk,
      CE => '1',
      CLR => uart_hs_inst_n_0,
      D => \FSM_onehot_debug_readmem_b_status[1]_i_1_n_0\,
      Q => \FSM_onehot_debug_readmem_b_status_reg_n_0_[1]\
    );
\FSM_onehot_debug_readmem_b_status_reg[2]\: unisim.vcomponents.FDCE
    generic map(
      INIT => '0'
    )
        port map (
      C => clk,
      CE => '1',
      CLR => uart_hs_inst_n_0,
      D => \FSM_onehot_debug_readmem_b_status[2]_i_1_n_0\,
      Q => \FSM_onehot_debug_readmem_b_status_reg_n_0_[2]\
    );
\FSM_onehot_debug_readmem_dr_status[0]_i_1\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"557FFFFF55550000"
    )
        port map (
      I0 => \FSM_onehot_debug_readmem_dr_status[2]_i_2_n_0\,
      I1 => \FSM_onehot_debug_readmem_dr_status_reg_n_0_[1]\,
      I2 => M00_AXI_rvalid,
      I3 => \FSM_onehot_debug_readmem_dr_status_reg_n_0_[0]\,
      I4 => M00_AXI_rready_i_2_n_0,
      I5 => \FSM_onehot_debug_readmem_dr_status_reg_n_0_[0]\,
      O => \FSM_onehot_debug_readmem_dr_status[0]_i_1_n_0\
    );
\FSM_onehot_debug_readmem_dr_status[1]_i_1\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"0A2AFFFF0A000000"
    )
        port map (
      I0 => \FSM_onehot_debug_readmem_dr_status[2]_i_2_n_0\,
      I1 => \FSM_onehot_debug_readmem_dr_status_reg_n_0_[1]\,
      I2 => M00_AXI_rvalid,
      I3 => \FSM_onehot_debug_readmem_dr_status_reg_n_0_[0]\,
      I4 => M00_AXI_rready_i_2_n_0,
      I5 => \FSM_onehot_debug_readmem_dr_status_reg_n_0_[1]\,
      O => \FSM_onehot_debug_readmem_dr_status[1]_i_1_n_0\
    );
\FSM_onehot_debug_readmem_dr_status[2]_i_1\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"A0AAFFFFA0800000"
    )
        port map (
      I0 => \FSM_onehot_debug_readmem_dr_status[2]_i_2_n_0\,
      I1 => \FSM_onehot_debug_readmem_dr_status_reg_n_0_[1]\,
      I2 => M00_AXI_rvalid,
      I3 => \FSM_onehot_debug_readmem_dr_status_reg_n_0_[0]\,
      I4 => M00_AXI_rready_i_2_n_0,
      I5 => \FSM_onehot_debug_readmem_dr_status_reg_n_0_[2]\,
      O => \FSM_onehot_debug_readmem_dr_status[2]_i_1_n_0\
    );
\FSM_onehot_debug_readmem_dr_status[2]_i_2\: unisim.vcomponents.LUT2
    generic map(
      INIT => X"7"
    )
        port map (
      I0 => \FSM_onehot_debug_readmem_ar_status_reg_n_0_[2]\,
      I1 => \FSM_onehot_debug_readmem_dr_status_reg_n_0_[2]\,
      O => \FSM_onehot_debug_readmem_dr_status[2]_i_2_n_0\
    );
\FSM_onehot_debug_readmem_dr_status_reg[0]\: unisim.vcomponents.FDPE
    generic map(
      INIT => '1'
    )
        port map (
      C => clk,
      CE => '1',
      D => \FSM_onehot_debug_readmem_dr_status[0]_i_1_n_0\,
      PRE => uart_hs_inst_n_0,
      Q => \FSM_onehot_debug_readmem_dr_status_reg_n_0_[0]\
    );
\FSM_onehot_debug_readmem_dr_status_reg[1]\: unisim.vcomponents.FDCE
    generic map(
      INIT => '0'
    )
        port map (
      C => clk,
      CE => '1',
      CLR => uart_hs_inst_n_0,
      D => \FSM_onehot_debug_readmem_dr_status[1]_i_1_n_0\,
      Q => \FSM_onehot_debug_readmem_dr_status_reg_n_0_[1]\
    );
\FSM_onehot_debug_readmem_dr_status_reg[2]\: unisim.vcomponents.FDCE
    generic map(
      INIT => '0'
    )
        port map (
      C => clk,
      CE => '1',
      CLR => uart_hs_inst_n_0,
      D => \FSM_onehot_debug_readmem_dr_status[2]_i_1_n_0\,
      Q => \FSM_onehot_debug_readmem_dr_status_reg_n_0_[2]\
    );
\FSM_onehot_debug_readmem_dw_status[0]_i_1\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"0F7FFFFF0F0F0000"
    )
        port map (
      I0 => M00_AXI_wready,
      I1 => \FSM_onehot_debug_readmem_dw_status_reg_n_0_[1]\,
      I2 => \FSM_onehot_debug_readmem_dw_status[2]_i_2_n_0\,
      I3 => \FSM_onehot_debug_readmem_dw_status_reg_n_0_[0]\,
      I4 => M00_AXI_bready_i_2_n_0,
      I5 => \FSM_onehot_debug_readmem_dw_status_reg_n_0_[0]\,
      O => \FSM_onehot_debug_readmem_dw_status[0]_i_1_n_0\
    );
\FSM_onehot_debug_readmem_dw_status[1]_i_1\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"5070FFFF50000000"
    )
        port map (
      I0 => M00_AXI_wready,
      I1 => \FSM_onehot_debug_readmem_dw_status_reg_n_0_[1]\,
      I2 => \FSM_onehot_debug_readmem_dw_status[2]_i_2_n_0\,
      I3 => \FSM_onehot_debug_readmem_dw_status_reg_n_0_[0]\,
      I4 => M00_AXI_bready_i_2_n_0,
      I5 => \FSM_onehot_debug_readmem_dw_status_reg_n_0_[1]\,
      O => \FSM_onehot_debug_readmem_dw_status[1]_i_1_n_0\
    );
\FSM_onehot_debug_readmem_dw_status[2]_i_1\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"A0F0FFFFA0800000"
    )
        port map (
      I0 => M00_AXI_wready,
      I1 => \FSM_onehot_debug_readmem_dw_status_reg_n_0_[1]\,
      I2 => \FSM_onehot_debug_readmem_dw_status[2]_i_2_n_0\,
      I3 => \FSM_onehot_debug_readmem_dw_status_reg_n_0_[0]\,
      I4 => M00_AXI_bready_i_2_n_0,
      I5 => \FSM_onehot_debug_readmem_dw_status_reg_n_0_[2]\,
      O => \FSM_onehot_debug_readmem_dw_status[2]_i_1_n_0\
    );
\FSM_onehot_debug_readmem_dw_status[2]_i_2\: unisim.vcomponents.LUT3
    generic map(
      INIT => X"7F"
    )
        port map (
      I0 => \FSM_onehot_debug_readmem_dw_status_reg_n_0_[2]\,
      I1 => \FSM_onehot_debug_readmem_b_status_reg_n_0_[2]\,
      I2 => \FSM_onehot_debug_readmem_aw_status_reg_n_0_[2]\,
      O => \FSM_onehot_debug_readmem_dw_status[2]_i_2_n_0\
    );
\FSM_onehot_debug_readmem_dw_status_reg[0]\: unisim.vcomponents.FDPE
    generic map(
      INIT => '1'
    )
        port map (
      C => clk,
      CE => '1',
      D => \FSM_onehot_debug_readmem_dw_status[0]_i_1_n_0\,
      PRE => uart_hs_inst_n_0,
      Q => \FSM_onehot_debug_readmem_dw_status_reg_n_0_[0]\
    );
\FSM_onehot_debug_readmem_dw_status_reg[1]\: unisim.vcomponents.FDCE
    generic map(
      INIT => '0'
    )
        port map (
      C => clk,
      CE => '1',
      CLR => uart_hs_inst_n_0,
      D => \FSM_onehot_debug_readmem_dw_status[1]_i_1_n_0\,
      Q => \FSM_onehot_debug_readmem_dw_status_reg_n_0_[1]\
    );
\FSM_onehot_debug_readmem_dw_status_reg[2]\: unisim.vcomponents.FDCE
    generic map(
      INIT => '0'
    )
        port map (
      C => clk,
      CE => '1',
      CLR => uart_hs_inst_n_0,
      D => \FSM_onehot_debug_readmem_dw_status[2]_i_1_n_0\,
      Q => \FSM_onehot_debug_readmem_dw_status_reg_n_0_[2]\
    );
M00_AXI_arvalid_i_1: unisim.vcomponents.LUT5
    generic map(
      INIT => X"FFBF8888"
    )
        port map (
      I0 => \FSM_onehot_debug_readmem_ar_status_reg_n_0_[0]\,
      I1 => M00_AXI_rready_i_2_n_0,
      I2 => \FSM_onehot_debug_readmem_ar_status_reg_n_0_[2]\,
      I3 => \FSM_onehot_debug_readmem_ar_status_reg_n_0_[1]\,
      I4 => \^debug\(0),
      O => M00_AXI_arvalid_i_1_n_0
    );
M00_AXI_arvalid_reg: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => '1',
      CLR => uart_hs_inst_n_0,
      D => M00_AXI_arvalid_i_1_n_0,
      Q => \^debug\(0)
    );
M00_AXI_awvalid_i_1: unisim.vcomponents.LUT5
    generic map(
      INIT => X"FFBF8888"
    )
        port map (
      I0 => \FSM_onehot_debug_readmem_aw_status_reg_n_0_[0]\,
      I1 => M00_AXI_bready_i_2_n_0,
      I2 => \FSM_onehot_debug_readmem_aw_status_reg_n_0_[2]\,
      I3 => \FSM_onehot_debug_readmem_aw_status_reg_n_0_[1]\,
      I4 => \^debug\(2),
      O => M00_AXI_awvalid_i_1_n_0
    );
M00_AXI_awvalid_reg: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => '1',
      CLR => uart_hs_inst_n_0,
      D => M00_AXI_awvalid_i_1_n_0,
      Q => \^debug\(2)
    );
M00_AXI_bready_i_1: unisim.vcomponents.LUT5
    generic map(
      INIT => X"FFBF8888"
    )
        port map (
      I0 => \FSM_onehot_debug_readmem_b_status_reg_n_0_[0]\,
      I1 => M00_AXI_bready_i_2_n_0,
      I2 => \FSM_onehot_debug_readmem_b_status_reg_n_0_[2]\,
      I3 => \FSM_onehot_debug_readmem_b_status_reg_n_0_[1]\,
      I4 => \^led\(1),
      O => M00_AXI_bready_i_1_n_0
    );
M00_AXI_bready_i_2: unisim.vcomponents.LUT6
    generic map(
      INIT => X"0000004000000000"
    )
        port map (
      I0 => command_done_reg_n_0,
      I1 => command(6),
      I2 => command(4),
      I3 => \led[3]_INST_0_i_1_n_0\,
      I4 => command(1),
      I5 => command(0),
      O => M00_AXI_bready_i_2_n_0
    );
M00_AXI_bready_reg: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => '1',
      CLR => uart_hs_inst_n_0,
      D => M00_AXI_bready_i_1_n_0,
      Q => \^led\(1)
    );
M00_AXI_rready_i_1: unisim.vcomponents.LUT5
    generic map(
      INIT => X"FFBF8888"
    )
        port map (
      I0 => \FSM_onehot_debug_readmem_dr_status_reg_n_0_[0]\,
      I1 => M00_AXI_rready_i_2_n_0,
      I2 => \FSM_onehot_debug_readmem_dr_status_reg_n_0_[2]\,
      I3 => \FSM_onehot_debug_readmem_dr_status_reg_n_0_[1]\,
      I4 => \^debug\(1),
      O => M00_AXI_rready_i_1_n_0
    );
M00_AXI_rready_i_2: unisim.vcomponents.LUT6
    generic map(
      INIT => X"0000000001000000"
    )
        port map (
      I0 => command(1),
      I1 => command(0),
      I2 => command_done_reg_n_0,
      I3 => command(6),
      I4 => command(4),
      I5 => \led[3]_INST_0_i_1_n_0\,
      O => M00_AXI_rready_i_2_n_0
    );
M00_AXI_rready_reg: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => '1',
      CLR => uart_hs_inst_n_0,
      D => M00_AXI_rready_i_1_n_0,
      Q => \^debug\(1)
    );
\M00_AXI_wdata[15]_i_1\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"00200000"
    )
        port map (
      I0 => command(0),
      I1 => command(1),
      I2 => keyin(0),
      I3 => command_done_reg_n_0,
      I4 => \M00_AXI_wdata[31]_i_2_n_0\,
      O => \M00_AXI_wdata[15]_i_1_n_0\
    );
\M00_AXI_wdata[23]_i_1\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"10000000"
    )
        port map (
      I0 => command(0),
      I1 => command_done_reg_n_0,
      I2 => keyin(0),
      I3 => command(1),
      I4 => \M00_AXI_wdata[31]_i_2_n_0\,
      O => \M00_AXI_wdata[23]_i_1_n_0\
    );
\M00_AXI_wdata[31]_i_1\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"20000000"
    )
        port map (
      I0 => command(0),
      I1 => command_done_reg_n_0,
      I2 => keyin(0),
      I3 => command(1),
      I4 => \M00_AXI_wdata[31]_i_2_n_0\,
      O => \M00_AXI_wdata[31]_i_1_n_0\
    );
\M00_AXI_wdata[31]_i_2\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"0001000000000000"
    )
        port map (
      I0 => command(3),
      I1 => command(7),
      I2 => command(6),
      I3 => command(4),
      I4 => command(5),
      I5 => command(2),
      O => \M00_AXI_wdata[31]_i_2_n_0\
    );
\M00_AXI_wdata[7]_i_1\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"00100000"
    )
        port map (
      I0 => command(0),
      I1 => command(1),
      I2 => keyin(0),
      I3 => command_done_reg_n_0,
      I4 => \M00_AXI_wdata[31]_i_2_n_0\,
      O => \M00_AXI_wdata[7]_i_1_n_0\
    );
\M00_AXI_wdata_reg[0]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \M00_AXI_wdata[7]_i_1_n_0\,
      D => \data_reg_n_0_[0]\,
      Q => M00_AXI_wdata(0),
      R => '0'
    );
\M00_AXI_wdata_reg[10]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \M00_AXI_wdata[15]_i_1_n_0\,
      D => \data_reg_n_0_[2]\,
      Q => M00_AXI_wdata(10),
      R => '0'
    );
\M00_AXI_wdata_reg[11]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \M00_AXI_wdata[15]_i_1_n_0\,
      D => \data_reg_n_0_[3]\,
      Q => M00_AXI_wdata(11),
      R => '0'
    );
\M00_AXI_wdata_reg[12]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \M00_AXI_wdata[15]_i_1_n_0\,
      D => \data_reg_n_0_[4]\,
      Q => M00_AXI_wdata(12),
      R => '0'
    );
\M00_AXI_wdata_reg[13]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \M00_AXI_wdata[15]_i_1_n_0\,
      D => \data_reg_n_0_[5]\,
      Q => M00_AXI_wdata(13),
      R => '0'
    );
\M00_AXI_wdata_reg[14]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \M00_AXI_wdata[15]_i_1_n_0\,
      D => \data_reg_n_0_[6]\,
      Q => M00_AXI_wdata(14),
      R => '0'
    );
\M00_AXI_wdata_reg[15]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \M00_AXI_wdata[15]_i_1_n_0\,
      D => \data_reg_n_0_[7]\,
      Q => M00_AXI_wdata(15),
      R => '0'
    );
\M00_AXI_wdata_reg[16]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \M00_AXI_wdata[23]_i_1_n_0\,
      D => \data_reg_n_0_[0]\,
      Q => M00_AXI_wdata(16),
      R => '0'
    );
\M00_AXI_wdata_reg[17]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \M00_AXI_wdata[23]_i_1_n_0\,
      D => \data_reg_n_0_[1]\,
      Q => M00_AXI_wdata(17),
      R => '0'
    );
\M00_AXI_wdata_reg[18]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \M00_AXI_wdata[23]_i_1_n_0\,
      D => \data_reg_n_0_[2]\,
      Q => M00_AXI_wdata(18),
      R => '0'
    );
\M00_AXI_wdata_reg[19]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \M00_AXI_wdata[23]_i_1_n_0\,
      D => \data_reg_n_0_[3]\,
      Q => M00_AXI_wdata(19),
      R => '0'
    );
\M00_AXI_wdata_reg[1]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \M00_AXI_wdata[7]_i_1_n_0\,
      D => \data_reg_n_0_[1]\,
      Q => M00_AXI_wdata(1),
      R => '0'
    );
\M00_AXI_wdata_reg[20]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \M00_AXI_wdata[23]_i_1_n_0\,
      D => \data_reg_n_0_[4]\,
      Q => M00_AXI_wdata(20),
      R => '0'
    );
\M00_AXI_wdata_reg[21]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \M00_AXI_wdata[23]_i_1_n_0\,
      D => \data_reg_n_0_[5]\,
      Q => M00_AXI_wdata(21),
      R => '0'
    );
\M00_AXI_wdata_reg[22]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \M00_AXI_wdata[23]_i_1_n_0\,
      D => \data_reg_n_0_[6]\,
      Q => M00_AXI_wdata(22),
      R => '0'
    );
\M00_AXI_wdata_reg[23]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \M00_AXI_wdata[23]_i_1_n_0\,
      D => \data_reg_n_0_[7]\,
      Q => M00_AXI_wdata(23),
      R => '0'
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
      CE => \M00_AXI_wdata[7]_i_1_n_0\,
      D => \data_reg_n_0_[2]\,
      Q => M00_AXI_wdata(2),
      R => '0'
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
      CE => \M00_AXI_wdata[7]_i_1_n_0\,
      D => \data_reg_n_0_[3]\,
      Q => M00_AXI_wdata(3),
      R => '0'
    );
\M00_AXI_wdata_reg[4]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \M00_AXI_wdata[7]_i_1_n_0\,
      D => \data_reg_n_0_[4]\,
      Q => M00_AXI_wdata(4),
      R => '0'
    );
\M00_AXI_wdata_reg[5]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \M00_AXI_wdata[7]_i_1_n_0\,
      D => \data_reg_n_0_[5]\,
      Q => M00_AXI_wdata(5),
      R => '0'
    );
\M00_AXI_wdata_reg[6]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \M00_AXI_wdata[7]_i_1_n_0\,
      D => \data_reg_n_0_[6]\,
      Q => M00_AXI_wdata(6),
      R => '0'
    );
\M00_AXI_wdata_reg[7]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \M00_AXI_wdata[7]_i_1_n_0\,
      D => \data_reg_n_0_[7]\,
      Q => M00_AXI_wdata(7),
      R => '0'
    );
\M00_AXI_wdata_reg[8]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \M00_AXI_wdata[15]_i_1_n_0\,
      D => \data_reg_n_0_[0]\,
      Q => M00_AXI_wdata(8),
      R => '0'
    );
\M00_AXI_wdata_reg[9]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \M00_AXI_wdata[15]_i_1_n_0\,
      D => \data_reg_n_0_[1]\,
      Q => M00_AXI_wdata(9),
      R => '0'
    );
M00_AXI_wlast_i_1: unisim.vcomponents.LUT6
    generic map(
      INIT => X"FFDFFFFFAA000000"
    )
        port map (
      I0 => keyin(0),
      I1 => \FSM_onehot_debug_readmem_dw_status_reg_n_0_[1]\,
      I2 => \FSM_onehot_debug_readmem_dw_status_reg_n_0_[2]\,
      I3 => \FSM_onehot_debug_readmem_dw_status_reg_n_0_[0]\,
      I4 => M00_AXI_bready_i_2_n_0,
      I5 => \^m00_axi_wlast\,
      O => M00_AXI_wlast_i_1_n_0
    );
M00_AXI_wlast_reg: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => '1',
      D => M00_AXI_wlast_i_1_n_0,
      Q => \^m00_axi_wlast\,
      R => '0'
    );
\M00_AXI_wstrb[3]_i_1\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"0000000000000200"
    )
        port map (
      I0 => command(5),
      I1 => command(6),
      I2 => command(1),
      I3 => command(4),
      I4 => command_done_reg_n_0,
      I5 => \M00_AXI_wstrb[3]_i_2_n_0\,
      O => \M00_AXI_wstrb[3]_i_1_n_0\
    );
\M00_AXI_wstrb[3]_i_2\: unisim.vcomponents.LUT3
    generic map(
      INIT => X"FE"
    )
        port map (
      I0 => command(7),
      I1 => command(3),
      I2 => command(2),
      O => \M00_AXI_wstrb[3]_i_2_n_0\
    );
\M00_AXI_wstrb_reg[0]\: unisim.vcomponents.FDPE
     port map (
      C => clk,
      CE => \M00_AXI_wstrb[3]_i_1_n_0\,
      D => \data_reg_n_0_[0]\,
      PRE => uart_hs_inst_n_0,
      Q => M00_AXI_wstrb(0)
    );
\M00_AXI_wstrb_reg[1]\: unisim.vcomponents.FDPE
     port map (
      C => clk,
      CE => \M00_AXI_wstrb[3]_i_1_n_0\,
      D => \data_reg_n_0_[1]\,
      PRE => uart_hs_inst_n_0,
      Q => M00_AXI_wstrb(1)
    );
\M00_AXI_wstrb_reg[2]\: unisim.vcomponents.FDPE
     port map (
      C => clk,
      CE => \M00_AXI_wstrb[3]_i_1_n_0\,
      D => \data_reg_n_0_[2]\,
      PRE => uart_hs_inst_n_0,
      Q => M00_AXI_wstrb(2)
    );
\M00_AXI_wstrb_reg[3]\: unisim.vcomponents.FDPE
     port map (
      C => clk,
      CE => \M00_AXI_wstrb[3]_i_1_n_0\,
      D => \data_reg_n_0_[3]\,
      PRE => uart_hs_inst_n_0,
      Q => M00_AXI_wstrb(3)
    );
M00_AXI_wvalid_i_1: unisim.vcomponents.LUT5
    generic map(
      INIT => X"FBFFAA00"
    )
        port map (
      I0 => \FSM_onehot_debug_readmem_dw_status_reg_n_0_[0]\,
      I1 => \FSM_onehot_debug_readmem_dw_status_reg_n_0_[2]\,
      I2 => \FSM_onehot_debug_readmem_dw_status_reg_n_0_[1]\,
      I3 => M00_AXI_bready_i_2_n_0,
      I4 => \^led\(0),
      O => M00_AXI_wvalid_i_1_n_0
    );
M00_AXI_wvalid_reg: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => '1',
      CLR => uart_hs_inst_n_0,
      D => M00_AXI_wvalid_i_1_n_0,
      Q => \^led\(0)
    );
\addr[15]_i_1\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"00200000"
    )
        port map (
      I0 => command(0),
      I1 => command(1),
      I2 => keyin(0),
      I3 => command_done_reg_n_0,
      I4 => \addr[31]_i_2_n_0\,
      O => \addr[15]_i_1_n_0\
    );
\addr[23]_i_1\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"10000000"
    )
        port map (
      I0 => command(0),
      I1 => command_done_reg_n_0,
      I2 => keyin(0),
      I3 => command(1),
      I4 => \addr[31]_i_2_n_0\,
      O => \addr[23]_i_1_n_0\
    );
\addr[31]_i_1\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"20000000"
    )
        port map (
      I0 => command(0),
      I1 => command_done_reg_n_0,
      I2 => keyin(0),
      I3 => command(1),
      I4 => \addr[31]_i_2_n_0\,
      O => \addr[31]_i_1_n_0\
    );
\addr[31]_i_2\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"0000000000000002"
    )
        port map (
      I0 => command(5),
      I1 => command(4),
      I2 => command(6),
      I3 => command(2),
      I4 => command(3),
      I5 => command(7),
      O => \addr[31]_i_2_n_0\
    );
\addr[7]_i_1\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"00100000"
    )
        port map (
      I0 => command(0),
      I1 => command(1),
      I2 => keyin(0),
      I3 => command_done_reg_n_0,
      I4 => \addr[31]_i_2_n_0\,
      O => \addr[7]_i_1_n_0\
    );
\addr_reg[0]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \addr[7]_i_1_n_0\,
      D => \data_reg_n_0_[0]\,
      Q => M00_AXI_araddr(0),
      R => '0'
    );
\addr_reg[10]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \addr[15]_i_1_n_0\,
      D => \data_reg_n_0_[2]\,
      Q => M00_AXI_araddr(10),
      R => '0'
    );
\addr_reg[11]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \addr[15]_i_1_n_0\,
      D => \data_reg_n_0_[3]\,
      Q => M00_AXI_araddr(11),
      R => '0'
    );
\addr_reg[12]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \addr[15]_i_1_n_0\,
      D => \data_reg_n_0_[4]\,
      Q => M00_AXI_araddr(12),
      R => '0'
    );
\addr_reg[13]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \addr[15]_i_1_n_0\,
      D => \data_reg_n_0_[5]\,
      Q => M00_AXI_araddr(13),
      R => '0'
    );
\addr_reg[14]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \addr[15]_i_1_n_0\,
      D => \data_reg_n_0_[6]\,
      Q => M00_AXI_araddr(14),
      R => '0'
    );
\addr_reg[15]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \addr[15]_i_1_n_0\,
      D => \data_reg_n_0_[7]\,
      Q => M00_AXI_araddr(15),
      R => '0'
    );
\addr_reg[16]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \addr[23]_i_1_n_0\,
      D => \data_reg_n_0_[0]\,
      Q => M00_AXI_araddr(16),
      R => '0'
    );
\addr_reg[17]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \addr[23]_i_1_n_0\,
      D => \data_reg_n_0_[1]\,
      Q => M00_AXI_araddr(17),
      R => '0'
    );
\addr_reg[18]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \addr[23]_i_1_n_0\,
      D => \data_reg_n_0_[2]\,
      Q => M00_AXI_araddr(18),
      R => '0'
    );
\addr_reg[19]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \addr[23]_i_1_n_0\,
      D => \data_reg_n_0_[3]\,
      Q => M00_AXI_araddr(19),
      R => '0'
    );
\addr_reg[1]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \addr[7]_i_1_n_0\,
      D => \data_reg_n_0_[1]\,
      Q => M00_AXI_araddr(1),
      R => '0'
    );
\addr_reg[20]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \addr[23]_i_1_n_0\,
      D => \data_reg_n_0_[4]\,
      Q => M00_AXI_araddr(20),
      R => '0'
    );
\addr_reg[21]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \addr[23]_i_1_n_0\,
      D => \data_reg_n_0_[5]\,
      Q => M00_AXI_araddr(21),
      R => '0'
    );
\addr_reg[22]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \addr[23]_i_1_n_0\,
      D => \data_reg_n_0_[6]\,
      Q => M00_AXI_araddr(22),
      R => '0'
    );
\addr_reg[23]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \addr[23]_i_1_n_0\,
      D => \data_reg_n_0_[7]\,
      Q => M00_AXI_araddr(23),
      R => '0'
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
      CE => \addr[7]_i_1_n_0\,
      D => \data_reg_n_0_[2]\,
      Q => M00_AXI_araddr(2),
      R => '0'
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
      CE => \addr[7]_i_1_n_0\,
      D => \data_reg_n_0_[3]\,
      Q => M00_AXI_araddr(3),
      R => '0'
    );
\addr_reg[4]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \addr[7]_i_1_n_0\,
      D => \data_reg_n_0_[4]\,
      Q => M00_AXI_araddr(4),
      R => '0'
    );
\addr_reg[5]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \addr[7]_i_1_n_0\,
      D => \data_reg_n_0_[5]\,
      Q => M00_AXI_araddr(5),
      R => '0'
    );
\addr_reg[6]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \addr[7]_i_1_n_0\,
      D => \data_reg_n_0_[6]\,
      Q => M00_AXI_araddr(6),
      R => '0'
    );
\addr_reg[7]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \addr[7]_i_1_n_0\,
      D => \data_reg_n_0_[7]\,
      Q => M00_AXI_araddr(7),
      R => '0'
    );
\addr_reg[8]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \addr[15]_i_1_n_0\,
      D => \data_reg_n_0_[0]\,
      Q => M00_AXI_araddr(8),
      R => '0'
    );
\addr_reg[9]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \addr[15]_i_1_n_0\,
      D => \data_reg_n_0_[1]\,
      Q => M00_AXI_araddr(9),
      R => '0'
    );
command_done_i_1: unisim.vcomponents.LUT5
    generic map(
      INIT => X"FFFFAAF8"
    )
        port map (
      I0 => command_done_i_2_n_0,
      I1 => keyin(0),
      I2 => command_done_reg_n_0,
      I3 => command_done_i_3_n_0,
      I4 => command_done_i_4_n_0,
      O => command_done_i_1_n_0
    );
command_done_i_2: unisim.vcomponents.LUT6
    generic map(
      INIT => X"040C040004000400"
    )
        port map (
      I0 => \FSM_onehot_debug_readmem_dw_status[2]_i_2_n_0\,
      I1 => command_done_i_5_n_0,
      I2 => command(1),
      I3 => command(0),
      I4 => \FSM_onehot_debug_readmem_dr_status_reg_n_0_[2]\,
      I5 => \FSM_onehot_debug_readmem_ar_status_reg_n_0_[2]\,
      O => command_done_i_2_n_0
    );
command_done_i_3: unisim.vcomponents.LUT6
    generic map(
      INIT => X"0000000000010000"
    )
        port map (
      I0 => command(4),
      I1 => command(6),
      I2 => command(1),
      I3 => command(0),
      I4 => command_done_reg_n_0,
      I5 => \led[3]_INST_0_i_1_n_0\,
      O => command_done_i_3_n_0
    );
command_done_i_4: unisim.vcomponents.LUT6
    generic map(
      INIT => X"00000000AAFF80C0"
    )
        port map (
      I0 => command_done_i_6_n_0,
      I1 => command(0),
      I2 => command_done_i_7_n_0,
      I3 => command(1),
      I4 => command_done_i_8_n_0,
      I5 => command_done_reg_n_0,
      O => command_done_i_4_n_0
    );
command_done_i_5: unisim.vcomponents.LUT6
    generic map(
      INIT => X"0000000000000008"
    )
        port map (
      I0 => command(6),
      I1 => command(4),
      I2 => command(2),
      I3 => command(3),
      I4 => command(7),
      I5 => command(5),
      O => command_done_i_5_n_0
    );
command_done_i_6: unisim.vcomponents.LUT2
    generic map(
      INIT => X"6"
    )
        port map (
      I0 => command(4),
      I1 => command(5),
      O => command_done_i_6_n_0
    );
command_done_i_7: unisim.vcomponents.LUT4
    generic map(
      INIT => X"0001"
    )
        port map (
      I0 => command(2),
      I1 => command(3),
      I2 => command(7),
      I3 => command(6),
      O => command_done_i_7_n_0
    );
command_done_i_8: unisim.vcomponents.LUT6
    generic map(
      INIT => X"0001000300010000"
    )
        port map (
      I0 => command(2),
      I1 => command(3),
      I2 => command(7),
      I3 => command(6),
      I4 => command(4),
      I5 => command(5),
      O => command_done_i_8_n_0
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
\data_buff[31]_i_1\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"E0000000"
    )
        port map (
      I0 => \FSM_onehot_debug_readmem_dr_status_reg_n_0_[0]\,
      I1 => \FSM_onehot_debug_readmem_dr_status_reg_n_0_[1]\,
      I2 => M00_AXI_rvalid,
      I3 => keyin(0),
      I4 => M00_AXI_rready_i_2_n_0,
      O => \data_buff[31]_i_1_n_0\
    );
\data_buff_reg[0]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \data_buff[31]_i_1_n_0\,
      D => M00_AXI_rdata(0),
      Q => \data_buff_reg_n_0_[0]\,
      R => '0'
    );
\data_buff_reg[10]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \data_buff[31]_i_1_n_0\,
      D => M00_AXI_rdata(10),
      Q => \data_buff_reg_n_0_[10]\,
      R => '0'
    );
\data_buff_reg[11]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \data_buff[31]_i_1_n_0\,
      D => M00_AXI_rdata(11),
      Q => \data_buff_reg_n_0_[11]\,
      R => '0'
    );
\data_buff_reg[12]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \data_buff[31]_i_1_n_0\,
      D => M00_AXI_rdata(12),
      Q => \data_buff_reg_n_0_[12]\,
      R => '0'
    );
\data_buff_reg[13]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \data_buff[31]_i_1_n_0\,
      D => M00_AXI_rdata(13),
      Q => \data_buff_reg_n_0_[13]\,
      R => '0'
    );
\data_buff_reg[14]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \data_buff[31]_i_1_n_0\,
      D => M00_AXI_rdata(14),
      Q => \data_buff_reg_n_0_[14]\,
      R => '0'
    );
\data_buff_reg[15]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \data_buff[31]_i_1_n_0\,
      D => M00_AXI_rdata(15),
      Q => \data_buff_reg_n_0_[15]\,
      R => '0'
    );
\data_buff_reg[16]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \data_buff[31]_i_1_n_0\,
      D => M00_AXI_rdata(16),
      Q => \data_buff_reg_n_0_[16]\,
      R => '0'
    );
\data_buff_reg[17]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \data_buff[31]_i_1_n_0\,
      D => M00_AXI_rdata(17),
      Q => \data_buff_reg_n_0_[17]\,
      R => '0'
    );
\data_buff_reg[18]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \data_buff[31]_i_1_n_0\,
      D => M00_AXI_rdata(18),
      Q => \data_buff_reg_n_0_[18]\,
      R => '0'
    );
\data_buff_reg[19]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \data_buff[31]_i_1_n_0\,
      D => M00_AXI_rdata(19),
      Q => \data_buff_reg_n_0_[19]\,
      R => '0'
    );
\data_buff_reg[1]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \data_buff[31]_i_1_n_0\,
      D => M00_AXI_rdata(1),
      Q => \data_buff_reg_n_0_[1]\,
      R => '0'
    );
\data_buff_reg[20]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \data_buff[31]_i_1_n_0\,
      D => M00_AXI_rdata(20),
      Q => \data_buff_reg_n_0_[20]\,
      R => '0'
    );
\data_buff_reg[21]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \data_buff[31]_i_1_n_0\,
      D => M00_AXI_rdata(21),
      Q => \data_buff_reg_n_0_[21]\,
      R => '0'
    );
\data_buff_reg[22]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \data_buff[31]_i_1_n_0\,
      D => M00_AXI_rdata(22),
      Q => \data_buff_reg_n_0_[22]\,
      R => '0'
    );
\data_buff_reg[23]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \data_buff[31]_i_1_n_0\,
      D => M00_AXI_rdata(23),
      Q => \data_buff_reg_n_0_[23]\,
      R => '0'
    );
\data_buff_reg[24]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \data_buff[31]_i_1_n_0\,
      D => M00_AXI_rdata(24),
      Q => p_0_in1_in(0),
      R => '0'
    );
\data_buff_reg[25]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \data_buff[31]_i_1_n_0\,
      D => M00_AXI_rdata(25),
      Q => p_0_in1_in(1),
      R => '0'
    );
\data_buff_reg[26]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \data_buff[31]_i_1_n_0\,
      D => M00_AXI_rdata(26),
      Q => p_0_in1_in(2),
      R => '0'
    );
\data_buff_reg[27]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \data_buff[31]_i_1_n_0\,
      D => M00_AXI_rdata(27),
      Q => p_0_in1_in(3),
      R => '0'
    );
\data_buff_reg[28]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \data_buff[31]_i_1_n_0\,
      D => M00_AXI_rdata(28),
      Q => p_0_in1_in(4),
      R => '0'
    );
\data_buff_reg[29]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \data_buff[31]_i_1_n_0\,
      D => M00_AXI_rdata(29),
      Q => p_0_in1_in(5),
      R => '0'
    );
\data_buff_reg[2]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \data_buff[31]_i_1_n_0\,
      D => M00_AXI_rdata(2),
      Q => \data_buff_reg_n_0_[2]\,
      R => '0'
    );
\data_buff_reg[30]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \data_buff[31]_i_1_n_0\,
      D => M00_AXI_rdata(30),
      Q => p_0_in1_in(6),
      R => '0'
    );
\data_buff_reg[31]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \data_buff[31]_i_1_n_0\,
      D => M00_AXI_rdata(31),
      Q => p_0_in1_in(7),
      R => '0'
    );
\data_buff_reg[3]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \data_buff[31]_i_1_n_0\,
      D => M00_AXI_rdata(3),
      Q => \data_buff_reg_n_0_[3]\,
      R => '0'
    );
\data_buff_reg[4]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \data_buff[31]_i_1_n_0\,
      D => M00_AXI_rdata(4),
      Q => \data_buff_reg_n_0_[4]\,
      R => '0'
    );
\data_buff_reg[5]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \data_buff[31]_i_1_n_0\,
      D => M00_AXI_rdata(5),
      Q => \data_buff_reg_n_0_[5]\,
      R => '0'
    );
\data_buff_reg[6]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \data_buff[31]_i_1_n_0\,
      D => M00_AXI_rdata(6),
      Q => \data_buff_reg_n_0_[6]\,
      R => '0'
    );
\data_buff_reg[7]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \data_buff[31]_i_1_n_0\,
      D => M00_AXI_rdata(7),
      Q => \data_buff_reg_n_0_[7]\,
      R => '0'
    );
\data_buff_reg[8]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \data_buff[31]_i_1_n_0\,
      D => M00_AXI_rdata(8),
      Q => \data_buff_reg_n_0_[8]\,
      R => '0'
    );
\data_buff_reg[9]\: unisim.vcomponents.FDRE
     port map (
      C => clk,
      CE => \data_buff[31]_i_1_n_0\,
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
flg_i_1: unisim.vcomponents.LUT5
    generic map(
      INIT => X"FFFE0001"
    )
        port map (
      I0 => timer(0),
      I1 => flg_i_3_n_0,
      I2 => flg_i_4_n_0,
      I3 => flg_i_5_n_0,
      I4 => \^led\(2),
      O => flg_i_1_n_0
    );
flg_i_10: unisim.vcomponents.LUT4
    generic map(
      INIT => X"FFFE"
    )
        port map (
      I0 => timer(27),
      I1 => timer(26),
      I2 => timer(29),
      I3 => timer(28),
      O => flg_i_10_n_0
    );
flg_i_3: unisim.vcomponents.LUT5
    generic map(
      INIT => X"FFFFFEFF"
    )
        port map (
      I0 => timer(12),
      I1 => timer(13),
      I2 => timer(10),
      I3 => timer(11),
      I4 => flg_i_6_n_0,
      O => flg_i_3_n_0
    );
flg_i_4: unisim.vcomponents.LUT5
    generic map(
      INIT => X"FFFFFFFE"
    )
        port map (
      I0 => timer(4),
      I1 => timer(5),
      I2 => timer(2),
      I3 => timer(3),
      I4 => flg_i_7_n_0,
      O => flg_i_4_n_0
    );
flg_i_5: unisim.vcomponents.LUT6
    generic map(
      INIT => X"FFFFFFFFFFFFFFFE"
    )
        port map (
      I0 => flg_i_8_n_0,
      I1 => flg_i_9_n_0,
      I2 => timer(31),
      I3 => timer(30),
      I4 => timer(1),
      I5 => flg_i_10_n_0,
      O => flg_i_5_n_0
    );
flg_i_6: unisim.vcomponents.LUT4
    generic map(
      INIT => X"FFFD"
    )
        port map (
      I0 => timer(14),
      I1 => timer(15),
      I2 => timer(17),
      I3 => timer(16),
      O => flg_i_6_n_0
    );
flg_i_7: unisim.vcomponents.LUT4
    generic map(
      INIT => X"DFFF"
    )
        port map (
      I0 => timer(6),
      I1 => timer(7),
      I2 => timer(9),
      I3 => timer(8),
      O => flg_i_7_n_0
    );
flg_i_8: unisim.vcomponents.LUT4
    generic map(
      INIT => X"FFFD"
    )
        port map (
      I0 => timer(22),
      I1 => timer(23),
      I2 => timer(25),
      I3 => timer(24),
      O => flg_i_8_n_0
    );
flg_i_9: unisim.vcomponents.LUT4
    generic map(
      INIT => X"FFF7"
    )
        port map (
      I0 => timer(19),
      I1 => timer(18),
      I2 => timer(21),
      I3 => timer(20),
      O => flg_i_9_n_0
    );
flg_reg: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => '1',
      CLR => uart_hs_inst_n_0,
      D => flg_i_1_n_0,
      Q => \^led\(2)
    );
\led[3]_INST_0\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"00000001"
    )
        port map (
      I0 => command(0),
      I1 => command(1),
      I2 => command(4),
      I3 => command(6),
      I4 => \led[3]_INST_0_i_1_n_0\,
      O => \^led\(3)
    );
\led[3]_INST_0_i_1\: unisim.vcomponents.LUT4
    generic map(
      INIT => X"FFFE"
    )
        port map (
      I0 => command(2),
      I1 => command(3),
      I2 => command(7),
      I3 => command(5),
      O => \led[3]_INST_0_i_1_n_0\
    );
timer0_carry: unisim.vcomponents.CARRY4
     port map (
      CI => '0',
      CO(3) => timer0_carry_n_0,
      CO(2) => timer0_carry_n_1,
      CO(1) => timer0_carry_n_2,
      CO(0) => timer0_carry_n_3,
      CYINIT => timer(0),
      DI(3 downto 0) => B"0000",
      O(3 downto 0) => data0(4 downto 1),
      S(3 downto 0) => timer(4 downto 1)
    );
\timer0_carry__0\: unisim.vcomponents.CARRY4
     port map (
      CI => timer0_carry_n_0,
      CO(3) => \timer0_carry__0_n_0\,
      CO(2) => \timer0_carry__0_n_1\,
      CO(1) => \timer0_carry__0_n_2\,
      CO(0) => \timer0_carry__0_n_3\,
      CYINIT => '0',
      DI(3 downto 0) => B"0000",
      O(3 downto 0) => data0(8 downto 5),
      S(3 downto 0) => timer(8 downto 5)
    );
\timer0_carry__1\: unisim.vcomponents.CARRY4
     port map (
      CI => \timer0_carry__0_n_0\,
      CO(3) => \timer0_carry__1_n_0\,
      CO(2) => \timer0_carry__1_n_1\,
      CO(1) => \timer0_carry__1_n_2\,
      CO(0) => \timer0_carry__1_n_3\,
      CYINIT => '0',
      DI(3 downto 0) => B"0000",
      O(3 downto 0) => data0(12 downto 9),
      S(3 downto 0) => timer(12 downto 9)
    );
\timer0_carry__2\: unisim.vcomponents.CARRY4
     port map (
      CI => \timer0_carry__1_n_0\,
      CO(3) => \timer0_carry__2_n_0\,
      CO(2) => \timer0_carry__2_n_1\,
      CO(1) => \timer0_carry__2_n_2\,
      CO(0) => \timer0_carry__2_n_3\,
      CYINIT => '0',
      DI(3 downto 0) => B"0000",
      O(3 downto 0) => data0(16 downto 13),
      S(3 downto 0) => timer(16 downto 13)
    );
\timer0_carry__3\: unisim.vcomponents.CARRY4
     port map (
      CI => \timer0_carry__2_n_0\,
      CO(3) => \timer0_carry__3_n_0\,
      CO(2) => \timer0_carry__3_n_1\,
      CO(1) => \timer0_carry__3_n_2\,
      CO(0) => \timer0_carry__3_n_3\,
      CYINIT => '0',
      DI(3 downto 0) => B"0000",
      O(3 downto 0) => data0(20 downto 17),
      S(3 downto 0) => timer(20 downto 17)
    );
\timer0_carry__4\: unisim.vcomponents.CARRY4
     port map (
      CI => \timer0_carry__3_n_0\,
      CO(3) => \timer0_carry__4_n_0\,
      CO(2) => \timer0_carry__4_n_1\,
      CO(1) => \timer0_carry__4_n_2\,
      CO(0) => \timer0_carry__4_n_3\,
      CYINIT => '0',
      DI(3 downto 0) => B"0000",
      O(3 downto 0) => data0(24 downto 21),
      S(3 downto 0) => timer(24 downto 21)
    );
\timer0_carry__5\: unisim.vcomponents.CARRY4
     port map (
      CI => \timer0_carry__4_n_0\,
      CO(3) => \timer0_carry__5_n_0\,
      CO(2) => \timer0_carry__5_n_1\,
      CO(1) => \timer0_carry__5_n_2\,
      CO(0) => \timer0_carry__5_n_3\,
      CYINIT => '0',
      DI(3 downto 0) => B"0000",
      O(3 downto 0) => data0(28 downto 25),
      S(3 downto 0) => timer(28 downto 25)
    );
\timer0_carry__6\: unisim.vcomponents.CARRY4
     port map (
      CI => \timer0_carry__5_n_0\,
      CO(3 downto 2) => \NLW_timer0_carry__6_CO_UNCONNECTED\(3 downto 2),
      CO(1) => \timer0_carry__6_n_2\,
      CO(0) => \timer0_carry__6_n_3\,
      CYINIT => '0',
      DI(3 downto 0) => B"0000",
      O(3) => \NLW_timer0_carry__6_O_UNCONNECTED\(3),
      O(2 downto 0) => data0(31 downto 29),
      S(3) => '0',
      S(2 downto 0) => timer(31 downto 29)
    );
\timer[0]_i_1\: unisim.vcomponents.LUT4
    generic map(
      INIT => X"00FE"
    )
        port map (
      I0 => flg_i_5_n_0,
      I1 => flg_i_4_n_0,
      I2 => flg_i_3_n_0,
      I3 => timer(0),
      O => timer_2(0)
    );
\timer[10]_i_1\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"FFFE0000"
    )
        port map (
      I0 => timer(0),
      I1 => flg_i_3_n_0,
      I2 => flg_i_4_n_0,
      I3 => flg_i_5_n_0,
      I4 => data0(10),
      O => timer_2(10)
    );
\timer[11]_i_1\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"FFFE0000"
    )
        port map (
      I0 => timer(0),
      I1 => flg_i_3_n_0,
      I2 => flg_i_4_n_0,
      I3 => flg_i_5_n_0,
      I4 => data0(11),
      O => timer_2(11)
    );
\timer[12]_i_1\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"FFFE0000"
    )
        port map (
      I0 => timer(0),
      I1 => flg_i_3_n_0,
      I2 => flg_i_4_n_0,
      I3 => flg_i_5_n_0,
      I4 => data0(12),
      O => timer_2(12)
    );
\timer[13]_i_1\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"FFFE0000"
    )
        port map (
      I0 => timer(0),
      I1 => flg_i_3_n_0,
      I2 => flg_i_4_n_0,
      I3 => flg_i_5_n_0,
      I4 => data0(13),
      O => timer_2(13)
    );
\timer[14]_i_1\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"FFFE0000"
    )
        port map (
      I0 => timer(0),
      I1 => flg_i_3_n_0,
      I2 => flg_i_4_n_0,
      I3 => flg_i_5_n_0,
      I4 => data0(14),
      O => timer_2(14)
    );
\timer[15]_i_1\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"FFFE0000"
    )
        port map (
      I0 => timer(0),
      I1 => flg_i_3_n_0,
      I2 => flg_i_4_n_0,
      I3 => flg_i_5_n_0,
      I4 => data0(15),
      O => timer_2(15)
    );
\timer[16]_i_1\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"FFFE0000"
    )
        port map (
      I0 => timer(0),
      I1 => flg_i_3_n_0,
      I2 => flg_i_4_n_0,
      I3 => flg_i_5_n_0,
      I4 => data0(16),
      O => timer_2(16)
    );
\timer[17]_i_1\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"FFFE0000"
    )
        port map (
      I0 => timer(0),
      I1 => flg_i_3_n_0,
      I2 => flg_i_4_n_0,
      I3 => flg_i_5_n_0,
      I4 => data0(17),
      O => timer_2(17)
    );
\timer[18]_i_1\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"FFFE0000"
    )
        port map (
      I0 => timer(0),
      I1 => flg_i_3_n_0,
      I2 => flg_i_4_n_0,
      I3 => flg_i_5_n_0,
      I4 => data0(18),
      O => timer_2(18)
    );
\timer[19]_i_1\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"FFFE0000"
    )
        port map (
      I0 => timer(0),
      I1 => flg_i_3_n_0,
      I2 => flg_i_4_n_0,
      I3 => flg_i_5_n_0,
      I4 => data0(19),
      O => timer_2(19)
    );
\timer[1]_i_1\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"FFFE0000"
    )
        port map (
      I0 => timer(0),
      I1 => flg_i_3_n_0,
      I2 => flg_i_4_n_0,
      I3 => flg_i_5_n_0,
      I4 => data0(1),
      O => timer_2(1)
    );
\timer[20]_i_1\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"FFFE0000"
    )
        port map (
      I0 => timer(0),
      I1 => flg_i_3_n_0,
      I2 => flg_i_4_n_0,
      I3 => flg_i_5_n_0,
      I4 => data0(20),
      O => timer_2(20)
    );
\timer[21]_i_1\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"FFFE0000"
    )
        port map (
      I0 => timer(0),
      I1 => flg_i_3_n_0,
      I2 => flg_i_4_n_0,
      I3 => flg_i_5_n_0,
      I4 => data0(21),
      O => timer_2(21)
    );
\timer[22]_i_1\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"FFFE0000"
    )
        port map (
      I0 => timer(0),
      I1 => flg_i_3_n_0,
      I2 => flg_i_4_n_0,
      I3 => flg_i_5_n_0,
      I4 => data0(22),
      O => timer_2(22)
    );
\timer[23]_i_1\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"FFFE0000"
    )
        port map (
      I0 => timer(0),
      I1 => flg_i_3_n_0,
      I2 => flg_i_4_n_0,
      I3 => flg_i_5_n_0,
      I4 => data0(23),
      O => timer_2(23)
    );
\timer[24]_i_1\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"FFFE0000"
    )
        port map (
      I0 => timer(0),
      I1 => flg_i_3_n_0,
      I2 => flg_i_4_n_0,
      I3 => flg_i_5_n_0,
      I4 => data0(24),
      O => timer_2(24)
    );
\timer[25]_i_1\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"FFFE0000"
    )
        port map (
      I0 => timer(0),
      I1 => flg_i_3_n_0,
      I2 => flg_i_4_n_0,
      I3 => flg_i_5_n_0,
      I4 => data0(25),
      O => timer_2(25)
    );
\timer[26]_i_1\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"FFFE0000"
    )
        port map (
      I0 => timer(0),
      I1 => flg_i_3_n_0,
      I2 => flg_i_4_n_0,
      I3 => flg_i_5_n_0,
      I4 => data0(26),
      O => timer_2(26)
    );
\timer[27]_i_1\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"FFFE0000"
    )
        port map (
      I0 => timer(0),
      I1 => flg_i_3_n_0,
      I2 => flg_i_4_n_0,
      I3 => flg_i_5_n_0,
      I4 => data0(27),
      O => timer_2(27)
    );
\timer[28]_i_1\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"FFFE0000"
    )
        port map (
      I0 => timer(0),
      I1 => flg_i_3_n_0,
      I2 => flg_i_4_n_0,
      I3 => flg_i_5_n_0,
      I4 => data0(28),
      O => timer_2(28)
    );
\timer[29]_i_1\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"FFFE0000"
    )
        port map (
      I0 => timer(0),
      I1 => flg_i_3_n_0,
      I2 => flg_i_4_n_0,
      I3 => flg_i_5_n_0,
      I4 => data0(29),
      O => timer_2(29)
    );
\timer[2]_i_1\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"FFFE0000"
    )
        port map (
      I0 => timer(0),
      I1 => flg_i_3_n_0,
      I2 => flg_i_4_n_0,
      I3 => flg_i_5_n_0,
      I4 => data0(2),
      O => timer_2(2)
    );
\timer[30]_i_1\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"FFFE0000"
    )
        port map (
      I0 => timer(0),
      I1 => flg_i_3_n_0,
      I2 => flg_i_4_n_0,
      I3 => flg_i_5_n_0,
      I4 => data0(30),
      O => timer_2(30)
    );
\timer[31]_i_1\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"FFFE0000"
    )
        port map (
      I0 => timer(0),
      I1 => flg_i_3_n_0,
      I2 => flg_i_4_n_0,
      I3 => flg_i_5_n_0,
      I4 => data0(31),
      O => timer_2(31)
    );
\timer[3]_i_1\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"FFFE0000"
    )
        port map (
      I0 => timer(0),
      I1 => flg_i_3_n_0,
      I2 => flg_i_4_n_0,
      I3 => flg_i_5_n_0,
      I4 => data0(3),
      O => timer_2(3)
    );
\timer[4]_i_1\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"FFFE0000"
    )
        port map (
      I0 => timer(0),
      I1 => flg_i_3_n_0,
      I2 => flg_i_4_n_0,
      I3 => flg_i_5_n_0,
      I4 => data0(4),
      O => timer_2(4)
    );
\timer[5]_i_1\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"FFFE0000"
    )
        port map (
      I0 => timer(0),
      I1 => flg_i_3_n_0,
      I2 => flg_i_4_n_0,
      I3 => flg_i_5_n_0,
      I4 => data0(5),
      O => timer_2(5)
    );
\timer[6]_i_1\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"FFFE0000"
    )
        port map (
      I0 => timer(0),
      I1 => flg_i_3_n_0,
      I2 => flg_i_4_n_0,
      I3 => flg_i_5_n_0,
      I4 => data0(6),
      O => timer_2(6)
    );
\timer[7]_i_1\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"FFFE0000"
    )
        port map (
      I0 => timer(0),
      I1 => flg_i_3_n_0,
      I2 => flg_i_4_n_0,
      I3 => flg_i_5_n_0,
      I4 => data0(7),
      O => timer_2(7)
    );
\timer[8]_i_1\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"FFFE0000"
    )
        port map (
      I0 => timer(0),
      I1 => flg_i_3_n_0,
      I2 => flg_i_4_n_0,
      I3 => flg_i_5_n_0,
      I4 => data0(8),
      O => timer_2(8)
    );
\timer[9]_i_1\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"FFFE0000"
    )
        port map (
      I0 => timer(0),
      I1 => flg_i_3_n_0,
      I2 => flg_i_4_n_0,
      I3 => flg_i_5_n_0,
      I4 => data0(9),
      O => timer_2(9)
    );
\timer_reg[0]\: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => '1',
      CLR => uart_hs_inst_n_0,
      D => timer_2(0),
      Q => timer(0)
    );
\timer_reg[10]\: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => '1',
      CLR => uart_hs_inst_n_0,
      D => timer_2(10),
      Q => timer(10)
    );
\timer_reg[11]\: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => '1',
      CLR => uart_hs_inst_n_0,
      D => timer_2(11),
      Q => timer(11)
    );
\timer_reg[12]\: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => '1',
      CLR => uart_hs_inst_n_0,
      D => timer_2(12),
      Q => timer(12)
    );
\timer_reg[13]\: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => '1',
      CLR => uart_hs_inst_n_0,
      D => timer_2(13),
      Q => timer(13)
    );
\timer_reg[14]\: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => '1',
      CLR => uart_hs_inst_n_0,
      D => timer_2(14),
      Q => timer(14)
    );
\timer_reg[15]\: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => '1',
      CLR => uart_hs_inst_n_0,
      D => timer_2(15),
      Q => timer(15)
    );
\timer_reg[16]\: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => '1',
      CLR => uart_hs_inst_n_0,
      D => timer_2(16),
      Q => timer(16)
    );
\timer_reg[17]\: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => '1',
      CLR => uart_hs_inst_n_0,
      D => timer_2(17),
      Q => timer(17)
    );
\timer_reg[18]\: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => '1',
      CLR => uart_hs_inst_n_0,
      D => timer_2(18),
      Q => timer(18)
    );
\timer_reg[19]\: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => '1',
      CLR => uart_hs_inst_n_0,
      D => timer_2(19),
      Q => timer(19)
    );
\timer_reg[1]\: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => '1',
      CLR => uart_hs_inst_n_0,
      D => timer_2(1),
      Q => timer(1)
    );
\timer_reg[20]\: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => '1',
      CLR => uart_hs_inst_n_0,
      D => timer_2(20),
      Q => timer(20)
    );
\timer_reg[21]\: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => '1',
      CLR => uart_hs_inst_n_0,
      D => timer_2(21),
      Q => timer(21)
    );
\timer_reg[22]\: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => '1',
      CLR => uart_hs_inst_n_0,
      D => timer_2(22),
      Q => timer(22)
    );
\timer_reg[23]\: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => '1',
      CLR => uart_hs_inst_n_0,
      D => timer_2(23),
      Q => timer(23)
    );
\timer_reg[24]\: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => '1',
      CLR => uart_hs_inst_n_0,
      D => timer_2(24),
      Q => timer(24)
    );
\timer_reg[25]\: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => '1',
      CLR => uart_hs_inst_n_0,
      D => timer_2(25),
      Q => timer(25)
    );
\timer_reg[26]\: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => '1',
      CLR => uart_hs_inst_n_0,
      D => timer_2(26),
      Q => timer(26)
    );
\timer_reg[27]\: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => '1',
      CLR => uart_hs_inst_n_0,
      D => timer_2(27),
      Q => timer(27)
    );
\timer_reg[28]\: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => '1',
      CLR => uart_hs_inst_n_0,
      D => timer_2(28),
      Q => timer(28)
    );
\timer_reg[29]\: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => '1',
      CLR => uart_hs_inst_n_0,
      D => timer_2(29),
      Q => timer(29)
    );
\timer_reg[2]\: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => '1',
      CLR => uart_hs_inst_n_0,
      D => timer_2(2),
      Q => timer(2)
    );
\timer_reg[30]\: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => '1',
      CLR => uart_hs_inst_n_0,
      D => timer_2(30),
      Q => timer(30)
    );
\timer_reg[31]\: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => '1',
      CLR => uart_hs_inst_n_0,
      D => timer_2(31),
      Q => timer(31)
    );
\timer_reg[3]\: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => '1',
      CLR => uart_hs_inst_n_0,
      D => timer_2(3),
      Q => timer(3)
    );
\timer_reg[4]\: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => '1',
      CLR => uart_hs_inst_n_0,
      D => timer_2(4),
      Q => timer(4)
    );
\timer_reg[5]\: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => '1',
      CLR => uart_hs_inst_n_0,
      D => timer_2(5),
      Q => timer(5)
    );
\timer_reg[6]\: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => '1',
      CLR => uart_hs_inst_n_0,
      D => timer_2(6),
      Q => timer(6)
    );
\timer_reg[7]\: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => '1',
      CLR => uart_hs_inst_n_0,
      D => timer_2(7),
      Q => timer(7)
    );
\timer_reg[8]\: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => '1',
      CLR => uart_hs_inst_n_0,
      D => timer_2(8),
      Q => timer(8)
    );
\timer_reg[9]\: unisim.vcomponents.FDCE
     port map (
      C => clk,
      CE => '1',
      CLR => uart_hs_inst_n_0,
      D => timer_2(9),
      Q => timer(9)
    );
\uart_data_in[0]_i_1\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"FFFFFFFFFFEAEAEA"
    )
        port map (
      I0 => \uart_data_in[6]_i_2_n_0\,
      I1 => \uart_data_in[6]_i_3_n_0\,
      I2 => p_0_in1_in(0),
      I3 => \uart_data_in[6]_i_4_n_0\,
      I4 => \data_buff_reg_n_0_[8]\,
      I5 => \uart_data_in[0]_i_2_n_0\,
      O => \uart_data_in[0]_i_1_n_0\
    );
\uart_data_in[0]_i_2\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"00000A0C"
    )
        port map (
      I0 => \data_buff_reg_n_0_[16]\,
      I1 => \data_buff_reg_n_0_[0]\,
      I2 => uart_send_i_2_n_0,
      I3 => command(1),
      I4 => command(0),
      O => \uart_data_in[0]_i_2_n_0\
    );
\uart_data_in[1]_i_1\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"FFFFFFFFFFEAEAEA"
    )
        port map (
      I0 => \uart_data_in[6]_i_2_n_0\,
      I1 => \uart_data_in[6]_i_3_n_0\,
      I2 => p_0_in1_in(1),
      I3 => \uart_data_in[6]_i_4_n_0\,
      I4 => \data_buff_reg_n_0_[9]\,
      I5 => \uart_data_in[1]_i_2_n_0\,
      O => \uart_data_in[1]_i_1_n_0\
    );
\uart_data_in[1]_i_2\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"00000A0C"
    )
        port map (
      I0 => \data_buff_reg_n_0_[17]\,
      I1 => \data_buff_reg_n_0_[1]\,
      I2 => uart_send_i_2_n_0,
      I3 => command(1),
      I4 => command(0),
      O => \uart_data_in[1]_i_2_n_0\
    );
\uart_data_in[2]_i_1\: unisim.vcomponents.LUT3
    generic map(
      INIT => X"EA"
    )
        port map (
      I0 => \uart_data_in[2]_i_2_n_0\,
      I1 => p_0_in1_in(2),
      I2 => \uart_data_in[7]_i_5_n_0\,
      O => \uart_data_in[2]_i_1_n_0\
    );
\uart_data_in[2]_i_2\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"00002F2300002C20"
    )
        port map (
      I0 => \data_buff_reg_n_0_[18]\,
      I1 => command(0),
      I2 => command(1),
      I3 => \data_buff_reg_n_0_[10]\,
      I4 => uart_send_i_2_n_0,
      I5 => \data_buff_reg_n_0_[2]\,
      O => \uart_data_in[2]_i_2_n_0\
    );
\uart_data_in[3]_i_1\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"FFFFFFFFFFEAEAEA"
    )
        port map (
      I0 => \uart_data_in[6]_i_2_n_0\,
      I1 => \uart_data_in[6]_i_3_n_0\,
      I2 => p_0_in1_in(3),
      I3 => \uart_data_in[6]_i_4_n_0\,
      I4 => \data_buff_reg_n_0_[11]\,
      I5 => \uart_data_in[3]_i_2_n_0\,
      O => \uart_data_in[3]_i_1_n_0\
    );
\uart_data_in[3]_i_2\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"00000A0C"
    )
        port map (
      I0 => \data_buff_reg_n_0_[19]\,
      I1 => \data_buff_reg_n_0_[3]\,
      I2 => uart_send_i_2_n_0,
      I3 => command(1),
      I4 => command(0),
      O => \uart_data_in[3]_i_2_n_0\
    );
\uart_data_in[4]_i_1\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"FFFFFFFFFFEAEAEA"
    )
        port map (
      I0 => \uart_data_in[6]_i_2_n_0\,
      I1 => \uart_data_in[6]_i_3_n_0\,
      I2 => p_0_in1_in(4),
      I3 => \uart_data_in[6]_i_4_n_0\,
      I4 => \data_buff_reg_n_0_[12]\,
      I5 => \uart_data_in[4]_i_2_n_0\,
      O => \uart_data_in[4]_i_1_n_0\
    );
\uart_data_in[4]_i_2\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"00000A0C"
    )
        port map (
      I0 => \data_buff_reg_n_0_[20]\,
      I1 => \data_buff_reg_n_0_[4]\,
      I2 => uart_send_i_2_n_0,
      I3 => command(1),
      I4 => command(0),
      O => \uart_data_in[4]_i_2_n_0\
    );
\uart_data_in[5]_i_1\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"FFFFFFFFFFEAEAEA"
    )
        port map (
      I0 => \uart_data_in[6]_i_2_n_0\,
      I1 => \uart_data_in[6]_i_3_n_0\,
      I2 => p_0_in1_in(5),
      I3 => \uart_data_in[6]_i_4_n_0\,
      I4 => \data_buff_reg_n_0_[13]\,
      I5 => \uart_data_in[5]_i_2_n_0\,
      O => \uart_data_in[5]_i_1_n_0\
    );
\uart_data_in[5]_i_2\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"00000A0C"
    )
        port map (
      I0 => \data_buff_reg_n_0_[21]\,
      I1 => \data_buff_reg_n_0_[5]\,
      I2 => uart_send_i_2_n_0,
      I3 => command(1),
      I4 => command(0),
      O => \uart_data_in[5]_i_2_n_0\
    );
\uart_data_in[6]_i_1\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"FFFFFFFFFFEAEAEA"
    )
        port map (
      I0 => \uart_data_in[6]_i_2_n_0\,
      I1 => \uart_data_in[6]_i_3_n_0\,
      I2 => p_0_in1_in(6),
      I3 => \uart_data_in[6]_i_4_n_0\,
      I4 => \data_buff_reg_n_0_[14]\,
      I5 => \uart_data_in[6]_i_5_n_0\,
      O => \uart_data_in[6]_i_1_n_0\
    );
\uart_data_in[6]_i_2\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"00010000"
    )
        port map (
      I0 => \led[3]_INST_0_i_1_n_0\,
      I1 => command(4),
      I2 => command(6),
      I3 => command(1),
      I4 => command(0),
      O => \uart_data_in[6]_i_2_n_0\
    );
\uart_data_in[6]_i_3\: unisim.vcomponents.LUT3
    generic map(
      INIT => X"F8"
    )
        port map (
      I0 => command(1),
      I1 => command(0),
      I2 => uart_send_i_2_n_0,
      O => \uart_data_in[6]_i_3_n_0\
    );
\uart_data_in[6]_i_4\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"0000000000010000"
    )
        port map (
      I0 => command(2),
      I1 => command(3),
      I2 => command(7),
      I3 => command(5),
      I4 => \uart_data_in[7]_i_3_n_0\,
      I5 => command(6),
      O => \uart_data_in[6]_i_4_n_0\
    );
\uart_data_in[6]_i_5\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"00000A0C"
    )
        port map (
      I0 => \data_buff_reg_n_0_[22]\,
      I1 => \data_buff_reg_n_0_[6]\,
      I2 => uart_send_i_2_n_0,
      I3 => command(1),
      I4 => command(0),
      O => \uart_data_in[6]_i_5_n_0\
    );
\uart_data_in[7]_i_1\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"0000030200000000"
    )
        port map (
      I0 => command(4),
      I1 => \led[3]_INST_0_i_1_n_0\,
      I2 => command(6),
      I3 => \uart_data_in[7]_i_3_n_0\,
      I4 => command_done_reg_n_0,
      I5 => keyin(0),
      O => uart_data_in_0
    );
\uart_data_in[7]_i_2\: unisim.vcomponents.LUT3
    generic map(
      INIT => X"EA"
    )
        port map (
      I0 => \uart_data_in[7]_i_4_n_0\,
      I1 => p_0_in1_in(7),
      I2 => \uart_data_in[7]_i_5_n_0\,
      O => \uart_data_in[7]_i_2_n_0\
    );
\uart_data_in[7]_i_3\: unisim.vcomponents.LUT2
    generic map(
      INIT => X"2"
    )
        port map (
      I0 => command(0),
      I1 => command(1),
      O => \uart_data_in[7]_i_3_n_0\
    );
\uart_data_in[7]_i_4\: unisim.vcomponents.LUT6
    generic map(
      INIT => X"00002F2300002C20"
    )
        port map (
      I0 => \data_buff_reg_n_0_[23]\,
      I1 => command(0),
      I2 => command(1),
      I3 => \data_buff_reg_n_0_[15]\,
      I4 => uart_send_i_2_n_0,
      I5 => \data_buff_reg_n_0_[7]\,
      O => \uart_data_in[7]_i_4_n_0\
    );
\uart_data_in[7]_i_5\: unisim.vcomponents.LUT5
    generic map(
      INIT => X"FFFFEAEF"
    )
        port map (
      I0 => command(6),
      I1 => command(1),
      I2 => command(0),
      I3 => command(4),
      I4 => \led[3]_INST_0_i_1_n_0\,
      O => \uart_data_in[7]_i_5_n_0\
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
      keyin(0) => keyin(0),
      \uart_data_in_reg[7]\(7 downto 0) => uart_data_in(7 downto 0),
      uart_rxd => uart_rxd,
      uart_send => uart_send,
      uart_txd => uart_txd
    );
uart_send_i_1: unisim.vcomponents.LUT5
    generic map(
      INIT => X"0DFF0D00"
    )
        port map (
      I0 => uart_send_i_2_n_0,
      I1 => \uart_data_in[6]_i_4_n_0\,
      I2 => command_done_reg_n_0,
      I3 => keyin(0),
      I4 => uart_send,
      O => uart_send_i_1_n_0
    );
uart_send_i_2: unisim.vcomponents.LUT6
    generic map(
      INIT => X"FFFFFFFFFFFFFFFD"
    )
        port map (
      I0 => command(4),
      I1 => command(2),
      I2 => command(3),
      I3 => command(7),
      I4 => command(5),
      I5 => command(6),
      O => uart_send_i_2_n_0
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
  signal \^m00_axi_arvalid\ : STD_LOGIC;
  signal \^m00_axi_awready\ : STD_LOGIC;
  signal \^m00_axi_awvalid\ : STD_LOGIC;
  signal \^m00_axi_bready\ : STD_LOGIC;
  signal \^m00_axi_bvalid\ : STD_LOGIC;
  signal \^m00_axi_rready\ : STD_LOGIC;
  signal \^m00_axi_rvalid\ : STD_LOGIC;
  signal \^m00_axi_wready\ : STD_LOGIC;
  signal \^m00_axi_wvalid\ : STD_LOGIC;
  signal \^led\ : STD_LOGIC_VECTOR ( 3 downto 2 );
  attribute X_INTERFACE_INFO : string;
  attribute X_INTERFACE_INFO of M00_AXI_arready : signal is "xilinx.com:interface:aximm:1.0 M00_AXI ARREADY";
  attribute X_INTERFACE_INFO of M00_AXI_arvalid : signal is "xilinx.com:interface:aximm:1.0 M00_AXI ARVALID";
  attribute X_INTERFACE_INFO of M00_AXI_awready : signal is "xilinx.com:interface:aximm:1.0 M00_AXI AWREADY";
  attribute X_INTERFACE_INFO of M00_AXI_awvalid : signal is "xilinx.com:interface:aximm:1.0 M00_AXI AWVALID";
  attribute X_INTERFACE_INFO of M00_AXI_bready : signal is "xilinx.com:interface:aximm:1.0 M00_AXI BREADY";
  attribute X_INTERFACE_PARAMETER : string;
  attribute X_INTERFACE_PARAMETER of M00_AXI_bready : signal is "XIL_INTERFACENAME M00_AXI, DATA_WIDTH 32, PROTOCOL AXI3, FREQ_HZ 5e+07, ID_WIDTH 0, ADDR_WIDTH 32, AWUSER_WIDTH 0, ARUSER_WIDTH 0, WUSER_WIDTH 0, RUSER_WIDTH 0, BUSER_WIDTH 0, READ_WRITE_MODE READ_WRITE, HAS_BURST 1, HAS_LOCK 0, HAS_PROT 0, HAS_CACHE 0, HAS_QOS 0, HAS_REGION 0, HAS_WSTRB 1, HAS_BRESP 1, HAS_RRESP 1, SUPPORTS_NARROW_BURST 1, NUM_READ_OUTSTANDING 2, NUM_WRITE_OUTSTANDING 2, MAX_BURST_LENGTH 16, PHASE 0.000, CLK_DOMAIN design_1_processing_system7_0_0_FCLK_CLK0, NUM_READ_THREADS 1, NUM_WRITE_THREADS 1, RUSER_BITS_PER_BYTE 0, WUSER_BITS_PER_BYTE 0";
  attribute X_INTERFACE_INFO of M00_AXI_bvalid : signal is "xilinx.com:interface:aximm:1.0 M00_AXI BVALID";
  attribute X_INTERFACE_INFO of M00_AXI_rlast : signal is "xilinx.com:interface:aximm:1.0 M00_AXI RLAST";
  attribute X_INTERFACE_INFO of M00_AXI_rready : signal is "xilinx.com:interface:aximm:1.0 M00_AXI RREADY";
  attribute X_INTERFACE_INFO of M00_AXI_rvalid : signal is "xilinx.com:interface:aximm:1.0 M00_AXI RVALID";
  attribute X_INTERFACE_INFO of M00_AXI_wlast : signal is "xilinx.com:interface:aximm:1.0 M00_AXI WLAST";
  attribute X_INTERFACE_INFO of M00_AXI_wready : signal is "xilinx.com:interface:aximm:1.0 M00_AXI WREADY";
  attribute X_INTERFACE_INFO of M00_AXI_wvalid : signal is "xilinx.com:interface:aximm:1.0 M00_AXI WVALID";
  attribute X_INTERFACE_INFO of clk : signal is "xilinx.com:signal:clock:1.0 clk CLK";
  attribute X_INTERFACE_PARAMETER of clk : signal is "XIL_INTERFACENAME clk, ASSOCIATED_BUSIF M00_AXI, FREQ_HZ 5e+07, PHASE 0.000, CLK_DOMAIN design_1_processing_system7_0_0_FCLK_CLK0";
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
  M00_AXI_arvalid <= \^m00_axi_arvalid\;
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
  M00_AXI_awvalid <= \^m00_axi_awvalid\;
  M00_AXI_bready <= \^m00_axi_bready\;
  M00_AXI_rready <= \^m00_axi_rready\;
  M00_AXI_wvalid <= \^m00_axi_wvalid\;
  \^m00_axi_arready\ <= M00_AXI_arready;
  \^m00_axi_awready\ <= M00_AXI_awready;
  \^m00_axi_bvalid\ <= M00_AXI_bvalid;
  \^m00_axi_rvalid\ <= M00_AXI_rvalid;
  \^m00_axi_wready\ <= M00_AXI_wready;
  debug(7) <= \^m00_axi_awvalid\;
  debug(6) <= \^m00_axi_rready\;
  debug(5) <= \^m00_axi_arvalid\;
  debug(4) <= \^m00_axi_bvalid\;
  debug(3) <= \^m00_axi_wready\;
  debug(2) <= \^m00_axi_awready\;
  debug(1) <= \^m00_axi_rvalid\;
  debug(0) <= \^m00_axi_arready\;
  led(3 downto 2) <= \^led\(3 downto 2);
  led(1) <= \^m00_axi_bready\;
  led(0) <= \^m00_axi_wvalid\;
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
      M00_AXI_arready => \^m00_axi_arready\,
      M00_AXI_awready => \^m00_axi_awready\,
      M00_AXI_bvalid => \^m00_axi_bvalid\,
      M00_AXI_rdata(31 downto 0) => M00_AXI_rdata(31 downto 0),
      M00_AXI_rvalid => \^m00_axi_rvalid\,
      M00_AXI_wdata(31 downto 0) => M00_AXI_wdata(31 downto 0),
      M00_AXI_wlast => M00_AXI_wlast,
      M00_AXI_wready => \^m00_axi_wready\,
      M00_AXI_wstrb(3 downto 0) => M00_AXI_wstrb(3 downto 0),
      clk => clk,
      debug(2) => \^m00_axi_awvalid\,
      debug(1) => \^m00_axi_rready\,
      debug(0) => \^m00_axi_arvalid\,
      keyin(0) => keyin(3),
      led(3 downto 2) => \^led\(3 downto 2),
      led(1) => \^m00_axi_bready\,
      led(0) => \^m00_axi_wvalid\,
      uart_rxd => uart_rxd,
      uart_txd => uart_txd
    );
end STRUCTURE;