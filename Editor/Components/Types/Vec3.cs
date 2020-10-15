using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Editor.Components.Types
{
    [StructLayout(LayoutKind.Sequential)]
    class Vec3
    {
        public Vec3()
        {
            data = new float[3];
        }

        public Vec3(float x, float y, float z)
        {
            data = new float[3] { x, y, z };
        }

        public float x { get { return data[0]; } set { data[0] = value; } }
        public float y { get { return data[1]; } set { data[1] = value; } }
        public float z { get { return data[2]; } set { data[2] = value; } }

        public float[] data;
    }
}
