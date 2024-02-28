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
            CardInfoStruct card = ReadCard.ReadCardNo(offLineRead.Checked);

            lblAddress.Text = Encoding.Unicode.GetString(card.info.twoId.arrAddress);
            lblSex.Text = Encoding.Unicode.GetString(card.info.twoId.arrSex);
            lblBirthday.Text = Encoding.Unicode.GetString(card.info.twoId.arrBirthday);
            lblDept.Text = Encoding.Unicode.GetString(card.info.twoId.arrSignedDepartment);
            lblIdCard.Text = Encoding.Unicode.GetString(card.info.twoId.arrNo);
            lblName.Text = Encoding.Unicode.GetString(card.info.twoId.arrName);
            lblNation.Text = Encoding.Unicode.GetString(card.info.twoId.arrNation);
            lblValidDate.Text = Encoding.Unicode.GetString(card.info.twoId.arrValidityPeriodBegin) + "-" + Encoding.Unicode.GetString(card.info.twoId.arrValidityPeriodEnd);


            byte[] dstImage = new byte[50 * 1024];
            int outlen = 1024 * 50;
            if (ReadCard.decodeImage(card.info.twoId.arrPhoto, dstImage, ref outlen))
            {
                MemoryStream ms = new MemoryStream(dstImage);
                Image image = System.Drawing.Image.FromStream(ms);
                pictureBox1.Image = image;
            }

            byte[] sfzImage = new byte[200 * 1024];
            int outSfzLen = 1024 * 200;
            if (ReadCard.getSFZBmp(card, sfzImage, ref outSfzLen))
            {
                try
                {
                    using (FileStream fs = new FileStream("sfz.bmp", FileMode.Create))
                    {
                        // 将字节数组写入到文件流中
                        fs.Write(sfzImage, 0, outSfzLen);
                        fs.Close();
                    }
                }
                catch (Exception ex)
                {
                }
            }
        }
    }
}
