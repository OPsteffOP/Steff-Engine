using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Editor.Input
{
    public enum MouseEventType
    {
        LBUTTON_DOUBLE_CLICK = 0x203,
        LBUTTON_DOWN = 0x201,
        LBUTTON_UP = 0x202,
        RBUTTON_DOWN = 0x204,
        RBUTTON_UP = 0x205,
        MOUSE_MOTION = 0x0200
    }

    public class MouseEventArguments
    {
        public bool isHandled = false;
        public MouseEventType type;
        public Point mousePositon;
    }

    class MouseMessageFilter : IMessageFilter
    {
        private MouseEvent e;
        public delegate void MouseEvent(MouseEventArguments args);

        public MouseMessageFilter(MouseEvent e)
        {
            this.e = e;
        }

        public bool PreFilterMessage(ref Message m)
        {
            // http://www.pinvoke.net/default.aspx/Constants/WM.html
            const int WM_LBUTTONDOUBLECLICK     = 0x203;
            const int WM_LBUTTONDOWN            = 0x201;
            const int WM_LBUTTONUP              = 0x202;
            const int WM_RBUTTONDOWN            = 0x204;
            const int WM_RBUTTONUP              = 0x205;
            const int WM_MOUSEMOVE              = 0x0200;

            MouseEventArguments args = new MouseEventArguments();
            args.mousePositon = Cursor.Position;

            switch (m.Msg)
            {
                case WM_LBUTTONDOUBLECLICK:
                case WM_LBUTTONDOWN:
                case WM_LBUTTONUP:
                case WM_RBUTTONDOWN:
                case WM_RBUTTONUP:
                case WM_MOUSEMOVE:
                    args.type = (MouseEventType)m.Msg;
                    e(args);
                    break;
            }

            return args.isHandled;
        }
    }
}
