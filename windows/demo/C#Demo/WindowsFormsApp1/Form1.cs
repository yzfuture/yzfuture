using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Drawing.Imaging;
using System;
using System.Collections.Generic;
using System.Drawing;
using System.Drawing.Imaging;
using System.IO;
using System.Linq;
using System.Text;

namespace WindowsFormsApp1
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
            ReadCard.cardReadInit();
        }

        public void Close()
        {
            ReadCard.cardReadUninit();
        }

        private void button1_Click_1(object sender, EventArgs e)
        {
            TwoIdInfoStruct info = ReadCard.ReadCardNo(offLineRead.Checked);

            lblAddress.Text = Encoding.Unicode.GetString(info.arrTwoIdAddress);
            lblSex.Text = Encoding.Unicode.GetString(info.arrTwoIdSex);
            lblBirthday.Text = Encoding.Unicode.GetString(info.arrTwoIdBirthday);
            lblDept.Text = Encoding.Unicode.GetString(info.arrTwoIdSignedDepartment);
            lblIdCard.Text = Encoding.Unicode.GetString(info.arrTwoIdNo);
            lblName.Text = Encoding.Unicode.GetString(info.arrTwoIdName);
            lblNation.Text = Encoding.Unicode.GetString(info.arrTwoIdNation);
            lblValidDate.Text = Encoding.Unicode.GetString(info.arrTwoIdValidityPeriodBegin) + "-" + Encoding.Unicode.GetString(info.arrTwoIdValidityPeriodEnd);


            byte[] dstImage = new byte[50 * 1024];
            int outlen = 1024 * 50;
            if (ReadCard.decodeImage(info.arrTwoIdPhoto, dstImage, ref outlen))
            {
                MemoryStream ms = new MemoryStream(dstImage);
                Image image = System.Drawing.Image.FromStream(ms);
                pictureBox1.Image = image; 
            }
        }
    }
}
