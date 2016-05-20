using System;
using System.Windows.Forms;
using MjpegProcessor;

namespace MjpegProcessorTestWinForms
{
	public partial class Form1 : Form
	{
		public Form1(string cam)
		{
			InitializeComponent();
            start(cam);
		}

        //private void btnStart_Click(object sender, EventArgs e)
        private void start(string cam)
        {
			MjpegDecoder mjpeg = new MjpegDecoder();
			mjpeg.FrameReady += mjpeg_FrameReady;
			mjpeg.Error += mjpeg_Error;
			mjpeg.ParseStream(new Uri(cam));
		}

		private void mjpeg_FrameReady(object sender, FrameReadyEventArgs e)
		{
			image.Image = e.Bitmap;
		}

		void mjpeg_Error(object sender, ErrorEventArgs e)
		{
			MessageBox.Show(e.Message);
		}
	}
}
