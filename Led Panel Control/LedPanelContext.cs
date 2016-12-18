using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Led_Panel_Control
{
    public class LedPanelContext
    {
        private object _ledsLock = new object();
        private Color[,] _Leds { get; set; }

        public LedPanelContext(int width, int height)
        {
            Size = new Size(width, height);
            _Leds = new Color[height, width];
        }

        
        public Size Size { get; private set; }

        public void SetColor(Point point, Color color)
        {
            lock(_ledsLock)
            {
                _Leds[point.Y, point.X] = color;
            }
        }

        public Color[,] GetLeds()
        {
            lock(_ledsLock)
            {
                var ledsCopy = _Leds.Clone() as Color[,];
                return ledsCopy;
            }
        }
    }

    public class GlediatorProtocol
    {
        private string _Begin = new string(new char[] { (char)1 });
        public string Convert(Color[,] leds)
        {
            StringBuilder builder = new StringBuilder(_Begin, leds.GetLength(0) * leds.GetLength(1) * 3 + 5);
            
            for (int j = 0; j < leds.GetLength(0); j++)
            {
                for (int i = 0; i < leds.GetLength(1); i++)
                {
                    //System.Diagnostics.Debug.WriteLine(string.Format(" i = {0}, j = {1}", i, j));
                    int k = i;
                    if (j % 2 != 0)
                    {
                        k = leds.GetLength(1) - 1 - i;
                    }
                    builder.AppendFormat("{0}{1}{2}", (char)leds[j,k].R, (char)leds[j, k].G, (char)leds[j, k].B);
                }
            }

            return builder.ToString();
        }
    }
}
