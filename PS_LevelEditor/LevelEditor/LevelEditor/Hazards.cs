using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;
using System.Windows.Forms;

namespace LevelEditor
{
    class Hazards
    {
        private string name;
        private Point position;
        private int type;
        private RadioButton button;
        private int vectorPos;
        private int channel = -1;

        public string Name
        {
            get { return name; }
            set { name = value; }
        }

        public Point Position
        {
            get { return position; }
            set { position = value; }
        }

        public int Type
        {
            get { return type; }
            set { type = value; }
        }

        public int Channel
        {
            get { return channel; }
            set { channel = value; }
        }

        public RadioButton Button
        {
            get { return button; }
            set { button = value; }
        }

        public int VectorPos
        {
            get { return vectorPos; }
            set { vectorPos = value; }
        }

        public override string ToString()
        {
            return name;
        }
    }
}
