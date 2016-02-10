using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;

namespace WindowsFormsApplication4
{
    class joy
    {
        /// <summary>
        /// 获取操纵杆位置和按钮状态
        /// </summary>
        /// <param name="uJoyID"></param>
        /// <param name="pji"></param>
        /// <returns></returns>
        [DllImport("winmm.dll")]
        public static extern int joyGetPosEx(int uJoyID, ref JOYINFOEX pji);

        #region 游戏手柄的位置与按钮状态
        /// <summary>
        /// 游戏手柄的位置与按钮状态
        /// </summary>
        [StructLayout(LayoutKind.Sequential)]
        public struct JOYINFOEX
        {
            /// <summary>
            /// Size, in bytes, of this structure.
            /// </summary>
            public int dwSize;
            /// <summary>
            /// Flags indicating the valid information returned in this structure. Members that do not contain valid information are set to zero.
            /// </summary>
            public int dwFlags;
            /// <summary>
            /// Current X-coordinate.
            /// </summary>
            public int dwXpos;
            /// <summary>
            /// Current Y-coordinate.
            /// </summary>
            public int dwYpos;
            /// <summary>
            /// Current Z-coordinate.
            /// </summary>
            public int dwZpos;
            /// <summary>
            /// Current position of the rudder or fourth joystick axis.
            /// </summary>
            public int dwRpos;
            /// <summary>
            /// Current fifth axis position.
            /// </summary>
            public int dwUpos;
            /// <summary>
            /// Current sixth axis position.
            /// </summary>
            public int dwVpos;
            /// <summary>
            /// Current state of the 32 joystick buttons. The value of this member can be set to any combination of JOY_BUTTONn flags, where n is a value in the range of 1 through 32 corresponding to the button that is pressed.
            /// </summary>
            public int dwButtons;
            /// <summary>
            /// Current button number that is pressed.
            /// </summary>
            public int dwButtonNumber;
            /// <summary>
            /// Current position of the point-of-view control. Values for this member are in the range 0 through 35,900. These values represent the angle, in degrees, of each view multiplied by 100.
            /// </summary>
            public int dwPOV;
            /// <summary>
            /// Reserved; do not use.
            /// </summary>
            public int dwReserved1;
            /// <summary>
            /// Reserved; do not use.
            /// </summary>
            public int dwReserved2;
        }
        #endregion

    }
}
