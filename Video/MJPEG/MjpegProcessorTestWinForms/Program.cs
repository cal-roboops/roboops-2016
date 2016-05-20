using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;

namespace MjpegProcessorTestWinForms
{
	static class Program
	{
		/// <summary>
		/// The main entry point for the application.
		/// </summary>
		[STAThread]
		static void Main()
		{
            string cam1 = "http://192.168.137.52:8081";
            string cam2 = "http://192.168.137.52:8082";
            string cam3 = "http://192.168.137.52:8083";
            Application.EnableVisualStyles();
			Application.SetCompatibleTextRenderingDefault(false);
			Form1 stream1 = new Form1(cam1);
            Form1 stream2 = new Form1(cam2);
            Form1 stream3 = new Form1(cam3);
            //Application.Run(new Form1(cam3));

            stream1.Show();
            stream2.Show();
            stream3.Show();

            Application.Run();
        }
	}
}
