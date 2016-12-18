using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Led_Panel_Control
{
    public partial class Form1 : Form
    {
        private SerialPort _activeSerialPort;
        private FpsEngine _fpsEngine;
        private LedPanelContext _ledContext;
        private GlediatorProtocol _protocol;
        private LedPanelForm _ledView;

        private Timer _randomLedTimer;
        private Random _random;
        List<Color> _colorList;
        public Form1()
        {
            InitializeComponent();
            AppInit();
        }

        private void AppInit()
        {
            comboBox1.Items.AddRange(SerialPort.GetPortNames());
            _fpsEngine = new FpsEngine(FpsHandler);
            _ledContext = new LedPanelContext(30, 5);
            _protocol = new GlediatorProtocol();
            _ledView = new LedPanelForm(_ledContext);
            _ledView.Show();

            _randomLedTimer = new Timer();
            _randomLedTimer.Interval = 100;
            _randomLedTimer.Tick += _randomLedTimer_Tick;
            //_randomLedTimer.Start();
            _random = new Random();
            _colorList = new List<Color>();
            _colorList.Add(Color.Black);
            _colorList.Add(Color.FromArgb(0, 0, 64));
            _colorList.Add(Color.FromArgb(64, 0, 0));
            _colorList.Add(Color.FromArgb(0, 64, 0));
        }

        private void _randomLedTimer_Tick(object sender, EventArgs e)
        {
            Update1();
            Update2();
        }

        private void Update1()
        {
            int x = _random.Next(0, _ledContext.Size.Width);
            int y = _random.Next(0, _ledContext.Size.Height);
            _ledContext.SetColor(new Point(x, y), _colorList[_random.Next(_colorList.Count - 1)]);
        }

        int pos = 0;
        Color green = Color.FromArgb(0, 64, 0);
        private void Update2()
        {
            if (pos == _ledContext.Size.Width * _ledContext.Size.Height - 1)
            {
                pos = 0;
            }

            int y = pos / _ledContext.Size.Width;
            int x = pos - y * _ledContext.Size.Width;
            _ledContext.SetColor(new Point(x, y), Color.Black);
            pos++;

            y = pos / _ledContext.Size.Width;
            x = pos - y * _ledContext.Size.Width;
            _ledContext.SetColor(new Point(x, y), green);
        }

        private SerialPort CreateSerialPort(string portName)
        {
            SerialPort serialPort = new SerialPort();
            serialPort.PortName = portName;
            serialPort.BaudRate = 115200;
            serialPort.Parity = Parity.None;
            serialPort.DataBits = 8;
            serialPort.StopBits = StopBits.One;
            serialPort.Handshake = Handshake.None;
            serialPort.DataReceived += SerialPort_DataReceived;
            serialPort.Encoding = Encoding.GetEncoding(28591);

            return serialPort;
        }

        private void SerialPort_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            var port = (SerialPort)sender;
            string received = port.ReadExisting();

            SetReceivedText(received);
        }

        string lastMessage = string.Empty;
        private void SendText(string text, bool print = false)
        {
            if (string.IsNullOrWhiteSpace(text))
            {
                return;
            }

            if (lastMessage == text)
            {
                return;
            }

            lastMessage = text;
            _activeSerialPort.Write(text);

            if (print)
            {
                SetSendText(text);
            }
        }

        private void SendColor(Color color)
        {
            string begin = new string(new char[] { (char)1});
            string colorRGBText = begin;

            for (int i = 0; i < 90; i++)
            {
                colorRGBText += string.Format("{0}{1}{2}", (char)color.R, (char)color.G, (char)color.B);
            }

            SendText(colorRGBText);
        }

        private void button2_Click(object sender, EventArgs e)
        {
            if (_activeSerialPort == null)
            {
                _activeSerialPort.Close();
                _activeSerialPort.Dispose();
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            _activeSerialPort = CreateSerialPort(comboBox1.Text);
            _activeSerialPort.Open();
            SetInfoText("Port opened.");
        }

        private void SetReceivedText(string text)
        {
            Action action = () => 
            {
                text = text.Replace("\r", "<CR>").Replace("\n", "<LF>");
                string formattedText = string.Format("[{0:H:mm:ss.ffff}] <<< {1}\n", DateTime.Now, text);
                richTextBox1.AppendText(formattedText);
            };
            richTextBox1.Invoke(action);
        }

        private void SetSendText(string text, Color? color = null)
        {
            string formattedText = string.Format("[{0:H:mm:ss.ffff}] >>> {1}", DateTime.Now, text);
            SetText(formattedText, color);
        }
        private void SetText(string text, Color? color = null)
        {
            Action action = () =>
            {
                string formattedText = string.Format("{0}\n", text);
                Color currentFontColor = richTextBox1.ForeColor;

                if (color.HasValue)
                {
                    richTextBox1.ForeColor = color.Value;
                }

                richTextBox1.AppendText(formattedText);

                if (color.HasValue)
                {
                    richTextBox1.ForeColor = currentFontColor;
                }
            };

            richTextBox1.Invoke(action);
        }

        private void SetInfoText(string text)
        {
            string formattedText = string.Format("[{0:H:mm:ss.ffff}] {1}\n", DateTime.Now, text);
            SetText(formattedText, Color.DarkRed);
        }

        private void button3_Click(object sender, EventArgs e)
        {
            string text = textBox1.Text;
            SendText(text);
        }

        private void panelSelectedColor_DoubleClick(object sender, EventArgs e)
        {
            if (colorDialog1.ShowDialog() == DialogResult.OK)
            {
                panelSelectedColor.BackColor = colorDialog1.Color;
                SendColor(colorDialog1.Color);
            }
        }

        private void FpsHandler()
        {
            var leds = _ledContext.GetLeds();
            string data = _protocol.Convert(leds);
            SendText(data);

            RefreshView();
            
        }

        private void StartFpsButton_Click(object sender, EventArgs e)
        {
            _fpsEngine.Start();
        }

        private void RefreshView()
        {
            // InvokeRequired required compares the thread ID of the
            // calling thread to the thread ID of the creating thread.
            // If these threads are different, it returns true.
            if (_ledView.InvokeRequired)
            {
                Action a = _ledView.Repaint;
                this.Invoke(a, null);
            }
            else
            {
                _ledView.Repaint();
            }
        }
    }
}
