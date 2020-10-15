using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Editor.Helpers
{
    class ControlHelper
    {
        public static List<T> GetChildrenOfType<T>(Control control) where T : Control
        {
            List<T> children = new List<T>();
            GetChildrenOfType<T>(control, ref children);
            return children;
        }

        public static void GetChildrenOfType<T>(Control control, ref List<T> controls) where T : Control
        {
            foreach (Control child in control.Controls)
            {
                T childOfType = child as T;
                if (childOfType != null)
                {
                    controls.Add(childOfType);
                }

                GetChildrenOfType<T>(child, ref controls);
            }
        }
    }
}
