using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace LevelEditor
{
    class SpawnPipe : Hazards
    {
        int maxSpawns = 1;
        int direction = 1;
        float frequency = 0.0f;
        bool corkable = false;

        public int MaxSpawns
        {
            get { return maxSpawns; }
            set { maxSpawns = value; }
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

        public bool Corkable
        {
            get { return corkable; }
            set { corkable = value; }
        }
    }
}
