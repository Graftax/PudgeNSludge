using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace LevelEditor
{
    class GooPit : Hazards
    {
        // damage per sec
        float damage = 0.0f;

        public float Damage
        {
            get { return damage; }
            set { damage = value; }
        }
    }
}
