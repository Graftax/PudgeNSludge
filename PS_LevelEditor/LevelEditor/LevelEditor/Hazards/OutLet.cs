using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace LevelEditor
{
    class OutLet : Hazards
    {
        int direction = 1;

        public int Direction
        {
            get { return direction; }
            set { direction = value; }
        }
    }
}
