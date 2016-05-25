using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace LevelEditor
{
    class SteamIntake : Hazards
    {
        int direction = 1;
        bool corkable = false;
        string flowName = "";

        public int Direction
        {
            get { return direction; }
            set { direction = value; }
        }

        public bool Corkable
        {
            get { return corkable; }
            set { corkable = value; }
        }

        public string FlowName
        {
            get { return flowName; }
            set { flowName = value; }
        }
    }
}
