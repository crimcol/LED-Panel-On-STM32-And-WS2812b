using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Timers;

namespace Led_Panel_Control
{
    public class FpsEngine : IDisposable
    {
        private Action _Action;
        private Timer _fpsTimer;
        public FpsEngine(Action action)
        {
            Fps = 30;
            _Action = action;
            _fpsTimer = new Timer();
            _fpsTimer.Elapsed += _fpsTimer_Elapsed;
        }

        private void _fpsTimer_Elapsed(object sender, ElapsedEventArgs e)
        {
            try
            {
                _Action();
            }
            catch (Exception ex)
            {
                System.Diagnostics.Debug.WriteLine(ex.Message);
            }
        }

        public int Fps { get; }

        public void Start()
        {
            _fpsTimer.Interval = 1000 / Fps;
            _fpsTimer.Start();
        }
        public void Stop()
        {
            _fpsTimer.Stop();
        }

        public void Dispose()
        {
            if (_fpsTimer == null)
            {
                return;
            }

            _fpsTimer.Dispose();
            _fpsTimer = null;
        }
    }
}
