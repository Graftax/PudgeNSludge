using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace LevelEditor
{
    class PointLight : Light
    {
        VEC3F position;
        VEC3F atten;
        float radius;

        public VEC3F Position
        {
            get { return position; }
            set { position = value; }
        }

        public VEC3F Atten
        {
            get { return atten; }
            set { atten = value; }
        }

        public float Radius
        {
            get { return radius; }
            set { radius = value; }
        }

        public override string ToString()
        {
            string name = "Point Light";

            return name;
        }
    }
}
