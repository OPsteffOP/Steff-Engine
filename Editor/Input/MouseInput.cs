using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Editor.Input
{
    [StructLayout(LayoutKind.Sequential)]
    struct Float2
    {
        public Float2(float x, float y)
        {
            this.x = x;
            this.y = y;
        }

        public float x;
        public float y;
    }

    [StructLayout(LayoutKind.Sequential)]
    struct MouseInput
    {
        public MouseInput(MouseButtons mouse, Type type, float x, float y)
        {
            this.mouse = mouse;
            this.type = type;
            this.position = new Float2(x, y);
        }

        public MouseInput(MouseButtons mouse, Type type, Float2 position)
        {
            this.mouse = mouse;
            this.type = type;
            this.position = position;
        }

        public MouseButtons mouse;
        public Type type;
        public Float2 position;
    }
}
