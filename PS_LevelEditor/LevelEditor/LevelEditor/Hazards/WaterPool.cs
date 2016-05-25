using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace LevelEditor
{
    class WaterPool : Hazards
    {
        float healamount;
        string meshName = "";

        public float HealAmount
        {
            get { return healamount; }
            set { healamount = value; }
        }

        public string MeshName
        {
            get { return meshName; }
            set { meshName = value; }
        }
    }
}
