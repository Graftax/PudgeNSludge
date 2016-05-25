using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace LevelEditor
{
    class SteamPipe : Hazards
    {
        int steamLength = 1;
        int direction = 1;
        float frequency = 0.0f;
        float duration = 0.0f;
        bool constant = true;

        public int SteamLength
        {
            get { return steamLength; }
            set { steamLength = value; }
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

        public bool Constant
        {
            get { return constant; }
            set { constant = value; }
        }
    }
}
