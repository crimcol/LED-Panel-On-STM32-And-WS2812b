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

        public Form1()
        {
            InitializeComponent();
            AppInit();
        }

        private void AppInit()
        {
            comboBox1.Items.AddRange(SerialPort.GetPortNames());
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

        private void SendText(string text)
        {
            if (string.IsNullOrWhiteSpace(text))
            {
                return;
            }

            _activeSerialPort.Write(text);
            SetSendText(text);
        }

        private void SendColor(Color color)
        {
            string colorRGBText = string.Format("A{0}{1}{2}", (char)color.R, (char)color.G, (char)color.B);
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
    }
}
