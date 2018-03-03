using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Drawing.Imaging;
using Tao.OpenGl;
using System.Windows.Media.Media3D;

namespace SENSE
{
    public partial class _Form_Visualize : Form
    {

        public _Form_Visualize()
        {
            InitializeComponent();
        }
        
        //default
        private static Vector3D Visualize_Size = new Vector3D(4.2f, 1.3f, 0.7f);
        private static Vector3D dCAM_Pos = new Vector3D(0, 0, -30);
        private static Vector3D dCAM_rot = new Vector3D(-90, 0, -270);

        //variable
        private Vector3D CAM_Rot = dCAM_rot;
        private Vector3D CAM_Pos = dCAM_Pos;
        private Quaternion QUATERNION = new Quaternion(0, 0, 0, 1);
        private double[] DCM = new double[9];
        private float Dimension_Xp, Dimension_Yp, Dimension_Zp, Dimension_Xn, Dimension_Yn, Dimension_Zn;
        private Point LeftMouse, RightMouse;
        private Vector3D LeftOffset;
        private Vector3D RightOffset;
        private bool ToolTipEnable;

        private Timer Timer;
        private uint[] textures;
        private float[] Matrix;

#region User Input
        
        private void Visualize_KeyDown(object sender, KeyEventArgs e)
        {
            switch (e.KeyCode)
            {
                case Keys.PageUp:
                    if (CAM_Pos.Z < 0) CAM_Pos.Z++;
                    break;

                case Keys.PageDown:
                    CAM_Pos.Z--;
                    break;

                case Keys.Space:
                    CAM_Rot.X = dCAM_rot.X;
                    CAM_Rot.Y = dCAM_rot.Y;
                    CAM_Rot.Z = dCAM_rot.Z;
                    CAM_Pos.X = dCAM_Pos.X;
                    CAM_Pos.Y = dCAM_Pos.Y;
                    CAM_Pos.Z = dCAM_Pos.Z;
                    break;
            }
            ToolTipEnable = true;
        }

        private void Visualize_MouseMove(object sender, MouseEventArgs e)
        {
            //drag mouse on left button to rotate origin
            if (e.Button == MouseButtons.Left)
            {
                CAM_Rot.X = LeftOffset.X + 180 * (e.Location.Y - LeftMouse.Y) / Visualize.Height;
                CAM_Rot.Z = LeftOffset.Z + 360 * (e.Location.X - LeftMouse.X) / Visualize.Width;
            }
            else
            {
                LeftMouse.X = e.Location.X;
                LeftMouse.Y = e.Location.Y;
            }

            //drag mouse on right button to move Camera
            if (e.Button == MouseButtons.Right)
            {
                CAM_Pos.X = RightOffset.X + (e.Location.X - RightMouse.X) * 0.027f;
                CAM_Pos.Y = RightOffset.Y - (e.Location.Y - RightMouse.Y) * 0.027f;
            }
            else
            {
                RightMouse.X = e.Location.X;
                RightMouse.Y = e.Location.Y;
            }

            CAM_Pos.Z += e.Delta;

        }

        private void Visualize_MouseDown(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Left)
            {
                LeftOffset.X = CAM_Rot.X;
                LeftOffset.Z = CAM_Rot.Y;
            }
            if (e.Button == MouseButtons.Right)
            {
                RightOffset.X = CAM_Pos.X;
                RightOffset.Y = CAM_Pos.Y;
            }
            ToolTipEnable = true;
        }

        private void Visualize_MouseLeave(object sender, EventArgs e)
        {
            ToolTipEnable = false;
            toolTip1.Hide(Visualize);
        }

        private void Visualize_KeyUp(object sender, KeyEventArgs e)
        {
            ToolTipEnable = false;
            toolTip1.Hide(Visualize);
        }

        private void Visualize_MouseUp(object sender, MouseEventArgs e)
        {
            ToolTipEnable = false;
            toolTip1.Hide(Visualize);
        }
        
#endregion

#region Graphic Process

        private void GraphicTimer_Tick(object sender, EventArgs e)
        {
            lock (this)
            {
                Visualize.Refresh();
            }
        }

        private void Visualize_Load(object sender, EventArgs e)
        {
            //Compute object position and dimension
            Dimension_Xp = (float) Visualize_Size.X / 2;
            Dimension_Yp = (float) Visualize_Size.Y / 2;
            Dimension_Zp = (float) Visualize_Size.Z / 2;
            Dimension_Xn = (float)-Visualize_Size.X / 2;
            Dimension_Yn = (float)-Visualize_Size.Y / 2;
            Dimension_Zn = (float)-Visualize_Size.Z / 2;

            //init transformation matrix
            Matrix = new float[] {1.0f, 0.0f, 0.0f, 0.0f,
                                                                        0.0f, 1.0f, 0.0f, 0.0f,
                                                                        0.0f, 0.0f, 1.0f, 0.0f,
                                                                        0.0f, 0.0f, 0.0f, 1.0f};

            //File path
            string[] imageFiles = new string[] {  
                                            "image/Xplus.png", 
                                            "image/Xminus.png",  
                                            "image/Yplus.png", 
                                            "image/Yminus.png",
                                            "image/Zplus.png", 
                                            "image/Zminus.png" 
                                            };

            //Config Gl control
            Visualize.InitializeContexts();
            Visualize.SwapBuffers();
            Visualize_SizeChanged(sender, e);
            Gl.glShadeModel(Gl.GL_SMOOTH);
            Gl.glEnable(Gl.GL_LINE_SMOOTH);
            Gl.glEnable(Gl.GL_TEXTURE_2D);						    // Enable Texture Mapping        
            Gl.glEnable(Gl.GL_NORMALIZE);
            Gl.glEnable(Gl.GL_COLOR_MATERIAL);
            Gl.glEnable(Gl.GL_DEPTH_TEST);						    // Enables Depth Testing
            Gl.glEnable(Gl.GL_BLEND);
            Gl.glEnable(Gl.GL_LIGHTING);
            Gl.glEnable(Gl.GL_LIGHT0);
            Gl.glHint(Gl.GL_POLYGON_SMOOTH_HINT, Gl.GL_NICEST);     // Really Nice Point Smoothing
            Gl.glClearColor(0.6f, 0.7f, 0.9f, 0.2f);
            //Load file
            textures = LoadTexture(imageFiles);

            //Setup timer
            Timer = new Timer();
            Timer.Interval = 20;
            Timer.Tick += new EventHandler(GraphicTimer_Tick);
            Timer.Start();
        }

        private uint[] LoadTexture(string[] filesNames)
        {
            int numOfPic = filesNames.Length;
            uint[] texture = new uint[numOfPic];
            Bitmap[] bitmap = new Bitmap[numOfPic];
            BitmapData[] bitmapdata = new BitmapData[numOfPic];
            for (int im = 0; im < numOfPic; im++)
            {
                if (File.Exists(filesNames[im]))
                {
                    bitmap[im] = new Bitmap(filesNames[im]);
                }
            }
            Gl.glGenTextures(numOfPic, texture);
            for (int i = 0; i < numOfPic; i++)
            {
                bitmap[i].RotateFlip(RotateFlipType.RotateNoneFlipY);
                Rectangle rect = new Rectangle(0, 0, bitmap[i].Width, bitmap[i].Height);
                bitmapdata[i] = bitmap[i].LockBits(rect, ImageLockMode.ReadOnly, PixelFormat.Format24bppRgb);
                Gl.glBindTexture(Gl.GL_TEXTURE_2D, texture[i]);
                Gl.glTexEnvf(Gl.GL_TEXTURE_ENV, Gl.GL_TEXTURE_ENV_MODE, Gl.GL_MODULATE);
                Gl.glTexParameteri(Gl.GL_TEXTURE_2D, Gl.GL_TEXTURE_MAG_FILTER, Gl.GL_NEAREST);
                Gl.glTexParameteri(Gl.GL_TEXTURE_2D, Gl.GL_TEXTURE_MIN_FILTER, Gl.GL_NEAREST);
                Glu.gluBuild2DMipmaps(Gl.GL_TEXTURE_2D, (int)Gl.GL_RGB, bitmap[i].Width, bitmap[i].Height, Gl.GL_BGR_EXT, Gl.GL_UNSIGNED_BYTE, bitmapdata[i].Scan0);
                bitmap[i].UnlockBits(bitmapdata[i]);
                bitmap[i].Dispose();
            }
            return texture;
        }

        private void Visualize_SizeChanged(object sender, EventArgs e)
        {

            int height = Visualize.Size.Height;
            int width = Visualize.Size.Width;
            Gl.glViewport(0, 0, width, height);
            Gl.glPushMatrix();
            Gl.glMatrixMode(Gl.GL_PROJECTION);
            Gl.glLoadIdentity();
            Glu.gluPerspective(10, (float)width / (float)height, 1.0, 250);
            Gl.glMatrixMode(Gl.GL_MODELVIEW);
            Gl.glPopMatrix();
        }

        private void Render(object sender, PaintEventArgs e)
        {
            Matrix[0]   = (float)DCM[0];
            Matrix[1]   = (float)DCM[3];
            Matrix[2]   = (float)DCM[6];
            Matrix[4]   = (float)DCM[1];
            Matrix[5]   = (float)DCM[4];
            Matrix[6]   = (float)DCM[7];
            Matrix[8]   = (float)DCM[2];
            Matrix[9]   = (float)DCM[5];
            Matrix[10]  = (float)DCM[8];

            Gl.glClear(Gl.GL_COLOR_BUFFER_BIT | Gl.GL_DEPTH_BUFFER_BIT);    // Clear screen and DepthBuffer
            Gl.glLoadIdentity();
            Gl.glPolygonMode(Gl.GL_FRONT, Gl.GL_FILL);

            // Set camera view and distance
            Gl.glTranslatef((float)CAM_Pos.X, 0, 0);    //positive x points right
            Gl.glTranslatef(0, (float)CAM_Pos.Y, 0);    //positive z points up
            Gl.glTranslatef(0, 0, (float)CAM_Pos.Z);   //positive z points into screen

            // Rotate obj reference
            Gl.glRotatef((float)CAM_Rot.X, 1, 0, 0);
            Gl.glRotatef((float)CAM_Rot.Y, 0, 1, 0);
            Gl.glRotatef((float)CAM_Rot.Z, 0, 0, 1);

            // Push matrix
            Gl.glPushMatrix();
            // apply transformation matrix to cuboid
            Gl.glMultMatrixf(Matrix);


            // +'ve x face
            Gl.glBindTexture(Gl.GL_TEXTURE_2D, textures[0]);
            Gl.glBegin(Gl.GL_QUADS);
            Gl.glNormal3f(1, 0, 0); Gl.glTexCoord2f(0, 0); Gl.glVertex3f(Dimension_Xp, Dimension_Yn, Dimension_Zn);
            Gl.glNormal3f(1, 0, 0); Gl.glTexCoord2f(0, 1); Gl.glVertex3f(Dimension_Xp, Dimension_Yn, Dimension_Zp);
            Gl.glNormal3f(1, 0, 0); Gl.glTexCoord2f(1, 1); Gl.glVertex3f(Dimension_Xp, Dimension_Yp, Dimension_Zp);
            Gl.glNormal3f(1, 0, 0); Gl.glTexCoord2f(1, 0); Gl.glVertex3f(Dimension_Xp, Dimension_Yp, Dimension_Zn);
            Gl.glEnd();

            // -'ve x face
            Gl.glBindTexture(Gl.GL_TEXTURE_2D, textures[1]);
            Gl.glBegin(Gl.GL_QUADS);
            Gl.glNormal3f(-1, 0, 0); Gl.glTexCoord2f(1, 0); Gl.glVertex3f(Dimension_Xn, Dimension_Yn, Dimension_Zn);
            Gl.glNormal3f(-1, 0, 0); Gl.glTexCoord2f(1, 1); Gl.glVertex3f(Dimension_Xn, Dimension_Yn, Dimension_Zp);
            Gl.glNormal3f(-1, 0, 0); Gl.glTexCoord2f(0, 1); Gl.glVertex3f(Dimension_Xn, Dimension_Yp, Dimension_Zp);
            Gl.glNormal3f(-1, 0, 0); Gl.glTexCoord2f(0, 0); Gl.glVertex3f(Dimension_Xn, Dimension_Yp, Dimension_Zn);
            Gl.glEnd();

            // +'ve y face
            Gl.glBindTexture(Gl.GL_TEXTURE_2D, textures[2]);
            Gl.glBegin(Gl.GL_QUADS);
            Gl.glNormal3f(0, 1, 0); Gl.glTexCoord2f(1, 0); Gl.glVertex3f(Dimension_Xn, Dimension_Yp, Dimension_Zn);
            Gl.glNormal3f(0, 1, 0); Gl.glTexCoord2f(1, 1); Gl.glVertex3f(Dimension_Xn, Dimension_Yp, Dimension_Zp);
            Gl.glNormal3f(0, 1, 0); Gl.glTexCoord2f(0, 1); Gl.glVertex3f(Dimension_Xp, Dimension_Yp, Dimension_Zp);
            Gl.glNormal3f(0, 1, 0); Gl.glTexCoord2f(0, 0); Gl.glVertex3f(Dimension_Xp, Dimension_Yp, Dimension_Zn);
            Gl.glEnd();

            // -'ve y face
            Gl.glBindTexture(Gl.GL_TEXTURE_2D, textures[3]);
            Gl.glBegin(Gl.GL_QUADS);
            Gl.glNormal3f(0, -1, 0); Gl.glTexCoord2f(0, 0); Gl.glVertex3f(Dimension_Xn, Dimension_Yn, Dimension_Zn);
            Gl.glNormal3f(0, -1, 0); Gl.glTexCoord2f(0, 1); Gl.glVertex3f(Dimension_Xn, Dimension_Yn, Dimension_Zp);
            Gl.glNormal3f(0, -1, 0); Gl.glTexCoord2f(1, 1); Gl.glVertex3f(Dimension_Xp, Dimension_Yn, Dimension_Zp);
            Gl.glNormal3f(0, -1, 0); Gl.glTexCoord2f(1, 0); Gl.glVertex3f(Dimension_Xp, Dimension_Yn, Dimension_Zn);
            Gl.glEnd();

            // +'ve z face
            Gl.glBindTexture(Gl.GL_TEXTURE_2D, textures[4]);
            Gl.glBegin(Gl.GL_QUADS);
            Gl.glNormal3f(0, 0, 1); Gl.glTexCoord2f(0, 0); Gl.glVertex3f(Dimension_Xn, Dimension_Yn, Dimension_Zp);
            Gl.glNormal3f(0, 0, 1); Gl.glTexCoord2f(1, 0); Gl.glVertex3f(Dimension_Xp, Dimension_Yn, Dimension_Zp);
            Gl.glNormal3f(0, 0, 1); Gl.glTexCoord2f(1, 1); Gl.glVertex3f(Dimension_Xp, Dimension_Yp, Dimension_Zp);
            Gl.glNormal3f(0, 0, 1); Gl.glTexCoord2f(0, 1); Gl.glVertex3f(Dimension_Xn, Dimension_Yp, Dimension_Zp);
            Gl.glEnd();

            // -'ve z face
            Gl.glBindTexture(Gl.GL_TEXTURE_2D, textures[5]);
            Gl.glBegin(Gl.GL_QUADS);
            Gl.glNormal3f(0, 0, -1); Gl.glTexCoord2f(0, 1); Gl.glVertex3f(Dimension_Xn, Dimension_Yn, Dimension_Zn);
            Gl.glNormal3f(0, 0, -1); Gl.glTexCoord2f(1, 1); Gl.glVertex3f(Dimension_Xp, Dimension_Yn, Dimension_Zn);
            Gl.glNormal3f(0, 0, -1); Gl.glTexCoord2f(1, 0); Gl.glVertex3f(Dimension_Xp, Dimension_Yp, Dimension_Zn);
            Gl.glNormal3f(0, 0, -1); Gl.glTexCoord2f(0, 0); Gl.glVertex3f(Dimension_Xn, Dimension_Yp, Dimension_Zn);
            Gl.glEnd();


            // -axis


            Gl.glPopMatrix();
            Gl.glFlush();

            if(ToolTipEnable)
                toolTip1.Show(
                                "Cam pos " + CAM_Pos.X.ToString("f0") + " " + CAM_Pos.Y.ToString("f0") + " " + CAM_Pos.Z.ToString("f0") + "\n" +
                                "Cam rot " + CAM_Rot.X.ToString("f0") + " " + CAM_Rot.Y.ToString("f0") + " " + CAM_Rot.Z.ToString("f0"),             
                                Visualize, 0, 0
                             );

        }
        
#endregion
       
 
#region Extenal
        
        internal void UpdateData(double[] dcm)      { DCM = dcm; }

#endregion



    }
}
