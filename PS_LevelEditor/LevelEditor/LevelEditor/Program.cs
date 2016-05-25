using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using System.Diagnostics;

namespace LevelEditor
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        /// 

        [DllImport("Renderer")]
        public static extern bool _Init(Int32 handle, int width, int height);
        [DllImport("Renderer")]
        public static extern void _Render();
        [DllImport("Renderer")]
        public static extern void _Shutdown();

        [STAThread]
        static void Main()
        {
            //Application.EnableVisualStyles();
            //Application.SetCompatibleTextRenderingDefault(false);
            //Application.Run(new Form1());

            Form1 MainForm = new Form1();
            MainForm.Show();
            Stopwatch stopwatch = new Stopwatch();
            Timer time = new Timer();

            _Init(MainForm.DisplayPanel.Handle.ToInt32(), MainForm.DisplayPanel.Size.Width, MainForm.DisplayPanel.Size.Height);
            stopwatch.Start();
            time.Start();

            while (MainForm.Created)
            {

                
                float elapsedtime = stopwatch.Elapsed.Milliseconds / 500.0f;
                stopwatch.Reset();
                stopwatch.Start();
                MainForm.Update(elapsedtime);
                Application.DoEvents();
               _Render();

              
            }

            _Shutdown();
        }
    }
}
