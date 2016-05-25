using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace LevelEditor
{
    class Bridge : Hazards
    {
        bool rotateClockwise;
        int quadrant;

        public bool RotateClockwise
        {
            get { return rotateClockwise; }
            set { rotateClockwise = value; }
        }

        public int Quadrant
        {
            get { return quadrant; }
            set { quadrant = value; }
        }
    }
}
