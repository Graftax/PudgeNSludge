using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace LevelEditor
{
    class DoorButton : Hazards
    {
        private string wireName = "";

        public string WireName
        {
            get { return wireName; }
            set { wireName = value; }
        }
    }
}
