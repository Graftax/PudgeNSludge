using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace LevelEditor
{
    class DirectionalLight : Light
    {
        VEC3F direction;

        public VEC3F Direction
        {
            get { return direction; }
            set { direction = value; }
        }

        public override string ToString()
        {
            string name = "Directional Light";

            return name;
        }
    }
}
