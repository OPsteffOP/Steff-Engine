using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Editor
{
    public partial class DirectXPanel : Panel
    {
        public DirectXPanel()
        {
            InitializeComponent();
        }

        protected override void OnPaint(PaintEventArgs pe)
        {
            base.OnPaint(pe);
        }

        protected override void WndProc(ref Message msg)
        {
            if (Bridge.ProcessWindowsMessage(msg.HWnd, msg.Msg, msg.WParam, msg.LParam))
                base.WndProc(ref msg);
        }
    }
}
