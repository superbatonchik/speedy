using System;
using System.CodeDom;
using System.Collections.Generic;
using System.Diagnostics;
using System.Globalization;
using System.IO;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Windows.Threading;

namespace speedy_wpf
{
    public partial class MainWindow : Window
    {
        private Regex SpeedRegex = new Regex(@"\[speed:\s([-+]?\d{1,3}\.\d{3})\]", RegexOptions.Compiled);
        private Regex LatRegex = new Regex(@"\[lat:\s([-+]?\d{1,}\.\d{0,})\]", RegexOptions.Compiled);
        private Regex LonRegex = new Regex(@"\[long:\s([-+]?\d{1,}\.\d{0,})\]", RegexOptions.Compiled);

        private readonly SerialPort _port;
        public MainWindow()
        {
            InitializeComponent();
            _port = new SerialPort();
            connectBtn.Click += ConnectBtnOnClick;

            string curDir = Directory.GetCurrentDirectory();
            this.Web.Navigate(new Uri(String.Format("file:///{0}/page.html", curDir)));
        }

        private void ConnectBtnOnClick(object sender, RoutedEventArgs e)
        {
            if (_port != null)
                _port.DataReceived -= PortOnDataReceived;
            _port.BaudRate = 9600;
            _port.PortName = comPortTxt.Text;
            _port.Open();
            if (!_port.IsOpen)
                MessageBox.Show(this, "cannot open port");
            else
            {
                
                _port.DataReceived += PortOnDataReceived;
            }
        }

        private float latf = 0;
        float lonf = 0;

        private void PortOnDataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            var str = _port.ReadLine();
            var speedMatch = SpeedRegex.Match(str);
            if (speedMatch.Success)
            {
                App.Current.Dispatcher.Invoke(() =>
                {
                    this.speedText.Text = speedMatch.Groups[1].Value;
                });
            }
            var lonMatch = LonRegex.Match(str);
            var lon = "";
            var lat = "";
            if (lonMatch.Success)
            {
                lon = lonMatch.Groups[1].Value;
                App.Current.Dispatcher.Invoke(() =>
                {
                    this.lonText.Text = lon;
                });
            }
            var latMatch = LatRegex.Match(str);
            if (latMatch.Success)
            {
                lat = latMatch.Groups[1].Value;
                App.Current.Dispatcher.Invoke(() =>
                {
                    this.latText.Text = lat;
                });
            }
            if (latMatch.Success && lonMatch.Success)
            {
                try
                {
                    latf = float.Parse(lat, CultureInfo.GetCultureInfo("en-us"));
                    lonf = float.Parse(lon, CultureInfo.GetCultureInfo("en-us"));
                    App.Current.Dispatcher.Invoke(() => this.Web.InvokeScript("addPoint", latf, lonf));
                } catch { }
            }
        }
    }
}
