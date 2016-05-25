using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace LevelEditor
{
    class Crusher : Hazards
    {
        int length = 1;
        int direction = 1;
        float frequency = 0.0f;
        float duration = 0.0f;

        public int Length
        {
            get { return length; }
            set { length = value; }
        }

        public int Direction
        {
            get { return direction; }
            set { direction = value; }
        }

        public float Frequency
        {
            get { return frequency; }
            set { frequency = value; }
        }

        public float Duration
        {
            get { return duration; }
            set { duration = value; }
        }
    }
}
