using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Led_Panel_Control
{
    public partial class LedPanelForm : Form
    {
        private LedPanelContext _context;
        private Size _panelSize;
        private Pen _pen = new Pen(Color.Black);
        public LedPanelForm(LedPanelContext ledContext)
        {
            InitializeComponent();

            _context = ledContext;
            _panelSize = ledContext.Size;
            this.DoubleBuffered = true;
        }

        public void Repaint()
        {
            this.Invalidate();
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            base.OnPaint(e);
            var control = this;
            Bitmap bmp = new Bitmap(control.Size.Width, control.Size.Height);
            Graphics g = Graphics.FromImage(bmp);

            int xInterval = control.Size.Width / _panelSize.Width;
            int yInterval = control.Size.Height / _panelSize.Height;
            var leds = _context.GetLeds();

            for (int j = 0; j < _panelSize.Height; j++)
            {
                
                for (int i = 0; i < _panelSize.Width; i++)
                {
                    SolidBrush myBrush = new SolidBrush(leds[j,i]);
                    g.FillRectangle(myBrush, i * xInterval, j * yInterval, xInterval, yInterval);
                    myBrush.Dispose();
                }
            }

            // horizontal lines

            for (int i = 1; i < _panelSize.Width; i++)
            {
                g.DrawLine(_pen, new Point(i * xInterval, 0), new Point(i * xInterval, Size.Height));
            }

            // vertical lines

            for (int i = 1; i < _panelSize.Height; i++)
            {
                g.DrawLine(_pen, new Point(0, i * yInterval), new Point(Size.Width, i * yInterval));
            }

            e.Graphics.DrawImage(bmp, 0, 0, bmp.Width, bmp.Height);
            bmp.Dispose();
            g.Dispose();
        }

        private void LedPanelForm_MouseDown(object sender, MouseEventArgs e)
        {
            SetColorByPosition(e);
        }

        private void LedPanelForm_MouseMove(object sender, MouseEventArgs e)
        {
            SetColorByPosition(e);
        }

        private void SetColorByPosition(MouseEventArgs e)
        {
            if (e.Button == MouseButtons.None)
            {
                return;
            }

            var control = this;
            int xInterval = control.Size.Width / _panelSize.Width;
            int yInterval = control.Size.Height / _panelSize.Height;

            int x = e.X / xInterval;
            int y = e.Y / yInterval;

            if (x >= 0 && x < _panelSize.Width
                && y >= 0 && y < _panelSize.Height)
            {
                Color color = e.Button == MouseButtons.Right ? Color.Black : Color.FromArgb(0, 0, 128);
                _context.SetColor(new Point(x, y), color);
            }
        }
    }
}
