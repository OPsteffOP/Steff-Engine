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
    struct KeyInput
    {
        public KeyInput(Keys key, Type type)
        {
            this.key = key;
            this.type = type;
        }

        public Keys key;
        public Type type;
    }
}
